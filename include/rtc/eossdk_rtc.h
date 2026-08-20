#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_rtc.h"
#include "sdk/eos_rtc_types.h"
#include "sdk/eos_rtc_audio.h"
namespace sdk { class EOSSDK_Rtcaudio; }

namespace sdk
{
    class EOSSDK_RTC : public IRunCallback
    {
        std::mutex           m_rooms_mutex;
        std::set<string> m_joined_rooms;

    public:
        EOSSDK_RTC();
        ~EOSSDK_RTC();

        virtual bool CBRunFrame()                    override;
        virtual bool RunCallbacks(pFrameResult_t res) override;
        virtual void FreeCallback(pFrameResult_t res) override;

        void       JoinRoom(const EOS_RTC_JoinRoomOptions*, void*, EOS_RTC_OnJoinRoomCallback);
        void       LeaveRoom(const EOS_RTC_LeaveRoomOptions*, void*, EOS_RTC_OnLeaveRoomCallback);
        void       BlockParticipant(const EOS_RTC_BlockParticipantOptions*, void*, EOS_RTC_OnBlockParticipantCallback);

        EOS_EResult AddNotifyDisconnected(const EOS_RTC_AddNotifyDisconnectedOptions*, void*, EOS_RTC_OnDisconnectedCallback, EOS_NotificationId*);
        void        RemoveNotifyDisconnected(EOS_NotificationId);

        EOS_EResult AddNotifyParticipantStatusChanged(const EOS_RTC_AddNotifyParticipantStatusChangedOptions*, void*, EOS_RTC_OnParticipantStatusChangedCallback, EOS_NotificationId*);
        void        RemoveNotifyParticipantStatusChanged(EOS_NotificationId);

        void* GetAudioInterface();
    };
}
