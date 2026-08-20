

#pragma once
#include "common_includes.h"
#include "callback_manager.h"

namespace sdk
{
    class EOSSDK_Ecom;

    class EOSSDK_Ecom_Transaction
    {
        EOS_ProductUserId m_transaction_id;
        
    public:
        EOS_EResult GetTransactionId(char* OutBuffer, int32_t* InOutBufferLength);
        uint32_t    GetEntitlementsCount(const EOS_Ecom_Transaction_GetEntitlementsCountOptions*);
        EOS_EResult CopyEntitlementByIndex(const EOS_Ecom_Transaction_CopyEntitlementByIndexOptions*, EOS_Ecom_Entitlement**);
    };

    class EOSSDK_Ecom :
        public IRunCallback
    {
        string catalog_filename;
        string entitlements_filename;

        fifo_json m_catalog;
        fifo_json m_entitlements;

        std::map<string, fifo_json*> m_queried_entitlements;

        string m_last_transaction_id;

        EOS_EResult copy_entitlement(typename decltype(m_queried_entitlements)::iterator it, EOS_Ecom_Entitlement** OutEntitlement);

    public:
        EOSSDK_Ecom();
        ~EOSSDK_Ecom();

        virtual bool CBRunFrame();
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void        QueryOwnership(const EOS_Ecom_QueryOwnershipOptions*, void*, EOS_Ecom_OnQueryOwnershipCallback);
        void        QueryOwnershipToken(const EOS_Ecom_QueryOwnershipTokenOptions*, void*, EOS_Ecom_OnQueryOwnershipTokenCallback);
        void        QueryEntitlements(const EOS_Ecom_QueryEntitlementsOptions*, void*, EOS_Ecom_OnQueryEntitlementsCallback);
        void        QueryOffers(const EOS_Ecom_QueryOffersOptions*, void*, EOS_Ecom_OnQueryOffersCallback);
        void        Checkout(const EOS_Ecom_CheckoutOptions*, void*, EOS_Ecom_OnCheckoutCallback);
        void        RedeemEntitlements(const EOS_Ecom_RedeemEntitlementsOptions*, void*, EOS_Ecom_OnRedeemEntitlementsCallback);
        uint32_t    GetEntitlementsCount(const EOS_Ecom_GetEntitlementsCountOptions*);
        EOS_EResult CopyEntitlementByIndex(const EOS_Ecom_CopyEntitlementByIndexOptions*, EOS_Ecom_Entitlement**);
        EOS_EResult CopyEntitlementByNameAndIndex(const EOS_Ecom_CopyEntitlementByNameAndIndexOptions*, EOS_Ecom_Entitlement**);
        EOS_EResult CopyEntitlementById(const EOS_Ecom_CopyEntitlementByIdOptions*, EOS_Ecom_Entitlement**);
        uint32_t    GetEntitlementsByNameCount(const EOS_Ecom_GetEntitlementsByNameCountOptions*);
        uint32_t    GetOfferCount(const EOS_Ecom_GetOfferCountOptions*);
        EOS_EResult CopyOfferByIndex(const EOS_Ecom_CopyOfferByIndexOptions*, EOS_Ecom_CatalogOffer**);
        EOS_EResult CopyOfferById(const EOS_Ecom_CopyOfferByIdOptions*, EOS_Ecom_CatalogOffer**);
        uint32_t    GetOfferItemCount(const EOS_Ecom_GetOfferItemCountOptions*);
        EOS_EResult CopyOfferItemByIndex(const EOS_Ecom_CopyOfferItemByIndexOptions*, EOS_Ecom_CatalogItem**);
        EOS_EResult CopyItemById(const EOS_Ecom_CopyItemByIdOptions*, EOS_Ecom_CatalogItem**);
        uint32_t    GetOfferImageInfoCount(const EOS_Ecom_GetOfferImageInfoCountOptions*);
        EOS_EResult CopyOfferImageInfoByIndex(const EOS_Ecom_CopyOfferImageInfoByIndexOptions*, EOS_Ecom_KeyImageInfo**);
        uint32_t    GetItemImageInfoCount(const EOS_Ecom_GetItemImageInfoCountOptions*);
        EOS_EResult CopyItemImageInfoByIndex(const EOS_Ecom_CopyItemImageInfoByIndexOptions*, EOS_Ecom_KeyImageInfo**);
        uint32_t    GetItemReleaseCount(const EOS_Ecom_GetItemReleaseCountOptions*);
        EOS_EResult CopyItemReleaseByIndex(const EOS_Ecom_CopyItemReleaseByIndexOptions*, EOS_Ecom_CatalogRelease**);
        uint32_t    GetTransactionCount(const EOS_Ecom_GetTransactionCountOptions*);
        EOS_EResult CopyTransactionByIndex(const EOS_Ecom_CopyTransactionByIndexOptions*, EOS_Ecom_HTransaction*);
        EOS_EResult CopyTransactionById(const EOS_Ecom_CopyTransactionByIdOptions*, EOS_Ecom_HTransaction*);
        uint32_t    GetLastRedeemedEntitlementsCount(const EOS_Ecom_GetLastRedeemedEntitlementsCountOptions*);
        EOS_EResult CopyLastRedeemedEntitlementByIndex(const EOS_Ecom_CopyLastRedeemedEntitlementByIndexOptions*, char**, int32_t*);
    };
}
