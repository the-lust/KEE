#include "eossdk_auth.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"
#include "utils/jwt_utils.h"

namespace sdk
{

EOSSDK_Auth::EOSSDK_Auth():
    m_logged_in(false),
    m_access_expires(std::chrono::system_clock::now() + std::chrono::seconds(3600)),
    m_refresh_expires(std::chrono::system_clock::now() + std::chrono::seconds(7200))
{
    
    const string& acct_id  = Settings::Inst().userid_str;      
    const string& puid_str = Settings::Inst().productuserid_str;

    m_access_token  = jwt_utils::make_jwt(acct_id, puid_str, 0, 3600);
    m_refresh_token = jwt_utils::make_jwt(acct_id, puid_str, 0, 7200);

    GetCB_Manager().register_callbacks(this);
}

EOSSDK_Auth::~EOSSDK_Auth()
{
    GetCB_Manager().unregister_callbacks(this);
    GetCB_Manager().remove_all_notifications(this);
}

void EOSSDK_Auth::Login(const EOS_Auth_LoginOptions* Options, void* ClientData, const EOS_Auth_OnLoginCallback CompletionDelegate) {
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);
    if (!CompletionDelegate) return;

    // if no options, just fail it realy quick
    if (!Options) {
        dispatch_cb<EOS_Auth_LoginCallbackInfo>(this, CompletionDelegate, ClientData, EOS_EResult::EOS_InvalidParameters);
        return;
    }

    auto res = std::make_shared<FrameResult>();
    auto& lci = res->CreateCallback<EOS_Auth_LoginCallbackInfo>([this, ClientData, CompletionDelegate](void* param) {
        m_logged_in = true;
        
        // generate a new jwt for the user... realy simple bypass
        m_access_token = jwt_utils::make_jwt(Settings::Inst().account_id, Settings::Inst().product_user_id);
        m_access_expires = std::chrono::system_clock::now() + std::chrono::hours(1);
        m_refresh_token = "kee-refresh-token-" + Settings::Inst().userid_str;
        m_refresh_expires = std::chrono::system_clock::now() + std::chrono::hours(24);

        CompletionDelegate((EOS_Auth_LoginCallbackInfo*)param);

        // trigger notifications for login status change
        for (auto& n : GetCB_Manager().get_notifications(this, EOS_Auth_LoginStatusChangedCallbackInfo::k_iCallback)) {
            auto& lscci = n->GetCallback<EOS_Auth_LoginStatusChangedCallbackInfo>();
            lscci.LocalUserId = Settings::Inst().userid;
            lscci.PrevStatus = EOS_ELoginStatus::EOS_LS_NotLoggedIn;
            lscci.CurrentStatus = EOS_ELoginStatus::EOS_LS_LoggedIn;
            n->GetFunc()(n->GetFuncParam());
        }
    });

    // FIXME: do we need to check more things here? probably not for emu
    lci.ResultCode = EOS_EResult::EOS_Success;
    lci.LocalUserId = Settings::Inst().userid;
    lci.SelectedAccountId = Settings::Inst().userid;
    lci.ClientData = ClientData;
    res->done = true;
    GetCB_Manager().add_callback(this, res);
}



void EOSSDK_Auth::Logout(const EOS_Auth_LogoutOptions* Options, void* ClientData, const EOS_Auth_OnLogoutCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Auth_LogoutCallbackInfo, CompletionDelegate);
    info.LocalUserId = Settings::Inst().userid;

    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

void EOSSDK_Auth::LinkAccount(const EOS_Auth_LinkAccountOptions* Options, void* ClientData, const EOS_Auth_OnLinkAccountCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Auth_LinkAccountCallbackInfo, CompletionDelegate);
    info.LocalUserId = Settings::Inst().userid;
    info.PinGrantInfo = nullptr;

    if (Options == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_UnexpectedError;
    }

    SE_CB_END(this);
}

void EOSSDK_Auth::DeletePersistentAuth(const EOS_Auth_DeletePersistentAuthOptions* Options, void* ClientData, const EOS_Auth_OnDeletePersistentAuthCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Auth_DeletePersistentAuthCallbackInfo, CompletionDelegate);

    if (Options == nullptr || Options->RefreshToken == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

void EOSSDK_Auth::VerifyUserAuth(const EOS_Auth_VerifyUserAuthOptions* Options, void* ClientData, const EOS_Auth_OnVerifyUserAuthCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_Auth_VerifyUserAuthCallbackInfo, CompletionDelegate);
    if (Options == nullptr || Options->AuthToken == nullptr)
    {
        info.ResultCode = EOS_EResult::EOS_InvalidParameters;
    }
    else
    {
        info.ResultCode = EOS_EResult::EOS_Success;
    }

    SE_CB_END(this);
}

int32_t EOSSDK_Auth::GetLoggedInAccountsCount()
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return (m_logged_in ? 1 : 0);
}

