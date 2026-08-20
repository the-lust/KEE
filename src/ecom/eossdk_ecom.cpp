#include "eossdk_ecom.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

static const char* ownership_status_to_string(EOS_EOwnershipStatus status)
{
    switch (status)
    {
#define OWNERSHIP_CASE(X) case EOS_EOwnershipStatus::X: return #X
        OWNERSHIP_CASE(EOS_OS_NotOwned);
        OWNERSHIP_CASE(EOS_OS_Owned);
        default: return "Unknown Ownership";
#undef  OWNERSHIP_CASE
    }
}

EOSSDK_Ecom::EOSSDK_Ecom()
{
    string kee_settings = Settings::Inst().kee_settings_path;

    catalog_filename = FileManager::join(kee_settings, "catalog.json");
    entitlements_filename = FileManager::join(kee_settings, "entitlements.json");

    FileManager::load_json(catalog_filename, m_catalog);
    
    if (std::filesystem::exists(entitlements_filename))
    {
        FileManager::load_json(entitlements_filename, m_entitlements);
    }
    else
    {
        
        string dlc_txt = "steam_settings/DLC.txt";
        if (std::filesystem::exists(dlc_txt))
        {
            std::ifstream file(dlc_txt);
            string line;
            while (std::getline(file, line))
            {
                line.erase(0, line.find_first_not_of(" \t\r\n"));
                line.erase(line.find_last_not_of(" \t\r\n") + 1);
                if (!line.empty() && line[0] != '#')
                {
                    
                    m_entitlements[line]["entitlement_name"] = "DLC_" + line;
                    m_entitlements[line]["catalog_item_id"] = line;
                    m_entitlements[line]["redeemed"] = false;
                }
            }
        }
    }

    GetCB_Manager().register_callbacks(this);
}

EOSSDK_Ecom::~EOSSDK_Ecom()
{
    GetCB_Manager().unregister_callbacks(this);
}

EOS_EResult EOSSDK_Ecom::copy_entitlement(typename decltype(m_queried_entitlements)::iterator it, EOS_Ecom_Entitlement** OutEntitlement)
{
    bool redeemed;
    string const* entitlement_id = nullptr;
    string const* entitlement_name = nullptr;
    string const* catalog_item_id = nullptr;
    bool error = false;

    entitlement_id = &it->first;
    try
    {
        entitlement_name = (*it->second)["entitlement_name"].get_ptr<string*>();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "%s \"entitlement_name\" field was not found, it will not be owned", entitlement_id->c_str());
        error = true;
    }
    try
    {
        catalog_item_id  = (*it->second)["catalog_item_id"].get_ptr<string*>();
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "%s \"catalog_item_id\" field was not found, it will not be owned", entitlement_id->c_str());
        error = true;
    }
    try
    {
        redeemed = (*it->second)["redeemed"];
    }
    catch (...)
    {
        APP_LOG(Log::LogLevel::ERR, "%s \"redeemed\" field was not found, it will not be redeemed", entitlement_id->c_str());
        redeemed = false;
    }

    if (error)
    {
        *OutEntitlement = nullptr;
        return EOS_EResult::EOS_NotFound;
    }

    EOS_Ecom_Entitlement* entitlement = new EOS_Ecom_Entitlement;
    entitlement->ApiVersion = EOS_ECOM_ENTITLEMENT_API_LATEST;
    entitlement->EntitlementName = entitlement_name->c_str();
    entitlement->EntitlementId = entitlement_id->c_str();
    entitlement->CatalogItemId = catalog_item_id->c_str();
    entitlement->ServerIndex = -1;
    entitlement->bRedeemed = redeemed;
    entitlement->EndTimestamp = -1;
    *OutEntitlement = entitlement;

    return EOS_EResult::EOS_Success;
}

