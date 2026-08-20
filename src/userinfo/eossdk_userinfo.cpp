#include "eossdk_userinfo.h"
#include "eossdk_platform.h"
#include "eos_client_api.h"
#include "settings.h"

namespace sdk
{

decltype(EOSSDK_UserInfo::userinfo_query_timeout) EOSSDK_UserInfo::userinfo_query_timeout;

EOSSDK_UserInfo::EOSSDK_UserInfo()
{
    GetNetwork().register_listener(this, 0, Network_Message_pb::MessagesCase::kUserinfo);
    GetCB_Manager().register_callbacks(this);
    GetCB_Manager().register_frame(this);
}

EOSSDK_UserInfo::~EOSSDK_UserInfo()
{
    GetCB_Manager().unregister_frame(this);
    GetCB_Manager().unregister_callbacks(this);
    GetNetwork().unregister_listener(this, 0, Network_Message_pb::MessagesCase::kUserinfo);
}

void EOSSDK_UserInfo::setup_myself()
{
    auto& userinfo = get_myself();

    userinfo.set_country("");
    userinfo.set_displayname(Settings::Inst().username);
    userinfo.set_nickname(Settings::Inst().username);
    userinfo.set_preferredlanguage(Settings::Inst().language);
}

UserInfo_Info_pb& EOSSDK_UserInfo::get_myself()
{
    return m_userinfos[Settings::Inst().userid];
}

UserInfo_Info_pb* EOSSDK_UserInfo::get_userinfo(EOS_EpicAccountId userid)
{
    auto it = m_userinfos.find(userid);
    if (it != m_userinfos.end())
        return &it->second;

    return nullptr;
}

void EOSSDK_UserInfo::QueryUserInfo(const EOS_UserInfo_QueryUserInfoOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionDelegate == nullptr)
        return;

    APP_LOG(Log::LogLevel::DEBUG, "Query infos of %s", Options->TargetUserId->to_string().c_str());

    pFrameResult_t res(new FrameResult);
    EOS_UserInfo_QueryUserInfoCallbackInfo& quici = res->CreateCallback<EOS_UserInfo_QueryUserInfoCallbackInfo>((CallbackFunc)CompletionDelegate);
    quici.ClientData = ClientData;
    quici.LocalUserId = Settings::Inst().userid;

    if (Options == nullptr || Options->TargetUserId == nullptr)
    {
        quici.TargetUserId = GetEpicUserId(sdk::NULL_USER_ID);
        quici.ResultCode = EOS_EResult::EOS_InvalidParameters;

        res->done = true;
    }
    else
    {
        quici.TargetUserId = Options->TargetUserId;

        auto user = GetEOS_Connect().get_user_by_userid(Options->TargetUserId);
        if (user == GetEOS_Connect().get_end_users())
        {
            quici.ResultCode = EOS_EResult::EOS_NotFound;
            res->done = true;
        }
        else if (user->first == Settings::Inst().productuserid)
        {
            quici.ResultCode = EOS_EResult::EOS_Success;
            res->done = true;
        }
        else if(user->second.connected)
        {
            m_userinfos_queries[Options->TargetUserId].push_back(res);

            UserInfo_Info_Request_pb* request = new UserInfo_Info_Request_pb;
            send_userinfo_request(user->first->to_string(), request);
        }
        else
        {
            quici.ResultCode = EOS_EResult::EOS_NotFound;
            res->done = true;
        }
    }

    GetCB_Manager().add_callback(this, res);
}

void EOSSDK_UserInfo::QueryUserInfoByDisplayName(const EOS_UserInfo_QueryUserInfoByDisplayNameOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (CompletionDelegate == nullptr)
        return;

    APP_LOG(Log::LogLevel::DEBUG, "Query infos of %s", Options->DisplayName);

    pFrameResult_t res(new FrameResult);
    EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo& quibdnci = res->CreateCallback<EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo>((CallbackFunc)CompletionDelegate);
    quibdnci.ClientData = ClientData;
    quibdnci.LocalUserId = Settings::Inst().userid;

    if (Options == nullptr || Options->DisplayName == nullptr)
    {
        quibdnci.TargetUserId = GetEpicUserId(sdk::NULL_USER_ID);
        quibdnci.ResultCode = EOS_EResult::EOS_InvalidParameters;

        res->done = true;
    }
    else
    {
        auto user = GetEOS_Connect().get_user_by_name(Options->DisplayName);
        if (user == GetEOS_Connect().get_end_users())
        {
            quibdnci.ResultCode = EOS_EResult::EOS_NotFound;
            res->done = true;
        }
        else if (user->first == Settings::Inst().productuserid)
        {
            quibdnci.ResultCode = EOS_EResult::EOS_Success;
            res->done = true;
        }
        else if(user->second.connected)
        {
            quibdnci.TargetUserId = GetEpicUserId(user->second.infos.userid());
            m_userinfos_queries[quibdnci.TargetUserId].push_back(res);

            UserInfo_Info_Request_pb* request = new UserInfo_Info_Request_pb;
            send_userinfo_request(user->first->to_string(), request);
        }
        else
        {
            quibdnci.ResultCode = EOS_EResult::EOS_NotFound;
            res->done = true;
        }
    }

    GetCB_Manager().add_callback(this, res);
}

