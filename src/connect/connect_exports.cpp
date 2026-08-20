#include "connect/eossdk_connect.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Connect_Login(EOS_HConnect Handle, const EOS_Connect_LoginOptions* Options, void* ClientData, const EOS_Connect_OnLoginCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->Login(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_CreateUser(EOS_HConnect Handle, const EOS_Connect_CreateUserOptions* Options, void* ClientData, const EOS_Connect_OnCreateUserCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->CreateUser(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_LinkAccount(EOS_HConnect Handle, const EOS_Connect_LinkAccountOptions* Options, void* ClientData, const EOS_Connect_OnLinkAccountCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->LinkAccount(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_UnlinkAccount(EOS_HConnect Handle, const EOS_Connect_UnlinkAccountOptions* Options, void* ClientData, const EOS_Connect_OnUnlinkAccountCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->UnlinkAccount(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_CreateDeviceId(EOS_HConnect Handle, const EOS_Connect_CreateDeviceIdOptions* Options, void* ClientData, const EOS_Connect_OnCreateDeviceIdCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->CreateDeviceId(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_DeleteDeviceId(EOS_HConnect Handle, const EOS_Connect_DeleteDeviceIdOptions* Options, void* ClientData, const EOS_Connect_OnDeleteDeviceIdCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->DeleteDeviceId(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_TransferDeviceIdAccount(EOS_HConnect Handle, const EOS_Connect_TransferDeviceIdAccountOptions* Options, void* ClientData, const EOS_Connect_OnTransferDeviceIdAccountCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->TransferDeviceIdAccount(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_QueryExternalAccountMappings(EOS_HConnect Handle, const EOS_Connect_QueryExternalAccountMappingsOptions* Options, void* ClientData, const EOS_Connect_OnQueryExternalAccountMappingsCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->QueryExternalAccountMappings(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_QueryProductUserIdMappings(EOS_HConnect Handle, const EOS_Connect_QueryProductUserIdMappingsOptions* Options, void* ClientData, const EOS_Connect_OnQueryProductUserIdMappingsCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->QueryProductUserIdMappings(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(EOS_ProductUserId) EOS_Connect_GetExternalAccountMapping(EOS_HConnect Handle, const EOS_Connect_GetExternalAccountMappingsOptions* Options)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->GetExternalAccountMapping(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_GetProductUserIdMapping(EOS_HConnect Handle, const EOS_Connect_GetProductUserIdMappingOptions* Options, char* OutBuffer, int32_t* InOutBufferLength)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->GetProductUserIdMapping(Options, OutBuffer, InOutBufferLength);
    }

    EOS_DECLARE_FUNC(int32_t) EOS_Connect_GetLoggedInUsersCount(EOS_HConnect Handle)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->GetLoggedInUsersCount();
    }

    EOS_DECLARE_FUNC(EOS_ProductUserId) EOS_Connect_GetLoggedInUserByIndex(EOS_HConnect Handle, int32_t Index)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->GetLoggedInUserByIndex(Index);
    }

    EOS_DECLARE_FUNC(EOS_ELoginStatus) EOS_Connect_GetLoginStatus(EOS_HConnect Handle, EOS_ProductUserId LocalUserId)
    {
        if (Handle == nullptr)
            return EOS_ELoginStatus::EOS_LS_NotLoggedIn;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->GetLoginStatus(LocalUserId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Connect_AddNotifyAuthExpiration(EOS_HConnect Handle, const EOS_Connect_AddNotifyAuthExpirationOptions* Options, void* ClientData, const EOS_Connect_OnAuthExpirationCallback Notification)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->AddNotifyAuthExpiration(Options, ClientData, Notification);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_RemoveNotifyAuthExpiration(EOS_HConnect Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->RemoveNotifyAuthExpiration(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Connect_AddNotifyLoginStatusChanged(EOS_HConnect Handle, const EOS_Connect_AddNotifyLoginStatusChangedOptions* Options, void* ClientData, const EOS_Connect_OnLoginStatusChangedCallback Notification)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->AddNotifyLoginStatusChanged(Options, ClientData, Notification);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_RemoveNotifyLoginStatusChanged(EOS_HConnect Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        pInst->RemoveNotifyLoginStatusChanged(InId);
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Connect_GetProductUserExternalAccountCount(EOS_HConnect Handle, const EOS_Connect_GetProductUserExternalAccountCountOptions * Options)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->GetProductUserExternalAccountCount(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_CopyProductUserExternalAccountByIndex(EOS_HConnect Handle, const EOS_Connect_CopyProductUserExternalAccountByIndexOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->CopyProductUserExternalAccountByIndex(Options, OutExternalAccountInfo);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_CopyProductUserExternalAccountByAccountType(EOS_HConnect Handle, const EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->CopyProductUserExternalAccountByAccountType(Options, OutExternalAccountInfo);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_CopyProductUserExternalAccountByAccountId(EOS_HConnect Handle, const EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->CopyProductUserExternalAccountByAccountId(Options, OutExternalAccountInfo);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_CopyProductUserInfo(EOS_HConnect Handle, const EOS_Connect_CopyProductUserInfoOptions* Options, EOS_Connect_ExternalAccountInfo** OutExternalAccountInfo)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->CopyProductUserInfo(Options, OutExternalAccountInfo);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_ExternalAccountInfo_Release(EOS_Connect_ExternalAccountInfo* ExternalAccountInfo)
    {
        if (ExternalAccountInfo != nullptr)
        {
            delete ExternalAccountInfo;
        }
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_Logout(EOS_HConnect Handle, const EOS_Connect_LogoutOptions* Options, void* ClientData, const EOS_Connect_OnLogoutCallback CompletionDelegate)
    {
        if (CompletionDelegate) {
            EOS_Connect_LogoutCallbackInfo info;
            info.ResultCode = EOS_EResult::EOS_NotImplemented;
            info.ClientData = ClientData;
            info.LocalUserId = Options ? Options->LocalUserId : nullptr;
            CompletionDelegate(&info);
        }
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_CopyIdToken(EOS_HConnect Handle, const EOS_Connect_CopyIdTokenOptions* Options, EOS_Connect_IdToken ** OutIdToken)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_Connect*>(Handle);
        return pInst->CopyIdToken(Options, OutIdToken);
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_VerifyIdToken(EOS_HConnect Handle, const EOS_Connect_VerifyIdTokenOptions* Options, void* ClientData, const EOS_Connect_OnVerifyIdTokenCallback CompletionDelegate)
    {
        if (CompletionDelegate) {
            EOS_Connect_VerifyIdTokenCallbackInfo info;
            info.ResultCode = EOS_EResult::EOS_NotImplemented;
            info.ClientData = ClientData;
            CompletionDelegate(&info);
        }
    }

    EOS_DECLARE_FUNC(void) EOS_Connect_IdToken_Release(EOS_Connect_IdToken* IdToken)
    {
        if (IdToken != nullptr)
        {
            delete IdToken;
        }
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Connect_AddNotifyClientAuthTokenGenerated(EOS_HConnect Handle, const void* Options, void* ClientData, const void* NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Connect_RemoveNotifyClientAuthTokenGenerated(EOS_HConnect Handle, EOS_NotificationId InId) {}

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Connect_AddNotifyExchangeTokenStateChanged(EOS_HConnect Handle, const void* Options, void* ClientData, const void* NotificationFn)
    { return static_cast<EOS_NotificationId>(1); }

    EOS_DECLARE_FUNC(void) EOS_Connect_RemoveNotifyExchangeTokenStateChanged(EOS_HConnect Handle, EOS_NotificationId InId) {}

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_CopyExchangeToken(EOS_HConnect Handle, const void* Options, void** OutExchangeToken)
    { if (OutExchangeToken) *OutExchangeToken = nullptr; return EOS_EResult::EOS_NotFound; }

    EOS_DECLARE_FUNC(void) EOS_Connect_ExchangeToken_Release(void* ExchangeToken) { }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_GetClientAuthTokenAsString(EOS_HConnect Handle, const void* Options, char* OutBuffer, int32_t* InOutBufferLength)
    { if (InOutBufferLength) *InOutBufferLength = 0; return EOS_EResult::EOS_NotImplemented; }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_GetUserAuthTokenAsString(EOS_HConnect Handle, const void* Options, char* OutBuffer, int32_t* InOutBufferLength)
    { if (InOutBufferLength) *InOutBufferLength = 0; return EOS_EResult::EOS_NotImplemented; }

    EOS_DECLARE_FUNC(void) EOS_Connect_ExecuteWithOverriddenTokens(EOS_HConnect Handle, const void* Options)
    {}

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Connect_RemoveExchangeToken(EOS_HConnect Handle, const void* Options)
    { return EOS_EResult::EOS_Success; }

    EOS_DECLARE_FUNC(void) EOS_Connect_RequestExchangeToken(EOS_HConnect Handle, const void* Options, void* ClientData, const void* CompletionDelegate)
    { /* stub */ }
}
