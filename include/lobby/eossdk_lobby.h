#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "network.h"

namespace sdk
{
    class EOSSDK_Lobby;

    class EOSSDK_LobbyModification
    {
        friend class sdk::EOSSDK_Lobby;

        std::mutex m_local_mutex;

        Lobby_Infos_pb m_infos;

        bool m_lobby_modified;
        bool m_member_modified;

    public:
        EOSSDK_LobbyModification();
        ~EOSSDK_LobbyModification();

        EOS_EResult SetPermissionLevel(const EOS_LobbyModification_SetPermissionLevelOptions* Options);
        EOS_EResult SetMaxMembers(const EOS_LobbyModification_SetMaxMembersOptions* Options);
        EOS_EResult AddAttribute(const EOS_LobbyModification_AddAttributeOptions* Options);
        EOS_EResult RemoveAttribute(const EOS_LobbyModification_RemoveAttributeOptions* Options);
        EOS_EResult AddMemberAttribute(const EOS_LobbyModification_AddMemberAttributeOptions* Options);
        EOS_EResult RemoveMemberAttribute(const EOS_LobbyModification_RemoveMemberAttributeOptions* Options);
        void        Release();
    };

    class EOSSDK_LobbySearch:
        public IRunCallback,
        public IRunNetwork
    {
        friend class sdk::EOSSDK_Lobby;

        static constexpr auto search_timeout = std::chrono::milliseconds(5000);
        static std::atomic<uint64_t> search_id;

        std::mutex m_local_mutex;

        bool                      m_released;
        uint32_t                  m_max_results;
        Lobbies_Search_pb         m_search_infos;
        pFrameResult_t            m_search_cb;
        std::set<Network::peer_t> m_search_peers;
        std::list<Lobby_Infos_pb> m_results;

    public:
        EOSSDK_LobbySearch();
        ~EOSSDK_LobbySearch();

        bool released();

        bool send_lobbies_search(Lobbies_Search_pb* search);

        bool on_lobbies_search_response(Network_Message_pb const& msg, Lobbies_Search_response_pb const& resp);

        virtual bool CBRunFrame();
        virtual bool RunNetwork(Network_Message_pb const& msg);
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void        Find(const EOS_LobbySearch_FindOptions* Options, void* ClientData, const EOS_LobbySearch_OnFindCallback CompletionDelegate);
        EOS_EResult SetLobbyId(const EOS_LobbySearch_SetLobbyIdOptions* Options);
        EOS_EResult SetTargetUserId(const EOS_LobbySearch_SetTargetUserIdOptions* Options);
        EOS_EResult SetParameter(const EOS_LobbySearch_SetParameterOptions* Options);
        EOS_EResult RemoveParameter(const EOS_LobbySearch_RemoveParameterOptions* Options);
        EOS_EResult SetMaxResults(const EOS_LobbySearch_SetMaxResultsOptions* Options);
        uint32_t    GetSearchResultCount(const EOS_LobbySearch_GetSearchResultCountOptions* Options);
        EOS_EResult CopySearchResultByIndex(const EOS_LobbySearch_CopySearchResultByIndexOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle);
        void        Release();
    };

    class EOSSDK_LobbyDetails
    {
        friend class sdk::EOSSDK_Lobby;
        friend class sdk::EOSSDK_LobbySearch;

        Lobby_Infos_pb m_infos;

    public:
        EOSSDK_LobbyDetails();
        ~EOSSDK_LobbyDetails();

        EOS_ProductUserId GetLobbyOwner(const EOS_LobbyDetails_GetLobbyOwnerOptions* Options);
        EOS_EResult       CopyInfo(const EOS_LobbyDetails_CopyInfoOptions* Options, EOS_LobbyDetails_Info** OutLobbyDetailsInfo);
        uint32_t          GetAttributeCount(const EOS_LobbyDetails_GetAttributeCountOptions* Options);
        EOS_EResult       CopyAttributeByIndex(const EOS_LobbyDetails_CopyAttributeByIndexOptions* Options, EOS_Lobby_Attribute** OutAttribute);
        EOS_EResult       CopyAttributeByKey(const EOS_LobbyDetails_CopyAttributeByKeyOptions* Options, EOS_Lobby_Attribute** OutAttribute);
        uint32_t          GetMemberCount(const EOS_LobbyDetails_GetMemberCountOptions* Options);
        EOS_ProductUserId GetMemberByIndex(const EOS_LobbyDetails_GetMemberByIndexOptions* Options);
        uint32_t          GetMemberAttributeCount(const EOS_LobbyDetails_GetMemberAttributeCountOptions* Options);
        EOS_EResult       CopyMemberAttributeByIndex(const EOS_LobbyDetails_CopyMemberAttributeByIndexOptions* Options, EOS_Lobby_Attribute** OutAttribute);
        EOS_EResult       CopyMemberAttributeByKey(const EOS_LobbyDetails_CopyMemberAttributeByKeyOptions* Options, EOS_Lobby_Attribute** OutAttribute);
        void              Release();
    };