void EOSSDK_Ecom::QueryOwnership(const EOS_Ecom_QueryOwnershipOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionDelegate == nullptr)
        return;

    pFrameResult_t res(new FrameResult);

    EOS_Ecom_QueryOwnershipCallbackInfo& qoci = res->CreateCallback<EOS_Ecom_QueryOwnershipCallbackInfo>((CallbackFunc)CompletionDelegate);

    int32_t itemownershipversion = EOS_ECOM_ITEMOWNERSHIP_API_LATEST;

    switch (Options->ApiVersion)
    {
        case 2: 
        {
            auto opts = Options;
            APP_LOG(Log::LogLevel::INFO, "TODO?: Check the catalog namespace");
            APP_LOG(Log::LogLevel::DEBUG, "CatalogNamespace: %s", (opts->CatalogNamespace == nullptr ? "" : opts->CatalogNamespace));
        }
        case 1:
        {
            auto opts = Options;
            qoci.LocalUserId = opts->LocalUserId;
            
            qoci.ItemOwnershipCount = Options->CatalogItemIdCount;
            APP_LOG(Log::LogLevel::DEBUG, "CatalogItemIdCount: %u", opts->CatalogItemIdCount);
            if (qoci.ItemOwnershipCount > 0)
            {
                EOS_Ecom_ItemOwnership* ownerships = new EOS_Ecom_ItemOwnership[qoci.ItemOwnershipCount];
                for (uint32_t i = 0; i < Options->CatalogItemIdCount; ++i)
                {
                    APP_LOG(Log::LogLevel::DEBUG, "CatalogItemIds[%u]: %s", i, (opts->CatalogItemIds[i] == nullptr ? "" : opts->CatalogItemIds[i]));

                    EOS_EOwnershipStatus owned = EOS_EOwnershipStatus::EOS_OS_NotOwned;
                    char* id;
                    if (opts->CatalogItemIds[i] != nullptr)
                    {
                        size_t idlen = strlen(opts->CatalogItemIds[i])+1;
                        id = new char[idlen];
                        memcpy(id, opts->CatalogItemIds[i], idlen);
                        
                        auto catalog_it = m_catalog.find(id);
                        if (catalog_it != m_catalog.end())
                        {
                            try
                            {
                                if (catalog_it.value()["owned"].get<bool>())
                                {
                                    owned = EOS_EOwnershipStatus::EOS_OS_Owned;
                                }
                                APP_LOG(Log::LogLevel::INFO, "Catalog Item id %s, %s (from %s)", id, ownership_status_to_string(owned), catalog_filename.c_str());
                            }
                            catch(...)
                            {
                                APP_LOG(Log::LogLevel::ERR, "Catalog Item id %s \"owned\" field was invalid, item not owned", id);
                            }
                        }
                        else
                        {
                            owned = (Settings::Inst().unlock_dlcs ? EOS_EOwnershipStatus::EOS_OS_Owned : EOS_EOwnershipStatus::EOS_OS_NotOwned);
                            APP_LOG(Log::LogLevel::INFO, "Catalog Item id %s, %s (from \"unlock_dlcs\")", id, ownership_status_to_string(owned));
                        }
                    }
                    else
                    {
                        id = new char[1];
                        *id = 0;

                        APP_LOG(Log::LogLevel::WARN, "Empty Catalog Item id, item not owned");
                    }

                    ownerships[i].OwnershipStatus = owned;
                    ownerships[i].ApiVersion = itemownershipversion;
                    ownerships[i].Id = id;
                }
                qoci.ItemOwnership = ownerships;
            }

        }
    }

    qoci.ClientData = ClientData;
    qoci.ResultCode = EOS_EResult::EOS_Success;
    
    SE_CB_END(this);
}

void EOSSDK_Ecom::QueryOwnershipToken(const EOS_Ecom_QueryOwnershipTokenOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOwnershipTokenCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionDelegate == nullptr)
        return;

    switch (Options->ApiVersion)
    {
        case 2: 
        {
            auto opts = Options;
            APP_LOG(Log::LogLevel::DEBUG, "CatalogNamespace: %s", (opts->CatalogNamespace == nullptr ? "" : opts->CatalogNamespace));
        }
        case 1:
        {
            auto opts = Options;
            APP_LOG(Log::LogLevel::DEBUG, "CatalogItemIdCount: %u", opts->CatalogItemIdCount);
            for (uint32_t i = 0; i < opts->CatalogItemIdCount; ++i)
            {
                APP_LOG(Log::LogLevel::DEBUG, "CatalogItemIds[%u]: %s", i, (opts->CatalogItemIds[i] == nullptr ? "" : opts->CatalogItemIds[i]));
            }
        }
    }

    APP_LOG(Log::LogLevel::INFO, "QueryOwnershipToken requested for %u items", Options->CatalogItemIdCount);

    pFrameResult_t res(new FrameResult);
    EOS_Ecom_QueryOwnershipTokenCallbackInfo& qotci = res->CreateCallback<EOS_Ecom_QueryOwnershipTokenCallbackInfo>((CallbackFunc)CompletionDelegate);
    
    qotci.ClientData = ClientData;
    qotci.ResultCode = EOS_EResult::EOS_Success;
    qotci.LocalUserId = Settings::Inst().userid;

    string token_str = "kee-ownership-token-" + Settings::Inst().userid_str;
    char* str = new char[token_str.length() + 1];
    strncpy(str, token_str.c_str(), token_str.length() + 1);
    qotci.OwnershipToken = str;
    
    SE_CB_END(this);
}

