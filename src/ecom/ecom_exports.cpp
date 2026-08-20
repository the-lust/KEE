#include "ecom/eossdk_ecom.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Ecom_QueryOwnership(EOS_HEcom Handle, const EOS_Ecom_QueryOwnershipOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        pInst->QueryOwnership(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_QueryOwnershipToken(EOS_HEcom Handle, const EOS_Ecom_QueryOwnershipTokenOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOwnershipTokenCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        pInst->QueryOwnershipToken(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_QueryEntitlements(EOS_HEcom Handle, const EOS_Ecom_QueryEntitlementsOptions* Options, void* ClientData, const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        pInst->QueryEntitlements(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_QueryOffers(EOS_HEcom Handle, const EOS_Ecom_QueryOffersOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOffersCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        pInst->QueryOffers(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_Checkout(EOS_HEcom Handle, const EOS_Ecom_CheckoutOptions* Options, void* ClientData, const EOS_Ecom_OnCheckoutCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        pInst->Checkout(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_RedeemEntitlements(EOS_HEcom Handle, const EOS_Ecom_RedeemEntitlementsOptions* Options, void* ClientData, const EOS_Ecom_OnRedeemEntitlementsCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        pInst->RedeemEntitlements(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_QueryEntitlementToken(EOS_HEcom Handle, const EOS_Ecom_QueryEntitlementTokenOptions* Options, void* ClientData, const EOS_Ecom_OnQueryEntitlementTokenCallback CompletionDelegate)
    {
        if (CompletionDelegate) {
            EOS_Ecom_QueryEntitlementTokenCallbackInfo info;
            info.ResultCode = EOS_EResult::EOS_NotImplemented;
            info.ClientData = ClientData;
            info.LocalUserId = Options ? Options->LocalUserId : nullptr;
            CompletionDelegate(&info);
        }
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetEntitlementsCount(EOS_HEcom Handle, const EOS_Ecom_GetEntitlementsCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->GetEntitlementsCount(Options);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetEntitlementsByNameCount(EOS_HEcom Handle, const EOS_Ecom_GetEntitlementsByNameCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->GetEntitlementsByNameCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyEntitlementByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyEntitlementByIndex(Options, OutEntitlement);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyEntitlementByNameAndIndex(EOS_HEcom Handle, const EOS_Ecom_CopyEntitlementByNameAndIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyEntitlementByNameAndIndex(Options, OutEntitlement);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyEntitlementById(EOS_HEcom Handle, const EOS_Ecom_CopyEntitlementByIdOptions* Options, EOS_Ecom_Entitlement** OutEntitlement)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyEntitlementById(Options, OutEntitlement);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetOfferCount(EOS_HEcom Handle, const EOS_Ecom_GetOfferCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->GetOfferCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyOfferByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyOfferByIndexOptions* Options, EOS_Ecom_CatalogOffer** OutOffer)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyOfferByIndex(Options, OutOffer);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyOfferById(EOS_HEcom Handle, const EOS_Ecom_CopyOfferByIdOptions* Options, EOS_Ecom_CatalogOffer** OutOffer)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyOfferById(Options, OutOffer);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetOfferItemCount(EOS_HEcom Handle, const EOS_Ecom_GetOfferItemCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->GetOfferItemCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyOfferItemByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyOfferItemByIndexOptions* Options, EOS_Ecom_CatalogItem** OutItem)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyOfferItemByIndex(Options, OutItem);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyItemById(EOS_HEcom Handle, const EOS_Ecom_CopyItemByIdOptions* Options, EOS_Ecom_CatalogItem** OutItem)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyItemById(Options, OutItem);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetOfferImageInfoCount(EOS_HEcom Handle, const EOS_Ecom_GetOfferImageInfoCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->GetOfferImageInfoCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyOfferImageInfoByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyOfferImageInfoByIndexOptions* Options, EOS_Ecom_KeyImageInfo** OutImageInfo)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyOfferImageInfoByIndex(Options, OutImageInfo);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetItemImageInfoCount(EOS_HEcom Handle, const EOS_Ecom_GetItemImageInfoCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->GetItemImageInfoCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyItemImageInfoByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyItemImageInfoByIndexOptions* Options, EOS_Ecom_KeyImageInfo** OutImageInfo)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyItemImageInfoByIndex(Options, OutImageInfo);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetItemReleaseCount(EOS_HEcom Handle, const EOS_Ecom_GetItemReleaseCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->GetItemReleaseCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyItemReleaseByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyItemReleaseByIndexOptions* Options, EOS_Ecom_CatalogRelease** OutRelease)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyItemReleaseByIndex(Options, OutRelease);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetTransactionCount(EOS_HEcom Handle, const EOS_Ecom_GetTransactionCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->GetTransactionCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyTransactionByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyTransactionByIndexOptions* Options, EOS_Ecom_HTransaction* OutTransaction)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyTransactionByIndex(Options, OutTransaction);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyTransactionById(EOS_HEcom Handle, const EOS_Ecom_CopyTransactionByIdOptions* Options, EOS_Ecom_HTransaction* OutTransaction)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom*>(Handle);
        return pInst->CopyTransactionById(Options, OutTransaction);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_Transaction_GetTransactionId(EOS_Ecom_HTransaction Handle, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom_Transaction*>(Handle);
        return pInst->GetTransactionId(OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_Transaction_GetEntitlementsCount(EOS_Ecom_HTransaction Handle, const EOS_Ecom_Transaction_GetEntitlementsCountOptions* Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Ecom_Transaction*>(Handle);
        return pInst->GetEntitlementsCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_Transaction_CopyEntitlementByIndex(EOS_Ecom_HTransaction Handle, const EOS_Ecom_Transaction_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Ecom_Transaction*>(Handle);
        return pInst->CopyEntitlementByIndex(Options, OutEntitlement);
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_Transaction_Release(EOS_Ecom_HTransaction Handle)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Ecom_Transaction*>(Handle);
        delete pInst;
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_Entitlement_Release(EOS_Ecom_Entitlement* Entitlement)
    {
        if (Entitlement == nullptr)
            return;

        delete[]Entitlement->EntitlementId;
        delete[]Entitlement->CatalogItemId;
        delete Entitlement;
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_CatalogOffer_Release(EOS_Ecom_CatalogOffer* Offer)
    {
        if (Offer == nullptr)
            return;

        delete[]Offer->Id;
        delete[]Offer->TitleText;
        delete[]Offer->DescriptionText;
        delete[]Offer->LongDescriptionText;
        delete[]Offer->CurrencyCode;
        delete Offer;
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_CatalogItem_Release(EOS_Ecom_CatalogItem* Item)
    {
        if (Item == nullptr)
            return;

        delete[]Item->Id;
        delete[]Item->TitleText;
        delete[]Item->DescriptionText;
        delete[]Item->LongDescriptionText;
        delete Item;
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_KeyImageInfo_Release(EOS_Ecom_KeyImageInfo* ImageInfo)
    {
        if (ImageInfo == nullptr)
            return;

        delete[]ImageInfo->Url;
        delete[]ImageInfo->Type;
        delete ImageInfo;
    }

    EOS_DECLARE_FUNC(void) EOS_Ecom_CatalogRelease_Release(EOS_Ecom_CatalogRelease* Release)
    {
        if (Release == nullptr)
            return;

        for (uint32_t i = 0; i < Release->CompatibleAppIdCount; ++i)
            delete[]Release->CompatibleAppIds[i];
        delete[]Release->CompatibleAppIds;

        for (uint32_t i = 0; i < Release->CompatiblePlatformCount; ++i)
            delete[]Release->CompatiblePlatforms[i];
        delete[]Release->CompatiblePlatforms;

        delete[]Release->ReleaseNote;
        delete Release;
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_QueryOwnershipBySandboxIds(EOS_HEcom Handle, const EOS_Ecom_QueryOwnershipBySandboxIdsOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOwnershipBySandboxIdsCallback CompletionDelegate) { return EOS_EResult::EOS_NotImplemented; }
    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetLastRedeemedEntitlementsCount(EOS_HEcom Handle, const EOS_Ecom_GetLastRedeemedEntitlementsCountOptions* Options) { return 0; }
    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyLastRedeemedEntitlementByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyLastRedeemedEntitlementByIndexOptions* Options, char** OutEntitlementId) { return EOS_EResult::EOS_NotImplemented; }
    EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetLastRedeemEntitlementsResultCount(EOS_HEcom Handle, const EOS_Ecom_GetLastRedeemEntitlementsResultCountOptions* Options) { return 0; }
    EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyLastRedeemEntitlementsResultByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyLastRedeemEntitlementsResultByIndexOptions* Options, EOS_Ecom_RedeemEntitlementsCallbackInfo** OutRedeemEntitlementsResult) { return EOS_EResult::EOS_NotImplemented; }

}
