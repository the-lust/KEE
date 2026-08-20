#include "auth/eossdk_auth.h"
#include "eossdk_platform.h"
#include "helper_funcs.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(void) EOS_Auth_Login(EOS_HAuth Handle, const EOS_Auth_LoginOptions* Options, void* ClientData, const EOS_Auth_OnLoginCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        pInst->Login(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_Logout(EOS_HAuth Handle, const EOS_Auth_LogoutOptions* Options, void* ClientData, const EOS_Auth_OnLogoutCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        pInst->Logout(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_LinkAccount(EOS_HAuth Handle, const EOS_Auth_LinkAccountOptions* Options, void* ClientData, const EOS_Auth_OnLinkAccountCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        pInst->LinkAccount(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_DeletePersistentAuth(EOS_HAuth Handle, const EOS_Auth_DeletePersistentAuthOptions* Options, void* ClientData, const EOS_Auth_OnDeletePersistentAuthCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        pInst->DeletePersistentAuth(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_VerifyUserAuth(EOS_HAuth Handle, const EOS_Auth_VerifyUserAuthOptions* Options, void* ClientData, const EOS_Auth_OnVerifyUserAuthCallback CompletionDelegate)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        pInst->VerifyUserAuth(Options, ClientData, CompletionDelegate);
    }

    EOS_DECLARE_FUNC(int32_t) EOS_Auth_GetLoggedInAccountsCount(EOS_HAuth Handle)
    {
        if (Handle == nullptr)
            return 0;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        return pInst->GetLoggedInAccountsCount();
    }

    EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_Auth_GetLoggedInAccountByIndex(EOS_HAuth Handle, int32_t Index)
    {
        if (Handle == nullptr)
            return nullptr;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        return pInst->GetLoggedInAccountByIndex(Index);
    }

    EOS_DECLARE_FUNC(EOS_ELoginStatus) EOS_Auth_GetLoginStatus(EOS_HAuth Handle, EOS_EpicAccountId LocalUserId)
    {
        if (Handle == nullptr)
            return EOS_ELoginStatus::EOS_LS_NotLoggedIn;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        return pInst->GetLoginStatus(LocalUserId);
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_RemoveNotifyLoginStatusChanged(EOS_HAuth Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_Auth*>(Handle);
        pInst->RemoveNotifyLoginStatusChanged(InId);
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_Token_Release(EOS_Auth_Token* AuthToken)
    {
        if (AuthToken == nullptr)
            return;

        delete[] AuthToken->AccessToken;
        delete[] AuthToken->RefreshToken;
        delete[] AuthToken->ExpiresAt;
        delete[] AuthToken->RefreshExpiresAt;
        delete AuthToken;
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Auth_CopyIdToken(EOS_HAuth Handle, const EOS_Auth_CopyIdTokenOptions* Options, EOS_Auth_IdToken ** OutIdToken)
    {
        if (Options == nullptr || OutIdToken == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        if (Options->AccountId == nullptr)
            return EOS_EResult::EOS_InvalidUser;

        EOS_Auth_IdToken* token = new EOS_Auth_IdToken;
        token->ApiVersion = EOS_AUTH_IDTOKEN_API_LATEST;
        token->AccountId = Options->AccountId;

        string jwt = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.e30.dummy";
        char* str = new char[jwt.length() + 1];
        strncpy(str, jwt.c_str(), jwt.length() + 1);
        token->JsonWebToken = str;

        *OutIdToken = token;
        return EOS_EResult::EOS_Success;
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_QueryIdToken(EOS_HAuth Handle, const EOS_Auth_QueryIdTokenOptions* Options, void* ClientData, const EOS_Auth_OnQueryIdTokenCallback CompletionDelegate)
    {
        if(CompletionDelegate) {
            EOS_Auth_QueryIdTokenCallbackInfo info{};
            info.ResultCode     = EOS_EResult::EOS_Success;
            info.ClientData     = ClientData;
            info.LocalUserId    = Options ? Options->LocalUserId : nullptr;
            info.TargetAccountId= Options ? Options->LocalUserId : nullptr;
            CompletionDelegate(&info);
        }
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_VerifyIdToken(EOS_HAuth Handle, const EOS_Auth_VerifyIdTokenOptions* Options, void* ClientData, const EOS_Auth_OnVerifyIdTokenCallback CompletionDelegate)
    {
        if(CompletionDelegate) {
            
            EOS_Auth_VerifyIdTokenCallbackInfo info{};
            info.ResultCode                  = EOS_EResult::EOS_Success;
            info.ClientData                  = ClientData;
            info.ApplicationId               = "";
            info.ClientId                    = "";
            info.ProductId                   = "";
            info.SandboxId                   = "";
            info.DeploymentId                = "";
            info.DisplayName                 = "";
            info.bIsExternalAccountInfoPresent = EOS_FALSE;
            info.ExternalAccountIdType       = EOS_EExternalAccountType::EOS_EAT_EPIC;
            info.ExternalAccountId           = "";
            info.ExternalAccountDisplayName  = "";
            info.Platform                    = "";
            CompletionDelegate(&info);
        }
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_Auth_GetSelectedAccountId(EOS_HAuth Handle, const EOS_EpicAccountId LocalUserId, EOS_EpicAccountId* OutSelectedAccountId)
    {
        if(OutSelectedAccountId) *OutSelectedAccountId = LocalUserId;
        return EOS_EResult::EOS_Success;
    }

    EOS_DECLARE_FUNC(uint32_t) EOS_Auth_GetMergedAccountsCount(EOS_HAuth Handle, const EOS_EpicAccountId LocalUserId)
    {
        return 0;
    }

    EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_Auth_GetMergedAccountByIndex(EOS_HAuth Handle, const EOS_EpicAccountId LocalUserId, const uint32_t Index)
    {
        return nullptr;
    }

    EOS_DECLARE_FUNC(void) EOS_Auth_IdToken_Release(EOS_Auth_IdToken* IdToken)
    {
        if(IdToken) {
            delete[] IdToken->JsonWebToken;
            delete IdToken;
        }
    }
}
