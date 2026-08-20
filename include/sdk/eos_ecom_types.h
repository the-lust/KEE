 

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_EcomHandle* EOS_HEcom;

EOS_EXTERN_C typedef struct EOS_Ecom_TransactionHandle* EOS_Ecom_HTransaction;

EOS_EXTERN_C typedef const char* EOS_Ecom_CatalogItemId;

EOS_EXTERN_C typedef const char* EOS_Ecom_CatalogOfferId;

EOS_EXTERN_C typedef const char* EOS_Ecom_EntitlementName;

EOS_EXTERN_C typedef const char* EOS_Ecom_EntitlementId;

EOS_EXTERN_C typedef const char* EOS_Ecom_SandboxId;

EOS_ENUM(EOS_EOwnershipStatus,
	
	EOS_OS_NotOwned = 0,
	
	EOS_OS_Owned = 1
);

EOS_ENUM(EOS_EEcomItemType,
	
	EOS_EIT_Durable = 0,
	
	EOS_EIT_Consumable = 1,
	
	EOS_EIT_Other = 2
);

EOS_ENUM(EOS_ECheckoutOrientation,
	
	EOS_ECO_Default = 0,
	
	EOS_ECO_Portrait = 1,
	
	EOS_ECO_Landscape = 2
);

#define EOS_ECOM_ENTITLEMENT_API_LATEST 2

#define EOS_ECOM_ENTITLEMENT_ENDTIMESTAMP_UNDEFINED -1

EOS_STRUCT(EOS_Ecom_Entitlement, (
	
	int32_t ApiVersion;
	
	EOS_Ecom_EntitlementName EntitlementName;
	
	EOS_Ecom_EntitlementId EntitlementId;
	
	EOS_Ecom_CatalogItemId CatalogItemId;
	
	int32_t ServerIndex;
	
	EOS_Bool bRedeemed;
	
	int64_t EndTimestamp;
));

EOS_DECLARE_FUNC(void) EOS_Ecom_Entitlement_Release(EOS_Ecom_Entitlement* Entitlement);

#define EOS_ECOM_ITEMOWNERSHIP_API_LATEST 1

EOS_STRUCT(EOS_Ecom_ItemOwnership, (
	
	int32_t ApiVersion;
	
	EOS_Ecom_CatalogItemId Id;
	
	EOS_EOwnershipStatus OwnershipStatus;
));

#define EOS_ECOM_CATALOGITEM_API_LATEST 1

#define EOS_ECOM_CATALOGITEM_ENTITLEMENTENDTIMESTAMP_UNDEFINED -1

#define EOS_ECOM_CATALOGITEMID_MAX_LENGTH 32

EOS_STRUCT(EOS_Ecom_CatalogItem, (
	
	int32_t ApiVersion;
	
	const char* CatalogNamespace;
	
	EOS_Ecom_CatalogItemId Id;
	
	EOS_Ecom_EntitlementName EntitlementName;
	
	const char* TitleText;
	
	const char* DescriptionText;
	
	const char* LongDescriptionText;
	
	const char* TechnicalDetailsText;
	
	const char* DeveloperText;
	
	EOS_EEcomItemType ItemType;
	
	int64_t EntitlementEndTimestamp;
));

EOS_DECLARE_FUNC(void) EOS_Ecom_CatalogItem_Release(EOS_Ecom_CatalogItem* CatalogItem);

#define EOS_ECOM_CATALOGOFFER_API_LATEST 5

#define EOS_ECOM_CATALOGOFFER_EXPIRATIONTIMESTAMP_UNDEFINED -1

#define EOS_ECOM_CATALOGOFFER_RELEASEDATETIMESTAMP_UNDEFINED -1

#define EOS_ECOM_CATALOGOFFER_EFFECTIVEDATETIMESTAMP_UNDEFINED -1

#define EOS_ECOM_CATALOGOFFERID_MAX_LENGTH 32