EOS_EpicAccountId EOSSDK_Auth::GetLoggedInAccountByIndex(int32_t Index)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Index == 0)
        return Settings::Inst().userid;

    return nullptr;
}

EOS_ELoginStatus EOSSDK_Auth::GetLoginStatus(EOS_EpicAccountId LocalUserId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (LocalUserId == Settings::Inst().userid)
        return (m_logged_in ? EOS_ELoginStatus::EOS_LS_LoggedIn : EOS_ELoginStatus::EOS_LS_NotLoggedIn);

    return EOS_ELoginStatus::EOS_LS_NotLoggedIn;
}

EOS_EResult EOSSDK_Auth::CopyUserAuthTokenOld(EOS_EpicAccountId LocalUserId, EOS_Auth_Token** OutUserAuthToken)
{
    TRACE_FUNC();
    EOS_Auth_CopyUserAuthTokenOptions options;
    options.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_001;

    return CopyUserAuthToken(&options, LocalUserId, OutUserAuthToken);
}

EOS_EResult EOSSDK_Auth::CopyUserAuthToken(const EOS_Auth_CopyUserAuthTokenOptions* Options, EOS_EpicAccountId LocalUserId, EOS_Auth_Token** OutUserAuthToken)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Options->ApiVersion > EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST)
        return EOS_EResult::EOS_VersionMismatch;

    if (OutUserAuthToken == nullptr)
        return EOS_EResult::EOS_InvalidParameters;

    if (LocalUserId == Settings::Inst().userid)
    {
        auto chrono_now = std::chrono::system_clock::now();

        EOS_Auth_Token* token = new EOS_Auth_Token;
        time_t now;
        time(&now);
        
        token->ApiVersion = EOS_AUTH_TOKEN_API_LATEST;
        token->App = Settings::Inst().gamename.c_str();
        token->ClientId = GetEOS_Platform().m_client_id.c_str();
        token->AccountId = LocalUserId;
        
        {
            char* str = new char[m_access_token.length() + 1];
            strncpy(str, m_access_token.c_str(), m_access_token.length());
            token->AccessToken = str;
        }
        token->ExpiresIn = std::chrono::duration_cast<std::chrono::milliseconds>(m_access_expires - chrono_now).count() / 1000.0;
        {
            char* str = new char[64];
            now += token->ExpiresIn;
            strftime(str, 64, "%FT%T.000Z", gmtime(&now));
            now -= token->ExpiresIn;
            token->ExpiresAt = str;
        }

        token->AuthType = EOS_EAuthTokenType::EOS_ATT_User;

        {
            char* str = new char[m_refresh_token.length() + 1];
            strncpy(str, m_refresh_token.c_str(), m_refresh_token.length());
            token->RefreshToken = str;
        }
        token->RefreshExpiresIn = std::chrono::duration_cast<std::chrono::milliseconds>(m_refresh_expires - chrono_now).count() / 1000.0;
        {
            char* str = new char[64];
            now += token->RefreshExpiresIn;
            strftime(str, 64, "%FT%T.000Z", gmtime(&now));
            token->RefreshExpiresAt = str;
        }

        *OutUserAuthToken = token;
        return EOS_EResult::EOS_Success;
    }
    
    APP_LOG(Log::LogLevel::DEBUG, "Accountid not found: %p %s", LocalUserId, (LocalUserId == nullptr ? "" : LocalUserId->to_string().c_str()));
    return EOS_EResult::EOS_NotFound;
}

EOS_NotificationId EOSSDK_Auth::AddNotifyLoginStatusChangedOld(void* ClientData, const EOS_Auth_OnLoginStatusChangedCallback Notification)
{
    TRACE_FUNC();
    EOS_Auth_AddNotifyLoginStatusChangedOptions options;
    options.ApiVersion = EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_001;

    return AddNotifyLoginStatusChanged(&options, ClientData, Notification);
}

EOS_NotificationId EOSSDK_Auth::AddNotifyLoginStatusChanged(const EOS_Auth_AddNotifyLoginStatusChangedOptions* Options, void* ClientData, const EOS_Auth_OnLoginStatusChangedCallback Notification)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (Notification == nullptr)
        return EOS_INVALID_NOTIFICATIONID;

    pFrameResult_t res(new FrameResult);

    EOS_Auth_LoginStatusChangedCallbackInfo& lscci = res->CreateCallback<EOS_Auth_LoginStatusChangedCallbackInfo>((CallbackFunc)Notification);
    lscci.ClientData = ClientData;

    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_Auth::RemoveNotifyLoginStatusChanged(EOS_NotificationId InId)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    GetCB_Manager().remove_notification(this, InId);
}

bool EOSSDK_Auth::CBRunFrame()
{
    return false;
}

bool EOSSDK_Auth::RunNetwork(Network_Message_pb const& msg)
{
    return false;
}

bool EOSSDK_Auth::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;
}

void EOSSDK_Auth::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    {

    }
}

}