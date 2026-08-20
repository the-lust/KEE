#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"

namespace sdk
{
    struct user_state_t
    {
        bool connected;    
        bool authentified; 
        time_pt last_infos;
        Connect_Infos_pb infos;
    };

    class EOSSDK_Connect :
        public IRunCallback,
        public IRunNetwork
    {
        static constexpr std::chrono::milliseconds user_infos_rate = std::chrono::milliseconds(3000);

        string m_device_id;
        string m_device_id_filename; 

    public:
        string m_username; 

        nlohmann::fifo_map<EOS_ProductUserId, user_state_t> m_users;

        EOSSDK_Connect();
        ~EOSSDK_Connect();

        inline typename decltype(EOSSDK_Connect::m_users)::iterator get_myself()
        {
            return m_users.begin();
        }
        inline typename decltype(EOSSDK_Connect::m_users)::iterator get_all_users()
        {
            return m_users.begin();
        }
        inline typename decltype(EOSSDK_Connect::m_users)::iterator get_other_users()
        {
            return ++(m_users.begin());
        }
        inline typename decltype(EOSSDK_Connect::m_users)::iterator get_user_by_userid(EOS_EpicAccountId userid)
        {
            return std::find_if(m_users.begin(), m_users.end(), [&userid](std::pair<EOS_ProductUserId const, user_state_t>& user)
            {
                return user.second.infos.userid() == userid->to_string();
            });
        }
        inline typename decltype(EOSSDK_Connect::m_users)::iterator get_user_by_productid(EOS_ProductUserId productid)
        {
            return m_users.find(productid);
        }
        inline typename decltype(EOSSDK_Connect::m_users)::iterator get_user_by_name(string const& username)
        {
            return std::find_if(m_users.begin(), m_users.end(), [&username](std::pair<EOS_ProductUserId const, user_state_t>& user)
            {
                return user.second.infos.displayname() == username;
            });
        }
        inline typename decltype(EOSSDK_Connect::m_users)::iterator get_end_users()
        {
            return m_users.end();
        }

        bool send_connect_infos_request(Network::peer_t const& peerid, Connect_Request_Info_pb* req);
        bool send_connect_infos(Network::peer_t const& peerid, Connect_Infos_pb* infos);

        bool on_peer_connect(Network_Message_pb const& msg, Network_Peer_Connect_pb const& peer);
        bool on_peer_disconnect(Network_Message_pb const& msg, Network_Peer_Disconnect_pb const& peer);
        bool on_connect_infos_request(Network_Message_pb const& msg, Connect_Request_Info_pb const& req);
        bool on_connect_infos(Network_Message_pb const& msg, Connect_Infos_pb const& infos);

        virtual bool CBRunFrame();
        virtual bool RunNetwork(Network_Message_pb const& msg);
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void Login(const EOS_Connect_LoginOptions* Options, void* ClientData, const EOS_Connect_OnLoginCallback CompletionDelegate);
        void CreateUser(const EOS_Connect_CreateUserOptions* Options, void* ClientData, const EOS_Connect_OnCreateUserCallback CompletionDelegate);
        void LinkAccount(const EOS_Connect_LinkAccountOptions* Options, void* ClientData, const EOS_Connect_OnLinkAccountCallback CompletionDelegate);
        void UnlinkAccount(const EOS_Connect_UnlinkAccountOptions* Options, void* ClientData, const EOS_Connect_OnUnlinkAccountCallback CompletionDelegate);
        void CreateDeviceId(const EOS_Connect_CreateDeviceIdOptions* Options, void* ClientData, const EOS_Connect_OnCreateDeviceIdCallback CompletionDelegate);
        void DeleteDeviceId(const EOS_Connect_DeleteDeviceIdOptions* Options, void* ClientData, const EOS_Connect_OnDeleteDeviceIdCallback CompletionDelegate);
        void TransferDeviceIdAccount(const EOS_Connect_TransferDeviceIdAccountOptions* Options, void* ClientData, const EOS_Connect_OnTransferDeviceIdAccountCallback CompletionDelegate);
        void QueryExternalAccountMappings(const EOS_Connect_QueryExternalAccountMappingsOptions* Options, void* ClientData, const EOS_Connect_OnQueryExternalAccountMappingsCallback CompletionDelegate);
        void QueryProductUserIdMappings(const EOS_Connect_QueryProductUserIdMappingsOptions* Options, void* ClientData, const EOS_Connect_OnQueryProductUserIdMappingsCallback CompletionDelegate);
        EOS_ProductUserId GetExternalAccountMapping(const EOS_Connect_GetExternalAccountMappingsOptions* Options);
        EOS_EResult GetProductUserIdMapping(const EOS_Connect_GetProductUserIdMappingOptions* Options, char* OutBuffer, int32_t* InOutBufferLength);
        int32_t GetLoggedInUsersCount();
        EOS_ProductUserId GetLoggedInUserByIndex(int32_t Index);
        EOS_ELoginStatus GetLoginStatus(EOS_ProductUserId LocalUserId);
        EOS_NotificationId AddNotifyAuthExpiration(const EOS_Connect_AddNotifyAuthExpirationOptions* Options, void* ClientData, const EOS_Connect_OnAuthExpirationCallback Notification);
        void RemoveNotifyAuthExpiration(EOS_NotificationId InId);
        EOS_NotificationId AddNotifyLoginStatusChanged(const EOS_Connect_AddNotifyLoginStatusChangedOptions* Options, void* ClientData, const EOS_Connect_OnLoginStatusChangedCallback Notification);
        void RemoveNotifyLoginStatusChanged(EOS_NotificationId InId);
        uint32_t GetProductUserExternalAccountCount(const EOS_Connect_GetProductUserExternalAccountCountOptions * Options);
        EOS_EResult CopyProductUserExternalAccountByIndex(const EOS_Connect_CopyProductUserExternalAccountByIndexOptions * Options, EOS_Connect_ExternalAccountInfo * *OutExternalAccountInfo);
        EOS_EResult CopyProductUserExternalAccountByAccountType(const EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions * Options, EOS_Connect_ExternalAccountInfo * *OutExternalAccountInfo);
        EOS_EResult CopyProductUserExternalAccountByAccountId(const EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions * Options, EOS_Connect_ExternalAccountInfo * *OutExternalAccountInfo);
        EOS_EResult CopyProductUserInfo(const EOS_Connect_CopyProductUserInfoOptions * Options, EOS_Connect_ExternalAccountInfo * *OutExternalAccountInfo);
        EOS_EResult CopyIdToken(const EOS_Connect_CopyIdTokenOptions* Options, EOS_Connect_IdToken** OutIdToken);
    };
}