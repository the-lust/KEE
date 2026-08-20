

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_UIHandle* EOS_HUI;

EOS_EXTERN_C typedef uint64_t EOS_UI_EventId;

#define EOS_UI_EVENTID_INVALID 0

#define EOS_UI_SHOWFRIENDS_API_LATEST 1

EOS_STRUCT(EOS_UI_ShowFriendsOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_STRUCT(EOS_UI_ShowFriendsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_UI_OnShowFriendsCallback, const EOS_UI_ShowFriendsCallbackInfo* Data);

#define EOS_UI_HIDEFRIENDS_API_LATEST 1

EOS_STRUCT(EOS_UI_HideFriendsOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_STRUCT(EOS_UI_HideFriendsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
));

EOS_DECLARE_CALLBACK(EOS_UI_OnHideFriendsCallback, const EOS_UI_HideFriendsCallbackInfo* Data);

#define EOS_UI_GETFRIENDSVISIBLE_API_LATEST 1

EOS_STRUCT(EOS_UI_GetFriendsVisibleOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

#define EOS_UI_GETFRIENDSEXCLUSIVEINPUT_API_LATEST 1

EOS_STRUCT(EOS_UI_GetFriendsExclusiveInputOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
));

#define EOS_UI_ADDNOTIFYDISPLAYSETTINGSUPDATED_API_LATEST 1

EOS_STRUCT(EOS_UI_AddNotifyDisplaySettingsUpdatedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_UI_OnDisplaySettingsUpdatedCallbackInfo, (
	
	void* ClientData;
	
	EOS_Bool bIsVisible;
	
	EOS_Bool bIsExclusiveInput;
));

EOS_DECLARE_CALLBACK(EOS_UI_OnDisplaySettingsUpdatedCallback, const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo* Data);

#define EOS_UI_KEY_CONSTANT(Prefix, Name, Value) Prefix ## Name = Value,
#define EOS_UI_KEY_MODIFIER(Prefix, Name, Value) Prefix ## Name = Value,
#define EOS_UI_KEY_MODIFIER_LAST(Prefix, Name, Value) Prefix ## Name = Value
#define EOS_UI_KEY_ENTRY_FIRST(Prefix, Name, Value) Prefix ## Name = Value,
#define EOS_UI_KEY_ENTRY(Prefix, Name) Prefix ## Name,
#define EOS_UI_KEY_CONSTANT_LAST(Prefix, Name) Prefix ## Name
EOS_ENUM_START(EOS_UI_EKeyCombination)
#include "eos_ui_keys.h"
EOS_ENUM_END(EOS_UI_EKeyCombination);
EOS_ENUM_BOOLEAN_OPERATORS(EOS_UI_EKeyCombination)

EOS_ENUM_START(EOS_UI_EInputStateButtonFlags)
#include "eos_ui_buttons.h"
EOS_ENUM_END(EOS_UI_EInputStateButtonFlags);
EOS_ENUM_BOOLEAN_OPERATORS(EOS_UI_EInputStateButtonFlags);
#undef EOS_UI_KEY_CONSTANT
#undef EOS_UI_KEY_MODIFIER
#undef EOS_UI_KEY_MODIFIER_LAST
#undef EOS_UI_KEY_ENTRY_FIRST
#undef EOS_UI_KEY_ENTRY
#undef EOS_UI_KEY_CONSTANT_LAST

#define EOS_UI_SETTOGGLEFRIENDSKEY_API_LATEST 1

EOS_STRUCT(EOS_UI_SetToggleFriendsKeyOptions, (
	
	int32_t ApiVersion;
	
	EOS_UI_EKeyCombination KeyCombination;
));

#define EOS_UI_GETTOGGLEFRIENDSKEY_API_LATEST 1

EOS_STRUCT(EOS_UI_GetToggleFriendsKeyOptions, (
	
	int32_t ApiVersion;
));

#define EOS_UI_SETTOGGLEFRIENDSBUTTON_API_LATEST 1

EOS_STRUCT(EOS_UI_SetToggleFriendsButtonOptions, (
	
	int32_t ApiVersion;
	
	EOS_UI_EInputStateButtonFlags ButtonCombination;
));

#define EOS_UI_GETTOGGLEFRIENDSBUTTON_API_LATEST 1

EOS_STRUCT(EOS_UI_GetToggleFriendsButtonOptions, (
	
	int32_t ApiVersion;
));

EOS_ENUM(EOS_UI_ENotificationLocation,
	
	EOS_UNL_TopLeft,
	
	EOS_UNL_TopRight,
	
	EOS_UNL_BottomLeft,
	
	EOS_UNL_BottomRight
);

#define EOS_UI_SETDISPLAYPREFERENCE_API_LATEST 1

EOS_STRUCT(EOS_UI_SetDisplayPreferenceOptions, (
	
	int32_t ApiVersion;
	
	EOS_UI_ENotificationLocation NotificationLocation;
));

#define EOS_UI_ACKNOWLEDGEEVENTID_API_LATEST 1

#define EOS_UI_ACKNOWLEDGECORRELATIONID_API_LATEST EOS_UI_ACKNOWLEDGEEVENTID_API_LATEST

EOS_STRUCT(EOS_UI_AcknowledgeEventIdOptions, (
	
	int32_t ApiVersion;
	
	EOS_UI_EventId UiEventId;
	
	EOS_EResult Result;
));

#define EOS_UI_REPORTINPUTSTATE_API_LATEST 2

EOS_STRUCT(EOS_UI_ReportInputStateOptions, (
	
	int32_t ApiVersion;

	EOS_UI_EInputStateButtonFlags ButtonDownFlags;

	EOS_Bool bAcceptIsFaceButtonRight;

	EOS_Bool bMouseButtonDown;

	uint32_t MousePosX;

	uint32_t MousePosY;

	uint32_t GamepadIndex;

	float LeftStickX;

	float LeftStickY;

	float RightStickX;

	float RightStickY;

	float LeftTrigger;

	float RightTrigger;
));

#define EOS_UI_PREPRESENT_API_LATEST 1

EOS_STRUCT(EOS_UI_PrePresentOptions, (
	
	int32_t ApiVersion;
	
	const void* PlatformSpecificData;
));

#define EOS_UI_SHOWBLOCKPLAYER_API_LATEST 1

EOS_STRUCT(EOS_UI_ShowBlockPlayerOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_STRUCT(EOS_UI_OnShowBlockPlayerCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_UI_OnShowBlockPlayerCallback, const EOS_UI_OnShowBlockPlayerCallbackInfo* Data);

#define EOS_UI_SHOWREPORTPLAYER_API_LATEST 1

EOS_STRUCT(EOS_UI_ShowReportPlayerOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_STRUCT(EOS_UI_OnShowReportPlayerCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_UI_OnShowReportPlayerCallback, const EOS_UI_OnShowReportPlayerCallbackInfo* Data);

#define EOS_UI_SHOWNATIVEPROFILE_API_LATEST 1

EOS_STRUCT(EOS_UI_ShowNativeProfileOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_STRUCT(EOS_UI_ShowNativeProfileCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EpicAccountId TargetUserId;
));

EOS_DECLARE_CALLBACK(EOS_UI_OnShowNativeProfileCallback, const EOS_UI_ShowNativeProfileCallbackInfo* Data);

#define EOS_UI_PAUSESOCIALOVERLAY_API_LATEST 1

EOS_STRUCT(EOS_UI_PauseSocialOverlayOptions, (
	
	int32_t ApiVersion;
	
	EOS_Bool bIsPaused;
));

#define EOS_UI_ISSOCIALOVERLAYPAUSED_API_LATEST 1

EOS_STRUCT(EOS_UI_IsSocialOverlayPausedOptions, (
	
	int32_t ApiVersion;
));

#define EOS_UI_RECT_API_LATEST 1

EOS_STRUCT(EOS_UI_Rect, (
	
	int32_t ApiVersion;
	
	int32_t X;
	
	int32_t Y;
	
	uint32_t Width;
	
	uint32_t Height;
));

EOS_STRUCT(EOS_UI_MemoryMonitorCallbackInfo, (
	
	void* ClientData;

	const void* SystemMemoryMonitorReport;
));

#define EOS_UI_ADDNOTIFYMEMORYMONITOR_API_LATEST 1

#define EOS_UI_ADDNOTIFYMEMORYMONITOROPTIONS_API_LATEST EOS_UI_ADDNOTIFYMEMORYMONITOR_API_LATEST

EOS_STRUCT(EOS_UI_AddNotifyMemoryMonitorOptions, (
	
	int32_t ApiVersion;
));

EOS_DECLARE_CALLBACK(EOS_UI_OnMemoryMonitorCallback, const EOS_UI_MemoryMonitorCallbackInfo* Data);

EOS_ENUM(EOS_UI_EOnScreenKeyboardBehavior,
	
	EOS_UIOSKB_None = 0,
	
	EOS_UIOSKB_Notification = 1,
	
	EOS_UIOSKB_Native = 2
);

#define EOS_UI_CONFIGUREONSCREENKEYBOARD_API_LATEST 1

EOS_STRUCT(EOS_UI_ConfigureOnScreenKeyboardOptions, (
	
	int32_t ApiVersion;
	
	EOS_UI_EOnScreenKeyboardBehavior Behavior;
	
	EOS_Bool bIsDeviceChecksEnabled;
));

EOS_ENUM(EOS_UI_EOnScreenKeyboardType,
	
	EOS_UIOSKT_None = 0,
	
	EOS_UIOSKT_Default = 1,
	
	EOS_UIOSKT_Text = 2,
	
	EOS_UIOSKT_TelephoneNumber = 3,
	
	EOS_UIOSKT_URL = 4,
	
	EOS_UIOSKT_Email = 5,
	
	EOS_UIOSKT_Numeric = 6,
	
	EOS_UIOSKT_Decimal = 7,
	
	EOS_UIOSKT_Search = 8
);

#define EOS_UI_ADDNOTIFYONSCREENKEYBOARDREQUESTED_API_LATEST 1

EOS_STRUCT(EOS_UI_AddNotifyOnScreenKeyboardRequestedOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_UI_OnScreenKeyboardRequestedCallbackInfo, (
	
	void* ClientData;
	
	EOS_UI_EOnScreenKeyboardType Type;
));

EOS_DECLARE_CALLBACK(EOS_UI_OnScreenKeyboardRequestedCallback, const EOS_UI_OnScreenKeyboardRequestedCallbackInfo* Data);

#pragma pack(pop)