void EOSSDK_Ecom::QueryEntitlements(const EOS_Ecom_QueryEntitlementsOptions* Options, void* ClientData, const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Ecom_QueryEntitlementsCallbackInfo, CompletionDelegate);
        info.LocalUserId = Settings::Inst().userid;

    m_queried_entitlements.clear();
    if (Options != nullptr)
    {
        switch (Options->ApiVersion)
        {
            case 3: 
            case 2:
            {
                auto opts = Options;
                APP_LOG(Log::LogLevel::DEBUG, "bIncludeRedeemed: %d", (int)opts->bIncludeRedeemed);
                APP_LOG(Log::LogLevel::DEBUG, "EntitlementNameCount: %u", opts->EntitlementNameCount);
                for (uint32_t i = 0; i < opts->EntitlementNameCount; ++i)
                {
                    auto it = m_entitlements.find(opts->EntitlementNames[i]);
                    if (it != m_entitlements.end())
                    {
                        bool redeemed;
                        try
                        {
                            redeemed = it.value()["redeemed"];
                        }
                        catch (...)
                        {
                            redeemed = false;
                        }
                        if (!redeemed || Options->bIncludeRedeemed == EOS_TRUE)
                        {
                            APP_LOG(Log::LogLevel::DEBUG, "EntitlementNames[%u]: %s - Found", i, (opts->EntitlementNames[i] == nullptr ? "" : opts->EntitlementNames[i]));
                            m_queried_entitlements[opts->EntitlementNames[i]] = &it.value();
                        }
                        else
                        {
                            APP_LOG(Log::LogLevel::DEBUG, "EntitlementNames[%u]: %s - Found but was already redeemed and client asked for non-redeemed", i, (opts->EntitlementNames[i] == nullptr ? "" : opts->EntitlementNames[i]));
                        }
                    }
                    else
                    {
                        APP_LOG(Log::LogLevel::DEBUG, "EntitlementNames[%u]: %s - Not Found", i, (opts->EntitlementNames[i] == nullptr ? "" : opts->EntitlementNames[i]));
                    }
                }
            }
        }
        info.ResultCode = EOS_EResult::EOS_Success;
    }
    else
    {
            info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }

    SE_CB_END(this);
}

void EOSSDK_Ecom::QueryOffers(const EOS_Ecom_QueryOffersOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOffersCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionDelegate == nullptr)
        return;

    switch (Options->ApiVersion)
    {
        case 1: 
        {
            auto opts = Options;
            APP_LOG(Log::LogLevel::DEBUG, "OverrideCatalogNamespace: %s", (opts->OverrideCatalogNamespace == nullptr ? "" : opts->OverrideCatalogNamespace));
        }
    }
}

void EOSSDK_Ecom::Checkout(const EOS_Ecom_CheckoutOptions* Options, void* ClientData, const EOS_Ecom_OnCheckoutCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionDelegate == nullptr)
        return;

    if (Options)
    {
        APP_LOG(Log::LogLevel::DEBUG, "Checkout: EntryCount=%u Namespace=%s",
            Options->EntryCount,
            (Options->OverrideCatalogNamespace ? Options->OverrideCatalogNamespace : ""));
        for (uint32_t i = 0; i < Options->EntryCount; ++i)
        {
            auto* pEntry = &Options->Entries[i];
            if (pEntry && pEntry->OfferId)
                APP_LOG(Log::LogLevel::DEBUG, "  Entries[%u].OfferId=%s", i, pEntry->OfferId);
        }
    }

    pFrameResult_t res(new FrameResult);
    EOS_Ecom_CheckoutCallbackInfo& cci = res->CreateCallback<EOS_Ecom_CheckoutCallbackInfo>((CallbackFunc)CompletionDelegate);
    cci.ClientData    = ClientData;
    cci.LocalUserId   = Options ? Options->LocalUserId : nullptr;
    cci.ResultCode    = EOS_EResult::EOS_Success;

    static uint64_t tx_counter = 1;
    m_last_transaction_id = "kee-tx-" + std::to_string(tx_counter++);
    cci.TransactionId = m_last_transaction_id.c_str();

    SE_CB_END(this);
}

