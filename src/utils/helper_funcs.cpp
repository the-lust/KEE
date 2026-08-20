#include "helper_funcs.h"

union epicid_t {
    struct {
        uint64_t part1;
        uint64_t part2;
    };
    uint8_t id[16];

    inline string to_string() const
    {
        std::stringstream sstr;

        sstr << std::hex
            << std::setfill('0') << std::setw(16) << part2
            << std::setfill('0') << std::setw(16) << part1;
        return sstr.str();
    }
};

LOCAL_API std::random_device& get_rd()
{
    static std::random_device rd;
    return rd;
}

LOCAL_API std::mt19937_64& get_gen()
{
    static std::mt19937_64 gen(get_rd()());
    return gen;
}

LOCAL_API std::recursive_mutex& global_mutex()
{
    static std::recursive_mutex global_mutex;
    return global_mutex;
}

LOCAL_API void random_string(string const& charset, char* buff, size_t length)
{
    std::uniform_int_distribution<int64_t> dis(0, charset.length() - 1);
    auto& gen = get_gen();

    for (int i = 0; i < length; ++i)
    {
        buff[i] = charset[dis(gen)];
    }
}

LOCAL_API static void randombytes(uint8_t* buf, size_t len)
{
    std::uniform_int_distribution<int64_t> dis;
    std::mt19937_64& gen = get_gen();

    (void)dis(gen);
    (void)dis(gen);

    size_t rand_buf_len = len / sizeof(int64_t) + (len % sizeof(int64_t) ? 1 : 0);
    int64_t* rand_buf = new int64_t[rand_buf_len];
    for (size_t i = 0; i < rand_buf_len; ++i)
        rand_buf[i] = dis(gen);

    memcpy(buf, rand_buf, len);
    delete[]rand_buf;
}

LOCAL_API string generate_account_id()
{
    epicid_t epicid;
    randombytes(epicid.id, sizeof(epicid));
    return epicid.to_string();
}

LOCAL_API string generate_account_id_from_name(string const& username)
{
    epicid_t epicid = {};
    epicid_t base = {};
    uint16_t i;

    while (epicid.part1 == 0 && epicid.part2 == 0)
    {
        epicid = base;
        if ((base.part1 + 0x0000001201030307ULL) < base.part1)
            base.part2 += static_cast<uint64_t>(static_cast<double>((std::numeric_limits<uint64_t>::max)()) - base.part1 + static_cast<double>(0x0000001201030307));

        base.part1 += 0x0000001201030307ULL;

        i = 0;
        std::for_each(username.begin(), username.end(), [&epicid, &i](const char& c)
        {
            uint8_t b = static_cast<uint8_t>(c);
            epicid.id[i   % sizeof(epicid.id)] ^= (b + i * 27);
            epicid.id[(sizeof(epicid.id)-1) - i % sizeof(epicid.id)] ^= (b - i * 8);
            ++i;
        });
    }

    return epicid.to_string();
}

LOCAL_API string generate_epic_id_user()
{
    return generate_account_id();
}

LOCAL_API string generate_epic_id_user_from_name(string const& username)
{
    return generate_account_id_from_name(username);
}

// Generate a deterministic ProductUserId from (app_id + epic_id)
// Uses a separate seed to avoid collision with EpicAccountId.
LOCAL_API string generate_product_user_id_from_name(string const& app_id, string const& epic_id)
{
    epicid_t epid = {};
    epicid_t base = {};
    uint16_t i;

    // Combine app_id and epic_id for uniqueness
    string combined = app_id + ":" + epic_id;

    while (epid.part1 == 0 && epid.part2 == 0)
    {
        epid = base;
        if ((base.part1 + 0x0000001201030307ULL) < base.part1)
            base.part2 += static_cast<uint64_t>(static_cast<double>((std::numeric_limits<uint64_t>::max)()) - base.part1 + static_cast<double>(0x0000001201030307));

        base.part1 += 0x0000001201030307ULL;

        i = 1; // start at 1 to differentiate from account_id
        std::for_each(combined.begin(), combined.end(), [&epid, &i](const char& c)
        {
            uint8_t b = static_cast<uint8_t>(c);
            epid.id[i   % sizeof(epid.id)] ^= (b + i * 13);
            epid.id[(sizeof(epid.id)-1) - i % sizeof(epid.id)] ^= (b - i * 5);
            ++i;
        });
    }

    return epid.to_string();
}