EOS_STRUCT(EOS_Ecom_CatalogOffer, (
	
	int32_t ApiVersion;
	
	int32_t ServerIndex;
	
	const char* CatalogNamespace;
	
	EOS_Ecom_CatalogOfferId Id;
	
	const char* TitleText;
	
	const char* DescriptionText;
	
	const char* LongDescriptionText;
	
	const char* TechnicalDetailsText_DEPRECATED;
	
	const char* CurrencyCode;
	
	EOS_EResult PriceResult;
	
	uint32_t OriginalPrice_DEPRECATED;
	
	uint32_t CurrentPrice_DEPRECATED;
	
	uint8_t DiscountPercentage;
	
	int64_t ExpirationTimestamp;
	
	uint32_t PurchasedCount_DEPRECATED;
	
	int32_t PurchaseLimit;
	
	EOS_Bool bAvailableForPurchase;
	
	uint64_t OriginalPrice64;
	
	uint64_t CurrentPrice64;
	
	uint32_t DecimalPoint;
	
	int64_t ReleaseDateTimestamp;
	
	int64_t EffectiveDateTimestamp;
));

EOS_DECLARE_FUNC(void) EOS_Ecom_CatalogOffer_Release(EOS_Ecom_CatalogOffer* CatalogOffer);

#define EOS_ECOM_KEYIMAGEINFO_API_LATEST 1

EOS_STRUCT(EOS_Ecom_KeyImageInfo, (
	
	int32_t ApiVersion;
	
	const char* Type;
	
	const char* Url;
	
	uint32_t Width;
	
	uint32_t Height;
));

EOS_DECLARE_FUNC(void) EOS_Ecom_KeyImageInfo_Release(EOS_Ecom_KeyImageInfo* KeyImageInfo);

#define EOS_ECOM_CATALOGRELEASE_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CatalogRelease, (
	
	int32_t ApiVersion;
	
	uint32_t CompatibleAppIdCount;
	
	const char** CompatibleAppIds;
	
	uint32_t CompatiblePlatformCount;
	
	const char** CompatiblePlatforms;
	
	const char* ReleaseNote;
));

EOS_DECLARE_FUNC(void) EOS_Ecom_CatalogRelease_Release(EOS_Ecom_CatalogRelease* CatalogRelease);

#define EOS_ECOM_CHECKOUTENTRY_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CheckoutEntry, (
	
	int32_t ApiVersion;
	
	EOS_Ecom_CatalogOfferId OfferId;
));

#define EOS_ECOM_QUERYOWNERSHIP_API_LATEST 2

#define EOS_ECOM_QUERYOWNERSHIP_MAX_CATALOG_IDS 400

EOS_STRUCT(EOS_Ecom_QueryOwnershipOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogItemId* CatalogItemIds;
	
	uint32_t CatalogItemIdCount;
	
	const char* CatalogNamespace;
));

EOS_STRUCT(EOS_Ecom_QueryOwnershipCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	const EOS_Ecom_ItemOwnership* ItemOwnership;
	
	uint32_t ItemOwnershipCount;
));

EOS_DECLARE_CALLBACK(EOS_Ecom_OnQueryOwnershipCallback, const EOS_Ecom_QueryOwnershipCallbackInfo* Data);

#define EOS_ECOM_QUERYOWNERSHIPBYSANDBOXIDSOPTIONS_API_LATEST 1

#define EOS_ECOM_QUERYOWNERSHIP_MAX_SANDBOX_IDS 10

EOS_STRUCT(EOS_Ecom_QueryOwnershipBySandboxIdsOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_SandboxId* SandboxIds;
	
	uint32_t SandboxIdsCount;
));

EOS_STRUCT(EOS_Ecom_SandboxIdItemOwnership, (
	
	EOS_Ecom_SandboxId SandboxId;
	
	const EOS_Ecom_CatalogItemId* OwnedCatalogItemIds;
	
	uint32_t OwnedCatalogItemIdsCount;
));

EOS_STRUCT(EOS_Ecom_QueryOwnershipBySandboxIdsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	const EOS_Ecom_SandboxIdItemOwnership* SandboxIdItemOwnerships;
	
	uint32_t SandboxIdItemOwnershipsCount;
));

EOS_DECLARE_CALLBACK(EOS_Ecom_OnQueryOwnershipBySandboxIdsCallback, const EOS_Ecom_QueryOwnershipBySandboxIdsCallbackInfo* Data);

#define EOS_ECOM_QUERYOWNERSHIPTOKEN_API_LATEST 2

#define EOS_ECOM_QUERYOWNERSHIPTOKEN_MAX_CATALOGITEM_IDS 32