void EOSSDK_Ecom::RedeemEntitlements(const EOS_Ecom_RedeemEntitlementsOptions* Options, void* ClientData, const EOS_Ecom_OnRedeemEntitlementsCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);
    APP_LOG(Log::LogLevel::INFO, "TODO");

    if (CompletionDelegate == nullptr)
        return;

    switch (Options->ApiVersion)
    {
        case 1: 
        {
            auto opts = Options;
            APP_LOG(Log::LogLevel::DEBUG, "EntitlementIdCount: %u", opts->EntitlementIdCount);
            for (uint32_t i = 0; i < opts->EntitlementIdCount; ++i)
            {
                APP_LOG(Log::LogLevel::DEBUG, "EntitlementIds[%u]: %s", i, (opts->EntitlementIds[i] == nullptr ? "" : opts->EntitlementIds[i]));
            }
        }
    }
}

uint32_t EOSSDK_Ecom::GetEntitlementsCount(const EOS_Ecom_GetEntitlementsCountOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);
    
    if (Options == nullptr)
        return 0;

    return m_queried_entitlements.size();
}

uint32_t EOSSDK_Ecom::GetEntitlementsByNameCount(const EOS_Ecom_GetEntitlementsByNameCountOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    APP_LOG(Log::LogLevel::INFO, "EntitlementName: %s", Options->EntitlementName == nullptr ? "<No Name>" : Options->EntitlementName);

    if (Options == nullptr || Options->EntitlementName == nullptr)
        return 0;

    uint32_t count = std::count_if(m_queried_entitlements.begin(), m_queried_entitlements.end(), [Options]( std::pair<const string, fifo_json*> &item) 
    {
        try
        {
            return (*item.second)["entitlement_name"].get_ref<string&>() == Options->EntitlementName;
        }
        catch (...)
        {
            return false;
        }
    });

    return count;
}

EOS_EResult EOSSDK_Ecom::CopyEntitlementByIndex(const EOS_Ecom_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->EntitlementIndex >= m_queried_entitlements.size() || OutEntitlement == nullptr)
    {
        set_nullptr(OutEntitlement);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_queried_entitlements.begin();
    std::advance(it, Options->EntitlementIndex);

    return copy_entitlement(it, OutEntitlement);
}

EOS_EResult EOSSDK_Ecom::CopyEntitlementByNameAndIndex(const EOS_Ecom_CopyEntitlementByNameAndIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options == nullptr || Options->EntitlementName == nullptr || OutEntitlement == nullptr)
    {
        set_nullptr(OutEntitlement);
        return EOS_EResult::EOS_InvalidParameters;
    }

    int i = 0;
    auto it = m_queried_entitlements.begin();
    for (; it != m_queried_entitlements.end(); ++it)
    {
        if ((*it->second)["entitlement_name"].get_ref<string&>() == Options->EntitlementName)
        {
            if (i == Options->Index)
            {
                break;
            }
            ++i;
        }
    }
    if (it == m_queried_entitlements.end())
    {
        *OutEntitlement = nullptr;
        return EOS_EResult::EOS_NotFound;
    }
    
    return copy_entitlement(it, OutEntitlement);
}

EOS_EResult EOSSDK_Ecom::CopyEntitlementById(const EOS_Ecom_CopyEntitlementByIdOptions* Options, EOS_Ecom_Entitlement** OutEntitlement)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    APP_LOG(Log::LogLevel::INFO, "Entitlement id: %s", Options->EntitlementId == nullptr ? "<no id>" : Options->EntitlementId);

    if (Options == nullptr || Options->EntitlementId == nullptr || OutEntitlement == nullptr)
    {
        set_nullptr(OutEntitlement);
        return EOS_EResult::EOS_InvalidParameters;
    }

    auto it = m_queried_entitlements.find(Options->EntitlementId);
    if (it == m_queried_entitlements.end())
    {
        *OutEntitlement = nullptr;
        return EOS_EResult::EOS_NotFound;
    }

    return copy_entitlement(it, OutEntitlement);
}

uint32_t EOSSDK_Ecom::GetOfferCount(const EOS_Ecom_GetOfferCountOptions* Options)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    return 0;
}

EOS_EResult EOSSDK_Ecom::CopyOfferByIndex(const EOS_Ecom_CopyOfferByIndexOptions* Options, EOS_Ecom_CatalogOffer** OutOffer)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutOffer);
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Ecom::CopyOfferById(const EOS_Ecom_CopyOfferByIdOptions* Options, EOS_Ecom_CatalogOffer** OutOffer)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutOffer);
    return EOS_EResult::EOS_NotFound;
}

