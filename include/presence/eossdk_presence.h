#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"

namespace sdk
{
    class EOSSDK_Presence;

    class EOSSDK_PresenceModification
    {
        friend EOSSDK_Presence;

        Presence_Info_pb infos;

        std::mutex m_local_mutex;

    public:
        EOS_EResult SetStatus(const EOS_PresenceModification_SetStatusOptions* Options);
        EOS_EResult SetRawRichText(const EOS_PresenceModification_SetRawRichTextOptions* Options);
        EOS_EResult SetData( const EOS_PresenceModification_SetDataOptions* Options);
        EOS_EResult DeleteData(const EOS_PresenceModification_DeleteDataOptions* Options);
        EOS_EResult SetJoinInfo(const EOS_PresenceModification_SetJoinInfoOptions* Options);
    };

    class EOSSDK_Presence :
        public IRunCallback,
        public IRunNetwork
    {
        static constexpr auto presence_query_timeout = std::chrono::milliseconds(1000);

        nlohmann::fifo_map<EOS_EpicAccountId, Presence_Info_pb> m_presences;
        std::unordered_map<EOS_EpicAccountId, std::list<pFrameResult_t>> m_presence_queries;

    public:
        EOSSDK_Presence();
        ~EOSSDK_Presence();

        void setup_myself();
        Presence_Info_pb& get_myself();
        Presence_Info_pb* get_presence(EOS_EpicAccountId userid);
        void trigger_presence_change(EOS_EpicAccountId userid);
        void set_user_status(EOS_EpicAccountId userid, EOS_Presence_EStatus status); 

        bool send_presence_info_request(Network::peer_t const& peerid, Presence_Info_Request_pb* req);
        bool send_my_presence_info(Network::peer_t const& peerid);
        bool send_my_presence_info_to_all_peers();

        bool on_peer_connect(Network_Message_pb const& msg, Network_Peer_Connect_pb const& peer);
        bool on_peer_disconnect(Network_Message_pb const& msg, Network_Peer_Disconnect_pb const& peer);
        bool on_presence_request(Network_Message_pb const& msg, Presence_Info_Request_pb const& req);
        bool on_presence_infos(Network_Message_pb const& msg, Presence_Info_pb const& infos);

        virtual bool CBRunFrame();
        virtual bool RunNetwork(Network_Message_pb const& msg);
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void               QueryPresence( const EOS_Presence_QueryPresenceOptions* Options, void* ClientData, const EOS_Presence_OnQueryPresenceCompleteCallback CompletionDelegate);
        EOS_Bool           HasPresence( const EOS_Presence_HasPresenceOptions* Options);
        EOS_EResult        CopyPresence( const EOS_Presence_CopyPresenceOptions* Options, EOS_Presence_Info** OutPresence);
        EOS_EResult        CreatePresenceModification( const EOS_Presence_CreatePresenceModificationOptions* Options, EOS_HPresenceModification* OutPresenceModificationHandle);
        void               SetPresence( const EOS_Presence_SetPresenceOptions* Options, void* ClientData, const EOS_Presence_SetPresenceCompleteCallback CompletionDelegate);
        EOS_NotificationId AddNotifyOnPresenceChanged( const EOS_Presence_AddNotifyOnPresenceChangedOptions* Options, void* ClientData, const EOS_Presence_OnPresenceChangedCallback NotificationHandler);
        void               RemoveNotifyOnPresenceChanged( EOS_NotificationId NotificationId);
        EOS_NotificationId AddNotifyJoinGameAccepted( const EOS_Presence_AddNotifyJoinGameAcceptedOptions* Options, void* ClientData, const EOS_Presence_OnJoinGameAcceptedCallback NotificationFn);
        void               RemoveNotifyJoinGameAccepted( EOS_NotificationId InId);
        EOS_EResult        GetJoinInfo( const EOS_Presence_GetJoinInfoOptions* Options, char* OutBuffer, int32_t* InOutBufferLength);
    };
}