EOS_STRUCT(EOS_Ecom_QueryOwnershipTokenOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogItemId* CatalogItemIds;
	
	uint32_t CatalogItemIdCount;
	
	const char* CatalogNamespace;
));

EOS_STRUCT(EOS_Ecom_QueryOwnershipTokenCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* OwnershipToken;
));

EOS_DECLARE_CALLBACK(EOS_Ecom_OnQueryOwnershipTokenCallback, const EOS_Ecom_QueryOwnershipTokenCallbackInfo* Data);

#define EOS_ECOM_QUERYENTITLEMENTS_API_LATEST 3

#define EOS_ECOM_QUERYENTITLEMENTS_MAX_ENTITLEMENT_IDS 256

EOS_STRUCT(EOS_Ecom_QueryEntitlementsOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_EntitlementName* EntitlementNames;
	
	uint32_t EntitlementNameCount;
	
	EOS_Bool bIncludeRedeemed;
	
	const char* OverrideCatalogNamespace;
));

EOS_STRUCT(EOS_Ecom_QueryEntitlementsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Ecom_OnQueryEntitlementsCallback, const EOS_Ecom_QueryEntitlementsCallbackInfo* Data);

#define EOS_ECOM_QUERYENTITLEMENTTOKEN_API_LATEST 1

#define EOS_ECOM_QUERYENTITLEMENTTOKEN_MAX_ENTITLEMENT_IDS 32

EOS_STRUCT(EOS_Ecom_QueryEntitlementTokenOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_EntitlementName* EntitlementNames;
	
	uint32_t EntitlementNameCount;
));

EOS_STRUCT(EOS_Ecom_QueryEntitlementTokenCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* EntitlementToken;
));

EOS_DECLARE_CALLBACK(EOS_Ecom_OnQueryEntitlementTokenCallback, const EOS_Ecom_QueryEntitlementTokenCallbackInfo* Data);

#define EOS_ECOM_QUERYOFFERS_API_LATEST 1

EOS_STRUCT(EOS_Ecom_QueryOffersOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* OverrideCatalogNamespace;
));

EOS_STRUCT(EOS_Ecom_QueryOffersCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_Ecom_OnQueryOffersCallback, const EOS_Ecom_QueryOffersCallbackInfo* Data);

#define EOS_ECOM_CHECKOUT_API_LATEST 2

#define EOS_ECOM_CHECKOUT_MAX_ENTRIES 10

#define EOS_ECOM_TRANSACTIONID_MAXIMUM_LENGTH 64

#define EOS_ECOM_TRANSACTIONID_MAX_LENGTH 64

EOS_STRUCT(EOS_Ecom_CheckoutOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* OverrideCatalogNamespace;
	
	uint32_t EntryCount;
	
	const EOS_Ecom_CheckoutEntry* Entries;
	
	EOS_ECheckoutOrientation PreferredOrientation;
));

EOS_STRUCT(EOS_Ecom_CheckoutCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* TransactionId;
));

EOS_DECLARE_CALLBACK(EOS_Ecom_OnCheckoutCallback, const EOS_Ecom_CheckoutCallbackInfo* Data);

#define EOS_ECOM_REDEEMENTITLEMENTS_API_LATEST 2

#define EOS_ECOM_REDEEMENTITLEMENTS_MAX_IDS 32

#define EOS_ECOM_ENTITLEMENTID_MAX_LENGTH 32

EOS_STRUCT(EOS_Ecom_RedeemEntitlementsOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	uint32_t EntitlementIdCount;
	
	EOS_Ecom_EntitlementId* EntitlementIds;
));

EOS_STRUCT(EOS_Ecom_RedeemEntitlementsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	uint32_t RedeemedEntitlementIdsCount;
	
	uint32_t PreviouslyRedeemedEntitlementIdsCount;
	
	uint32_t InvalidEntitlementIdsCount;
));

EOS_DECLARE_CALLBACK(EOS_Ecom_OnRedeemEntitlementsCallback, const EOS_Ecom_RedeemEntitlementsCallbackInfo* Data);

#define EOS_ECOM_GETLASTREDEEMEDENTITLEMENTSCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetLastRedeemedEntitlementsCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

#define EOS_ECOM_COPYLASTREDEEMEDENTITLEMENTBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyLastRedeemedEntitlementByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	uint32_t RedeemedEntitlementIndex;
));