uint32_t EOSSDK_Ecom::GetOfferItemCount(const EOS_Ecom_GetOfferItemCountOptions* Options)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    return 0;
}

EOS_EResult EOSSDK_Ecom::CopyOfferItemByIndex(const EOS_Ecom_CopyOfferItemByIndexOptions* Options, EOS_Ecom_CatalogItem** OutItem)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutItem);
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Ecom::CopyItemById(const EOS_Ecom_CopyItemByIdOptions* Options, EOS_Ecom_CatalogItem** OutItem)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutItem);
    return EOS_EResult::EOS_NotFound;
}

uint32_t EOSSDK_Ecom::GetOfferImageInfoCount(const EOS_Ecom_GetOfferImageInfoCountOptions* Options)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    return 0;
}

EOS_EResult EOSSDK_Ecom::CopyOfferImageInfoByIndex(const EOS_Ecom_CopyOfferImageInfoByIndexOptions* Options, EOS_Ecom_KeyImageInfo** OutImageInfo)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutImageInfo);
    return EOS_EResult::EOS_NotFound;
}

uint32_t EOSSDK_Ecom::GetItemImageInfoCount(const EOS_Ecom_GetItemImageInfoCountOptions* Options)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    return 0;
}

EOS_EResult EOSSDK_Ecom::CopyItemImageInfoByIndex(const EOS_Ecom_CopyItemImageInfoByIndexOptions* Options, EOS_Ecom_KeyImageInfo** OutImageInfo)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutImageInfo);
    return EOS_EResult::EOS_NotFound;
}

uint32_t EOSSDK_Ecom::GetItemReleaseCount(const EOS_Ecom_GetItemReleaseCountOptions* Options)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    return 0;
}

EOS_EResult EOSSDK_Ecom::CopyItemReleaseByIndex(const EOS_Ecom_CopyItemReleaseByIndexOptions* Options, EOS_Ecom_CatalogRelease** OutRelease)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutRelease);
    return EOS_EResult::EOS_NotFound;
}

uint32_t EOSSDK_Ecom::GetTransactionCount(const EOS_Ecom_GetTransactionCountOptions* Options)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);
    
    return 0;
}

EOS_EResult EOSSDK_Ecom::CopyTransactionByIndex(const EOS_Ecom_CopyTransactionByIndexOptions* Options, EOS_Ecom_HTransaction* OutTransaction)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutTransaction);
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_Ecom::CopyTransactionById(const EOS_Ecom_CopyTransactionByIdOptions* Options, EOS_Ecom_HTransaction* OutTransaction)
{
    TRACE_FUNC();
    APP_LOG(Log::LogLevel::INFO, "TODO");
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutTransaction);
    return EOS_EResult::EOS_NotFound;
}

bool EOSSDK_Ecom::CBRunFrame()
{
    return false;
}

bool EOSSDK_Ecom::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;
}

void EOSSDK_Ecom::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    switch (res->ICallback())
    {

        case EOS_Ecom_QueryOwnershipCallbackInfo::k_iCallback:
        {
            EOS_Ecom_QueryOwnershipCallbackInfo& callback = res->GetCallback<EOS_Ecom_QueryOwnershipCallbackInfo>();
            if (callback.ItemOwnershipCount > 0)
            {
                for (uint32_t i = 0; i < callback.ItemOwnershipCount; ++i)
                    delete[]callback.ItemOwnership[i].Id;

                delete[] callback.ItemOwnership;
            }
        }
        break;

        case EOS_Ecom_QueryOwnershipTokenCallbackInfo::k_iCallback:
        {
            EOS_Ecom_QueryOwnershipTokenCallbackInfo& callback = res->GetCallback<EOS_Ecom_QueryOwnershipTokenCallbackInfo>();
            delete[]callback.OwnershipToken;
        }
        break;

    }
}

EOS_EResult EOSSDK_Ecom_Transaction::GetTransactionId(char* OutBuffer, int32_t* InOutBufferLength)
{
    TRACE_FUNC();

    return EOS_EResult::EOS_NotFound;
}

uint32_t EOSSDK_Ecom_Transaction::GetEntitlementsCount(const EOS_Ecom_Transaction_GetEntitlementsCountOptions* Options)
{
    TRACE_FUNC();

    return 0;
}

EOS_EResult EOSSDK_Ecom_Transaction::CopyEntitlementByIndex(const EOS_Ecom_Transaction_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement)
{
    TRACE_FUNC();

    return EOS_EResult::EOS_NotFound;
}

}