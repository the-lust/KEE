#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_rtc_admin.h"
#include "sdk/eos_rtc_admin_types.h"

namespace sdk
{
    class EOSSDK_RTCAdmin : public IRunCallback
    {
        struct TokenEntry {
            EOS_ProductUserId user_id;
            string       token;
        };

        std::mutex           m_mutex;
        std::vector<TokenEntry>    m_tokens;
        std::list<string>     m_token_strings; 
        uint32_t             m_query_id{0};
        uint32_t             m_next_query_id{1};

    public:
        EOSSDK_RTCAdmin();
        ~EOSSDK_RTCAdmin();

        virtual bool CBRunFrame()                    override;
        virtual bool RunCallbacks(pFrameResult_t res) override;
        virtual void FreeCallback(pFrameResult_t res) override;

        void QueryJoinRoomToken(const EOS_RTCAdmin_QueryJoinRoomTokenOptions*, void*, EOS_RTCAdmin_OnQueryJoinRoomTokenCompleteCallback);
        EOS_EResult CopyUserTokenByIndex(const EOS_RTCAdmin_CopyUserTokenByIndexOptions*, EOS_RTCAdmin_UserToken**);
        EOS_EResult CopyUserTokenByUserId(const EOS_RTCAdmin_CopyUserTokenByUserIdOptions*, EOS_RTCAdmin_UserToken**);
        void Kick(const EOS_RTCAdmin_KickOptions*, void*, EOS_RTCAdmin_OnKickCompleteCallback);
        void SetParticipantHardMute(const EOS_RTCAdmin_SetParticipantHardMuteOptions*, void*, EOS_RTCAdmin_OnSetParticipantHardMuteCompleteCallback);
    };
}