EOS_ENUM(EOS_ERedeemEntitlementsResultListType,
	
	EOS_ERERLT_Redeemed = 0,
	
	EOS_ERERLT_PreviouslyRedeemed = 1,
	
	EOS_ERERLT_Invalid = 2
);

#define EOS_ECOM_GETLASTREDEEMENTITLEMENTSRESULTCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetLastRedeemEntitlementsResultCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_ERedeemEntitlementsResultListType ResultType;
));

#define EOS_ECOM_COPYLASTREDEEMENTITLEMENTSRESULTBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyLastRedeemEntitlementsResultByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	uint32_t EntitlementIndex;
	
	EOS_ERedeemEntitlementsResultListType ResultType;
));

#define EOS_ECOM_GETENTITLEMENTSCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetEntitlementsCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

#define EOS_ECOM_GETENTITLEMENTSBYNAMECOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetEntitlementsByNameCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_EntitlementName EntitlementName;
));

#define EOS_ECOM_COPYENTITLEMENTBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyEntitlementByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	uint32_t EntitlementIndex;
));

#define EOS_ECOM_COPYENTITLEMENTBYNAMEANDINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyEntitlementByNameAndIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_EntitlementName EntitlementName;
	
	uint32_t Index;
));

#define EOS_ECOM_COPYENTITLEMENTBYID_API_LATEST 2

EOS_STRUCT(EOS_Ecom_CopyEntitlementByIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_EntitlementId EntitlementId;
));

#define EOS_ECOM_GETOFFERCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetOfferCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

#define EOS_ECOM_COPYOFFERBYINDEX_API_LATEST 3

EOS_STRUCT(EOS_Ecom_CopyOfferByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	uint32_t OfferIndex;
));

#define EOS_ECOM_COPYOFFERBYID_API_LATEST 3

EOS_STRUCT(EOS_Ecom_CopyOfferByIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogOfferId OfferId;
));

#define EOS_ECOM_GETOFFERITEMCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetOfferItemCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogOfferId OfferId;
));

#define EOS_ECOM_COPYOFFERITEMBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyOfferItemByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogOfferId OfferId;
	
	uint32_t ItemIndex;
));

#define EOS_ECOM_COPYITEMBYID_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyItemByIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogItemId ItemId;
));

#define EOS_ECOM_GETOFFERIMAGEINFOCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetOfferImageInfoCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogOfferId OfferId;
));

#define EOS_ECOM_COPYOFFERIMAGEINFOBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyOfferImageInfoByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogOfferId OfferId;
	
	uint32_t ImageInfoIndex;
));

#define EOS_ECOM_GETITEMIMAGEINFOCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetItemImageInfoCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogItemId ItemId;
));

#define EOS_ECOM_COPYITEMIMAGEINFOBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyItemImageInfoByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogItemId ItemId;
	
	uint32_t ImageInfoIndex;
));

#define EOS_ECOM_GETITEMRELEASECOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetItemReleaseCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogItemId ItemId;
));

#define EOS_ECOM_COPYITEMRELEASEBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyItemReleaseByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_Ecom_CatalogItemId ItemId;
	
	uint32_t ReleaseIndex;
));

#define EOS_ECOM_GETTRANSACTIONCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_GetTransactionCountOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

#define EOS_ECOM_COPYTRANSACTIONBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyTransactionByIndexOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	uint32_t TransactionIndex;
));

#define EOS_ECOM_COPYTRANSACTIONBYID_API_LATEST 1

EOS_STRUCT(EOS_Ecom_CopyTransactionByIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	const char* TransactionId;
));

#define EOS_ECOM_TRANSACTION_GETENTITLEMENTSCOUNT_API_LATEST 1

EOS_STRUCT(EOS_Ecom_Transaction_GetEntitlementsCountOptions, (
	
	int32_t ApiVersion;
));

#define EOS_ECOM_TRANSACTION_COPYENTITLEMENTBYINDEX_API_LATEST 1

EOS_STRUCT(EOS_Ecom_Transaction_CopyEntitlementByIndexOptions, (
	
	int32_t ApiVersion;
	
	uint32_t EntitlementIndex;
));

EOS_DECLARE_FUNC(void) EOS_Ecom_Transaction_Release(EOS_Ecom_HTransaction Transaction);

#pragma pack(pop)