void EOSSDK_UserInfo::QueryUserInfoByExternalAccount(const EOS_UserInfo_QueryUserInfoByExternalAccountOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoByExternalAccountCallback CompletionDelegate)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    SE_CB_BEGIN(EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo, CompletionDelegate);
    info.AccountType = Options->AccountType;
    info.ExternalAccountId = "";
    info.LocalUserId = Settings::Inst().userid;
    info.TargetUserId = GetInvalidEpicUserId();
    info.ResultCode = EOS_EResult::EOS_UnexpectedError;

    SE_CB_END(this);
}

EOS_EResult EOSSDK_UserInfo::CopyUserInfo(const EOS_UserInfo_CopyUserInfoOptions* Options, EOS_UserInfo** OutUserInfo)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    if (OutUserInfo == nullptr || Options == nullptr || Options->TargetUserId == nullptr || OutUserInfo == nullptr)
    {
        set_nullptr(OutUserInfo);
        return EOS_EResult::EOS_InvalidParameters;
    }

    APP_LOG(Log::LogLevel::DEBUG, "Copy infos of %s", Options->TargetUserId->to_string().c_str());

    UserInfo_Info_pb* userinfo = get_userinfo(Options->TargetUserId);

    EOS_UserInfo* infos = new EOS_UserInfo();
    *OutUserInfo = infos;

    if (userinfo == nullptr)
    {
        memset(infos, 0, sizeof(*infos));
        return EOS_EResult::EOS_NotFound;
    }
   
    infos->ApiVersion        = EOS_USERINFO_COPYUSERINFO_API_LATEST;
    infos->Country           = (userinfo->country().empty()           ? nullptr : userinfo->country().c_str());
    infos->PreferredLanguage = (userinfo->preferredlanguage().empty() ? nullptr : userinfo->preferredlanguage().c_str());
    infos->DisplayName       = (userinfo->displayname().empty()       ? nullptr : userinfo->displayname().c_str());
    infos->Nickname          = (userinfo->nickname().empty()          ? nullptr : userinfo->nickname().c_str());
    infos->UserId            = Options->TargetUserId;

    return EOS_EResult::EOS_Success;
}

uint32_t EOSSDK_UserInfo::GetExternalUserInfoCount(const EOS_UserInfo_GetExternalUserInfoCountOptions* Options)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    return 0;
}

EOS_EResult EOSSDK_UserInfo::CopyExternalUserInfoByIndex(const EOS_UserInfo_CopyExternalUserInfoByIndexOptions* Options, EOS_UserInfo_ExternalUserInfo** OutExternalUserInfo)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutExternalUserInfo);
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_UserInfo::CopyExternalUserInfoByAccountType(const EOS_UserInfo_CopyExternalUserInfoByAccountTypeOptions* Options, EOS_UserInfo_ExternalUserInfo** OutExternalUserInfo)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutExternalUserInfo);
    return EOS_EResult::EOS_NotFound;
}

EOS_EResult EOSSDK_UserInfo::CopyExternalUserInfoByAccountId(const EOS_UserInfo_CopyExternalUserInfoByAccountIdOptions* Options, EOS_UserInfo_ExternalUserInfo** OutExternalUserInfo)
{
    TRACE_FUNC();
    lock _(GLOBAL_MUTEX);

    set_nullptr(OutExternalUserInfo);
    return EOS_EResult::EOS_NotFound;
}

bool EOSSDK_UserInfo::send_userinfo_request(Network::peer_t const& peerid, UserInfo_Info_Request_pb* req)
{
    Network_Message_pb msg;
    UserInfo_Message_pb* userinfo = new UserInfo_Message_pb;

    string const& userid = Settings::Inst().productuserid->to_string();

    userinfo->set_allocated_userinfo_info_request(req);
    msg.set_allocated_userinfo(userinfo);

    msg.set_source_id(userid);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    return GetNetwork().TCPSendTo(msg);
}