LOCAL_API string generate_product_user_id()
{
    return generate_product_user_id_from_name("DefaultGame", "00000000000000000000000000000000");
}

LOCAL_API void fatal_throw(const char* msg)
{
    APP_LOG(Log::LogLevel::FATAL, "%s", msg);
    throw std::exception();
}

LOCAL_API string get_callback_name(int iCallback)
{

#define I_CB(TYPE) if (iCallback == static_cast<int>(TYPE##_ID)) return #TYPE
    
    I_CB(EOS_Auth_LoginCallbackInfo);
    I_CB(EOS_Auth_LogoutCallbackInfo);
    I_CB(EOS_Auth_LinkAccountCallbackInfo);
    I_CB(EOS_Auth_VerifyUserAuthCallbackInfo);
    I_CB(EOS_Auth_DeletePersistentAuthCallbackInfo);
    I_CB(EOS_Auth_LoginStatusChangedCallbackInfo);
    
    I_CB(EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo);
    I_CB(EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo);
    I_CB(EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo);
    
    I_CB(EOS_Connect_LoginCallbackInfo);
    I_CB(EOS_Connect_CreateUserCallbackInfo);
    I_CB(EOS_Connect_LinkAccountCallbackInfo);
    I_CB(EOS_Connect_UnlinkAccountCallbackInfo);
    I_CB(EOS_Connect_CreateDeviceIdCallbackInfo);
    I_CB(EOS_Connect_DeleteDeviceIdCallbackInfo);
    I_CB(EOS_Connect_TransferDeviceIdAccountCallbackInfo);
    I_CB(EOS_Connect_QueryExternalAccountMappingsCallbackInfo);
    I_CB(EOS_Connect_QueryProductUserIdMappingsCallbackInfo);
    I_CB(EOS_Connect_AuthExpirationCallbackInfo);
    I_CB(EOS_Connect_LoginStatusChangedCallbackInfo);
    
    I_CB(EOS_Ecom_QueryOwnershipCallbackInfo);
    I_CB(EOS_Ecom_QueryOwnershipTokenCallbackInfo);
    I_CB(EOS_Ecom_QueryEntitlementsCallbackInfo);
    I_CB(EOS_Ecom_QueryOffersCallbackInfo);
    I_CB(EOS_Ecom_CheckoutCallbackInfo);
    I_CB(EOS_Ecom_RedeemEntitlementsCallbackInfo);
    
    I_CB(EOS_Friends_QueryFriendsCallbackInfo);
    I_CB(EOS_Friends_SendInviteCallbackInfo);
    I_CB(EOS_Friends_AcceptInviteCallbackInfo);
    I_CB(EOS_Friends_RejectInviteCallbackInfo);
    I_CB(EOS_Friends_OnFriendsUpdateInfo);
    
    I_CB(EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo);
    I_CB(EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo);
    I_CB(EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo);
    
    I_CB(EOS_Lobby_CreateLobbyCallbackInfo);
    I_CB(EOS_Lobby_DestroyLobbyCallbackInfo);
    I_CB(EOS_Lobby_JoinLobbyCallbackInfo);
    I_CB(EOS_Lobby_LeaveLobbyCallbackInfo);
    I_CB(EOS_Lobby_UpdateLobbyCallbackInfo);
    I_CB(EOS_Lobby_PromoteMemberCallbackInfo);
    I_CB(EOS_Lobby_KickMemberCallbackInfo);
    I_CB(EOS_Lobby_LobbyUpdateReceivedCallbackInfo);
    I_CB(EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo);
    I_CB(EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo);
    I_CB(EOS_Lobby_LobbyInviteReceivedCallbackInfo);
    I_CB(EOS_Lobby_SendInviteCallbackInfo);
    I_CB(EOS_Lobby_JoinLobbyAcceptedCallbackInfo);
    I_CB(EOS_Lobby_RejectInviteCallbackInfo);
    I_CB(EOS_Lobby_QueryInvitesCallbackInfo);
    I_CB(EOS_LobbySearch_FindCallbackInfo);
    
    I_CB(EOS_P2P_OnIncomingConnectionRequestInfo);
    I_CB(EOS_P2P_OnRemoteConnectionClosedInfo);
    I_CB(EOS_P2P_OnQueryNATTypeCompleteInfo);
    
    I_CB(EOS_PlayerDataStorage_QueryFileCallbackInfo);
    I_CB(EOS_PlayerDataStorage_QueryFileListCallbackInfo);
    I_CB(EOS_PlayerDataStorage_DuplicateFileCallbackInfo);
    I_CB(EOS_PlayerDataStorage_DeleteFileCallbackInfo);
    I_CB(EOS_PlayerDataStorage_FileTransferProgressCallbackInfo);
    I_CB(EOS_PlayerDataStorage_ReadFileDataCallbackInfo);
    I_CB(EOS_PlayerDataStorage_ReadFileCallbackInfo);
    I_CB(EOS_PlayerDataStorage_WriteFileDataCallbackInfo);
    I_CB(EOS_PlayerDataStorage_WriteFileCallbackInfo);
    
    I_CB(EOS_Presence_QueryPresenceCallbackInfo);
    I_CB(EOS_Presence_SetPresenceCallbackInfo);
    I_CB(EOS_Presence_PresenceChangedCallbackInfo);
    I_CB(EOS_Presence_JoinGameAcceptedCallbackInfo);
    
    I_CB(EOS_Sessions_SendInviteCallbackInfo);
    I_CB(EOS_Sessions_RejectInviteCallbackInfo);
    I_CB(EOS_Sessions_QueryInvitesCallbackInfo);
    I_CB(EOS_Sessions_UpdateSessionCallbackInfo);
    I_CB(EOS_Sessions_DestroySessionCallbackInfo);
    I_CB(EOS_Sessions_JoinSessionCallbackInfo);
    I_CB(EOS_Sessions_StartSessionCallbackInfo);
    I_CB(EOS_Sessions_EndSessionCallbackInfo);
    I_CB(EOS_Sessions_RegisterPlayersCallbackInfo);
    I_CB(EOS_Sessions_UnregisterPlayersCallbackInfo);
    I_CB(EOS_SessionSearch_FindCallbackInfo);
    I_CB(EOS_Sessions_SessionInviteReceivedCallbackInfo);
    I_CB(EOS_Sessions_SessionInviteAcceptedCallbackInfo);
    I_CB(EOS_Sessions_JoinSessionAcceptedCallbackInfo);
    
    I_CB(EOS_Stats_IngestStatCompleteCallbackInfo);
    I_CB(EOS_Stats_OnQueryStatsCompleteCallbackInfo);
    
    I_CB(EOS_UI_ShowFriendsCallbackInfo);
    I_CB(EOS_UI_HideFriendsCallbackInfo);
    I_CB(EOS_UI_OnDisplaySettingsUpdatedCallbackInfo);
    
    I_CB(EOS_UserInfo_QueryUserInfoCallbackInfo);
    I_CB(EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo);
    I_CB(EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo);
#undef I_CB

    return "";
}

LOCAL_API const char* search_attr_to_string(EOS_EOnlineComparisonOp comp)
{
    switch (comp)
    {
        case EOS_EOnlineComparisonOp::EOS_CO_EQUAL             : return "==";
        case EOS_EOnlineComparisonOp::EOS_CO_NOTEQUAL          : return "!=";
        case EOS_EOnlineComparisonOp::EOS_CO_GREATERTHAN       : return ">";
        case EOS_EOnlineComparisonOp::EOS_CO_GREATERTHANOREQUAL: return ">=";
        case EOS_EOnlineComparisonOp::EOS_CO_LESSTHAN          : return "<";
        case EOS_EOnlineComparisonOp::EOS_CO_LESSTHANOREQUAL   : return "<=";
    }
    return "?";
}