    struct lobby_state_t
    {
        enum state_e
        {
            created,
            joined,
        } state;
        Lobby_Infos_pb infos;
    };

    struct lobby_invite_t
    {
        EOS_ProductUserId peer_id;
        Lobby_Infos_pb infos;
    };

    struct lobby_join_t
    {
        pFrameResult_t cb;
    };

    class EOSSDK_Lobby :
        public IRunCallback,
        public IRunNetwork
    {
        static int32_t join_id;
        constexpr static auto join_timeout = std::chrono::milliseconds(5000);

        std::unordered_map<string, lobby_state_t>  m_lobbies;
        std::list<EOSSDK_LobbySearch*>                  m_lobbies_searchs;
        nlohmann::fifo_map<string, lobby_invite_t> m_lobby_invites;
        std::unordered_map<int32_t, lobby_join_t>       m_joins_requests;

    public:
        EOSSDK_Lobby();
        ~EOSSDK_Lobby();

        inline lobby_state_t* get_lobby_by_id(string const& lobby_id);
        std::vector<lobby_state_t*> get_lobbies_from_attributes(std::map<string, Lobby_Search_Parameter> const& parameters);
        bool add_member_to_lobby(string const& member, lobby_state_t* lobby);
        bool remove_member_from_lobby(string const& member, lobby_state_t* lobby);
        bool is_member_in_lobby(string const& member, lobby_state_t* lobby);
        bool i_am_owner(lobby_state_t* lobby);
        void notify_lobby_update(lobby_state_t* lobby);
        void notify_lobby_member_status_update(string const& member, EOS_ELobbyMemberStatus new_status, lobby_state_t* lobby);
        void notify_lobby_member_update(string const& member, lobby_state_t* lobby);
        void notify_lobby_invite_received(string const& invite_id, EOS_ProductUserId from_id);

        bool send_to_all_members(Network_Message_pb& msg, lobby_state_t* lobby);
        bool send_to_all_members_or_owner(Network_Message_pb& msg, lobby_state_t* lobby);
        bool send_lobby_update           (lobby_state_t* pLobby);
        bool send_lobbies_search_response(Network::peer_t const& peerid, Lobbies_Search_response_pb *resp);
        bool send_lobby_join_request     (Network::peer_t const& peerid, Lobby_Join_Request_pb      *req);
        bool send_lobby_join_response    (Network::peer_t const& peerid, Lobby_Join_Response_pb     *resp);
        bool send_lobby_invite           (Network::peer_t const& peerid, Lobby_Invite_pb            *invite);

        bool send_lobby_member_update    (Network::peer_t const& member_id, lobby_state_t *lobby);
        bool send_lobby_member_join      (Network::peer_t const& member_id, lobby_state_t *lobby);
        bool send_lobby_member_leave     (Network::peer_t const& member_id, lobby_state_t *lobby, EOS_ELobbyMemberStatus reason);
        bool send_lobby_member_promote   (Network::peer_t const& member_id, lobby_state_t *lobby);

        bool on_peer_disconnect     (Network_Message_pb const& msg, Network_Peer_Disconnect_pb const& peer);
        bool on_lobby_update        (Network_Message_pb const& msg, Lobby_Update_pb            const& update);
        bool on_lobbies_search      (Network_Message_pb const& msg, Lobbies_Search_pb          const& search);
        bool on_lobby_join_request  (Network_Message_pb const& msg, Lobby_Join_Request_pb      const& req);
        bool on_lobby_join_response (Network_Message_pb const& msg, Lobby_Join_Response_pb     const& resp);
        bool on_lobby_invite        (Network_Message_pb const& msg, Lobby_Invite_pb            const& invite);

        bool on_lobby_member_update (Network_Message_pb const& msg, Lobby_Member_Update_pb  const& update);
        bool on_lobby_member_join   (Network_Message_pb const& msg, Lobby_Member_Join_pb    const& join);
        bool on_lobby_member_leave  (Network_Message_pb const& msg, Lobby_Member_Leave_pb   const& leave);
        bool on_lobby_member_promote(Network_Message_pb const& msg, Lobby_Member_Promote_pb const& promote);

        virtual bool CBRunFrame();
        virtual bool RunNetwork(Network_Message_pb const& msg);
        virtual bool RunCallbacks(pFrameResult_t res);
        virtual void FreeCallback(pFrameResult_t res);

        void               CreateLobby(const EOS_Lobby_CreateLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnCreateLobbyCallback CompletionDelegate);
        void               DestroyLobby(const EOS_Lobby_DestroyLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnDestroyLobbyCallback CompletionDelegate);
        void               JoinLobby(const EOS_Lobby_JoinLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnJoinLobbyCallback CompletionDelegate);
        void               LeaveLobby(const EOS_Lobby_LeaveLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnLeaveLobbyCallback CompletionDelegate);
        EOS_EResult        UpdateLobbyModification(const EOS_Lobby_UpdateLobbyModificationOptions* Options, EOS_HLobbyModification* OutLobbyModificationHandle);
        void               UpdateLobby(const EOS_Lobby_UpdateLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnUpdateLobbyCallback CompletionDelegate);
        void               PromoteMember(const EOS_Lobby_PromoteMemberOptions* Options, void* ClientData, const EOS_Lobby_OnPromoteMemberCallback CompletionDelegate);
        void               KickMember(const EOS_Lobby_KickMemberOptions* Options, void* ClientData, const EOS_Lobby_OnKickMemberCallback CompletionDelegate);
        EOS_NotificationId AddNotifyLobbyUpdateReceived(const EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyUpdateReceivedCallback NotificationFn);
        void               RemoveNotifyLobbyUpdateReceived(EOS_NotificationId InId);
        EOS_NotificationId AddNotifyLobbyMemberUpdateReceived(const EOS_Lobby_AddNotifyLobbyMemberUpdateReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyMemberUpdateReceivedCallback NotificationFn);
        void               RemoveNotifyLobbyMemberUpdateReceived(EOS_NotificationId InId);
        EOS_NotificationId AddNotifyLobbyMemberStatusReceived(const EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyMemberStatusReceivedCallback NotificationFn);
        void               RemoveNotifyLobbyMemberStatusReceived(EOS_NotificationId InId);
        void               SendInvite(const EOS_Lobby_SendInviteOptions* Options, void* ClientData, const EOS_Lobby_OnSendInviteCallback CompletionDelegate);
        void               RejectInvite(const EOS_Lobby_RejectInviteOptions* Options, void* ClientData, const EOS_Lobby_OnRejectInviteCallback CompletionDelegate);
        void               QueryInvites(const EOS_Lobby_QueryInvitesOptions* Options, void* ClientData, const EOS_Lobby_OnQueryInvitesCallback CompletionDelegate);
        uint32_t           GetInviteCount(const EOS_Lobby_GetInviteCountOptions* Options);
        EOS_EResult        GetInviteIdByIndex(const EOS_Lobby_GetInviteIdByIndexOptions* Options, char* OutBuffer, int32_t* InOutBufferLength);
        EOS_EResult        CreateLobbySearch(const EOS_Lobby_CreateLobbySearchOptions* Options, EOS_HLobbySearch* OutLobbySearchHandle);
        EOS_NotificationId AddNotifyLobbyInviteReceived(const EOS_Lobby_AddNotifyLobbyInviteReceivedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyInviteReceivedCallback NotificationFn);
        void               RemoveNotifyLobbyInviteReceived(EOS_NotificationId InId);
        EOS_NotificationId AddNotifyLobbyInviteAccepted(const EOS_Lobby_AddNotifyLobbyInviteAcceptedOptions* Options, void* ClientData, const EOS_Lobby_OnLobbyInviteAcceptedCallback NotificationFn);
        void               RemoveNotifyLobbyInviteAccepted(EOS_NotificationId InId);
        EOS_NotificationId AddNotifyJoinLobbyAccepted(const EOS_Lobby_AddNotifyJoinLobbyAcceptedOptions* Options, void* ClientData, const EOS_Lobby_OnJoinLobbyAcceptedCallback NotificationFn);
        void               RemoveNotifyJoinLobbyAccepted(EOS_NotificationId InId);
        EOS_EResult        CopyLobbyDetailsHandleByInviteId(const EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle);
        EOS_EResult        CopyLobbyDetailsHandleByUiEventId(const EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle);
        EOS_EResult        CopyLobbyDetailsHandle(const EOS_Lobby_CopyLobbyDetailsHandleOptions* Options, EOS_HLobbyDetails* OutLobbyDetailsHandle);
    };
}