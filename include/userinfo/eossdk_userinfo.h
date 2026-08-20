#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"

namespace sdk
{
    class EOSSDK_UserInfo :
        public IRunCallback,
        public IRunNetwork
    {
        static constexpr auto userinfo_query_timeout = std::chrono::milliseconds(20000);

        std::unordered_map<EOS_EpicAccountId, UserInfo_Info_pb> m_userinfos;
        std::unordered_map<EOS_EpicAccountId, std::list<pFrameResult_t>> m_userinfos_queries;

    public:
        EOSSDK_UserInfo();
        ~EOSSDK_UserInfo();

        void setup_myself();
        UserInfo_Info_pb& get_myself();
        UserInfo_Info_pb* get_userinfo(EOS_EpicAccountId userid);

        bool send_userinfo_request(Network::peer_t const& peerid, UserInfo_Info_Request_pb* req);
        bool send_my_userinfo(Network::peer_t const& peerid);

        bool on_userinfo_request(Network_Message_pb const& msg, UserInfo_Info_Request_pb const& req);
        bool on_userinfo(Network_Message_pb const& msg, UserInfo_Info_pb const& infos);

        void QueryUserInfo(const EOS_UserInfo_QueryUserInfoOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoCallback CompletionDelegate);
        void QueryUserInfoByDisplayName(const EOS_UserInfo_QueryUserInfoByDisplayNameOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback CompletionDelegate);
        void QueryUserInfoByExternalAccount(const EOS_UserInfo_QueryUserInfoByExternalAccountOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoByExternalAccountCallback CompletionDelegate);
        EOS_EResult CopyUserInfo(const EOS_UserInfo_CopyUserInfoOptions* Options, EOS_UserInfo** OutUserInfo);
        uint32_t GetExternalUserInfoCount(const EOS_UserInfo_GetExternalUserInfoCountOptions* Options);
        EOS_EResult CopyExternalUserInfoByIndex(const EOS_UserInfo_CopyExternalUserInfoByIndexOptions* Options, EOS_UserInfo_ExternalUserInfo** OutExternalUserInfo);
        EOS_EResult CopyExternalUserInfoByAccountType(const EOS_UserInfo_CopyExternalUserInfoByAccountTypeOptions* Options, EOS_UserInfo_ExternalUserInfo** OutExternalUserInfo);
        EOS_EResult CopyExternalUserInfoByAccountId(const EOS_UserInfo_CopyExternalUserInfoByAccountIdOptions* Options, EOS_UserInfo_ExternalUserInfo** OutExternalUserInfo);

        virtual bool CBRunFrame();
        virtual bool RunNetwork(Network_Message_pb const& msg);
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);
    };
}