bool EOSSDK_UserInfo::send_my_userinfo(Network::peer_t const& peerid)
{
    Network_Message_pb msg;
    UserInfo_Message_pb* userinfo = new UserInfo_Message_pb;

    string const& userid = Settings::Inst().productuserid->to_string();

    userinfo->set_allocated_userinfo_info(&get_myself());
    msg.set_allocated_userinfo(userinfo);

    msg.set_source_id(userid);
    msg.set_dest_id(peerid);
    msg.set_game_id(Settings::Inst().appid);

    auto res = GetNetwork().TCPSendTo(msg);
    (void)userinfo->release_userinfo_info();

    return res;
}

bool EOSSDK_UserInfo::on_userinfo_request(Network_Message_pb const& msg, UserInfo_Info_Request_pb const& req)
{
    lock _(GLOBAL_MUTEX);

    return send_my_userinfo(msg.source_id());
}

bool EOSSDK_UserInfo::on_userinfo(Network_Message_pb const& msg, UserInfo_Info_pb const& infos)
{
    lock _(GLOBAL_MUTEX);

    auto user = GetEOS_Connect().get_user_by_productid(GetProductUserId(msg.source_id()));
    if (user != GetEOS_Connect().get_end_users())
    {
        EOS_EpicAccountId user_id = GetEpicUserId(user->second.infos.userid());
        m_userinfos[user_id] = infos;
        auto it = m_userinfos_queries.find(user_id);
        if (it != m_userinfos_queries.end())
        {
            auto result_it = it->second.begin();
            if (result_it != it->second.end())
            {
                switch ((*result_it)->ICallback())
                {
                    case EOS_UserInfo_QueryUserInfoCallbackInfo::k_iCallback:
                    {
                        EOS_UserInfo_QueryUserInfoCallbackInfo& quici = (*result_it)->GetCallback<EOS_UserInfo_QueryUserInfoCallbackInfo>();
                        quici.ResultCode = EOS_EResult::EOS_Success;
                    }
                    break;
                    case EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo::k_iCallback:
                    {
                        EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo& quibdnci = (*result_it)->GetCallback<EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo>();
                        quibdnci.ResultCode = EOS_EResult::EOS_Success;
                    }
                    break;
                }

                (*result_it)->done = true;

                it->second.erase(result_it);
            }
        }
    }

    return true;
}

bool EOSSDK_UserInfo::CBRunFrame()
{
    lock _(GLOBAL_MUTEX);

    for (auto& queries : m_userinfos_queries)
    {
        for (auto query_it = queries.second.begin(); query_it != queries.second.end();)
        {
            if ((clock::now() - (*query_it)->created_time) > userinfo_query_timeout)
            {
                switch ((*query_it)->ICallback())
                {
                    case EOS_UserInfo_QueryUserInfoCallbackInfo::k_iCallback:
                    {
                        EOS_UserInfo_QueryUserInfoCallbackInfo& quici = (*query_it)->GetCallback<EOS_UserInfo_QueryUserInfoCallbackInfo>();
                        quici.ResultCode = EOS_EResult::EOS_TimedOut;
                    }
                    break;
                    case EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo::k_iCallback:
                    {
                        EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo& quibdnci = (*query_it)->GetCallback<EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo>();
                        quibdnci.ResultCode = EOS_EResult::EOS_TimedOut;
                    }
                    break;
                }

                (*query_it)->done = true;
                query_it = queries.second.erase(query_it);
            }
            else
                ++query_it;
        }
    }

    return true;
}

bool EOSSDK_UserInfo::RunNetwork(Network_Message_pb const& msg)
{
    if (msg.source_id() == Settings::Inst().userid->to_string())
        return true;

    UserInfo_Message_pb const& userinfo = msg.userinfo();

    switch (userinfo.message_case())
    {
        case UserInfo_Message_pb::MessageCase::kUserinfoInfoRequest: return on_userinfo_request(msg, userinfo.userinfo_info_request());
        case UserInfo_Message_pb::MessageCase::kUserinfoInfo       : return on_userinfo(msg, userinfo.userinfo_info());
    }

    return true;
}

bool EOSSDK_UserInfo::RunCallbacks(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    return res->done;
}

void EOSSDK_UserInfo::FreeCallback(pFrameResult_t res)
{
    lock _(GLOBAL_MUTEX);

    {

    }
}

}