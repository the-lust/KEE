#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"

namespace sdk
{
    class EOSSDK_Auth :
        public IRunCallback,
        public IRunNetwork
    {
        bool m_logged_in;
        string m_access_token;
        std::chrono::system_clock::time_point m_access_expires;
        string m_refresh_token;
        std::chrono::system_clock::time_point m_refresh_expires;

    public:
        EOSSDK_Auth();
        ~EOSSDK_Auth();

        virtual bool CBRunFrame();
        virtual bool RunNetwork(Network_Message_pb const& msg);
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void Login(const EOS_Auth_LoginOptions* Options, void* ClientData, const EOS_Auth_OnLoginCallback CompletionDelegate);
        void Logout(const EOS_Auth_LogoutOptions* Options, void* ClientData, const EOS_Auth_OnLogoutCallback CompletionDelegate);
        void LinkAccount(const EOS_Auth_LinkAccountOptions* Options, void* ClientData, const EOS_Auth_OnLinkAccountCallback CompletionDelegate);
        void DeletePersistentAuth(const EOS_Auth_DeletePersistentAuthOptions* Options, void* ClientData, const EOS_Auth_OnDeletePersistentAuthCallback CompletionDelegate);
        void VerifyUserAuth(const EOS_Auth_VerifyUserAuthOptions* Options, void* ClientData, const EOS_Auth_OnVerifyUserAuthCallback CompletionDelegate);
        int32_t GetLoggedInAccountsCount();
        EOS_EpicAccountId GetLoggedInAccountByIndex(int32_t Index);
        EOS_ELoginStatus GetLoginStatus(EOS_EpicAccountId LocalUserId);
        EOS_EResult CopyUserAuthTokenOld(EOS_EpicAccountId LocalUserId, EOS_Auth_Token** OutUserAuthToken);
        EOS_EResult CopyUserAuthToken(const EOS_Auth_CopyUserAuthTokenOptions* Options, EOS_EpicAccountId LocalUserId, EOS_Auth_Token** OutUserAuthToken);
        EOS_NotificationId AddNotifyLoginStatusChangedOld(void* ClientData, const EOS_Auth_OnLoginStatusChangedCallback Notification);
        EOS_NotificationId AddNotifyLoginStatusChanged(const EOS_Auth_AddNotifyLoginStatusChangedOptions* Options, void* ClientData, const EOS_Auth_OnLoginStatusChangedCallback Notification);
        void RemoveNotifyLoginStatusChanged(EOS_NotificationId InId);
    };
}