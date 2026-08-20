
#pragma once

#include "eos_rtc_types.h"
#include "eos_rtc_audio_types.h"
#include "eos_rtc_data_types.h"

EOS_DECLARE_FUNC(EOS_HRTCAudio) EOS_RTC_GetAudioInterface(EOS_HRTC Handle);

EOS_DECLARE_FUNC(EOS_HRTCData) EOS_RTC_GetDataInterface(EOS_HRTC Handle);

EOS_DECLARE_FUNC(void) EOS_RTC_JoinRoom(EOS_HRTC Handle, const EOS_RTC_JoinRoomOptions* Options, void* ClientData, const EOS_RTC_OnJoinRoomCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_RTC_LeaveRoom(EOS_HRTC Handle, const EOS_RTC_LeaveRoomOptions* Options, void* ClientData, const EOS_RTC_OnLeaveRoomCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_RTC_BlockParticipant(EOS_HRTC Handle, const EOS_RTC_BlockParticipantOptions* Options, void* ClientData, const EOS_RTC_OnBlockParticipantCallback CompletionDelegate);

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyDisconnected(EOS_HRTC Handle, const EOS_RTC_AddNotifyDisconnectedOptions* Options, void* ClientData, const EOS_RTC_OnDisconnectedCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyDisconnected(EOS_HRTC Handle, EOS_NotificationId NotificationId);

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyParticipantStatusChanged(EOS_HRTC Handle, const EOS_RTC_AddNotifyParticipantStatusChangedOptions* Options, void* ClientData, const EOS_RTC_OnParticipantStatusChangedCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyParticipantStatusChanged(EOS_HRTC Handle, EOS_NotificationId NotificationId);

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyRoomBeforeJoin(EOS_HRTC Handle, const EOS_RTC_AddNotifyRoomBeforeJoinOptions* Options, void* ClientData, const EOS_RTC_OnRoomBeforeJoinCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyRoomBeforeJoin(EOS_HRTC Handle, EOS_NotificationId NotificationId);

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_SetSetting(EOS_HRTC Handle, const EOS_RTC_SetSettingOptions* Options);

EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_SetRoomSetting(EOS_HRTC Handle, const EOS_RTC_SetRoomSettingOptions* Options);

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyRoomStatisticsUpdated(EOS_HRTC Handle, const EOS_RTC_AddNotifyRoomStatisticsUpdatedOptions* Options, void* ClientData, const EOS_RTC_OnRoomStatisticsUpdatedCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyRoomStatisticsUpdated(EOS_HRTC Handle, EOS_NotificationId NotificationId);
