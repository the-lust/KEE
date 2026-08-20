#pragma once

// Minimal stub definitions for types referenced by export stubs
// that are not declared in the bundled SDK headers.

#include "sdk/eos_common.h"

// ── TextChat types (custom KEE additions, not in standard EOS SDK) ──────────
struct EOS_TextChatOptions {};
typedef struct EOS_TextChatHandle* EOS_HTextChat;
typedef struct EOS_TextChatServerHandle* EOS_HTextChatServer;

struct EOS_TextChat_SendMessageOptions { int32_t ApiVersion; };
struct EOS_TextChat_SendMessageCallbackInfo { EOS_EResult ResultCode; void* ClientData; EOS_ProductUserId LocalUserId; };
EOS_DECLARE_CALLBACK(EOS_TextChat_OnSendMessageCallback, const EOS_TextChat_SendMessageCallbackInfo* Data);

struct EOS_TextChat_AddNotifyMessageReceivedOptions { int32_t ApiVersion; };
EOS_DECLARE_CALLBACK(EOS_TextChat_OnMessageReceivedCallback, const void* Data);

struct EOS_TextChat_SetTextChatOptionsOptions { int32_t ApiVersion; };
struct EOS_TextChat_AddNotifyMessageReceivedCallbackInfo { EOS_EResult ResultCode; void* ClientData; };

struct EOS_TextChatServer_SendMessageOptions { int32_t ApiVersion; };
struct EOS_TextChatServer_SendMessageCallbackInfo { EOS_EResult ResultCode; void* ClientData; };
EOS_DECLARE_CALLBACK(EOS_TextChatServer_OnSendMessageCallback, const EOS_TextChatServer_SendMessageCallbackInfo* Data);

struct EOS_TextChatServer_AddNotifyMessageReceivedOptions { int32_t ApiVersion; };
EOS_DECLARE_CALLBACK(EOS_TextChatServer_OnMessageReceivedCallback, const void* Data);

struct EOS_TextChat_OnMessageReceivedCallbackInfo { EOS_EResult ResultCode; void* ClientData; };

// ── UI types not in SDK 1.19.x ────────────────────────────────────────────
struct EOS_UI_AddNotifyFocusChangedOptions { int32_t ApiVersion; };
EOS_DECLARE_CALLBACK(EOS_UI_OnFocusChangedCallback, const void* Data);

struct EOS_UI_AddNotifyLoseFocusRequestedOptions { int32_t ApiVersion; };
EOS_DECLARE_CALLBACK(EOS_UI_OnLoseFocusRequestedCallback, const void* Data);

struct EOS_UI_PromptVoiceTermsOptions { int32_t ApiVersion; };
struct EOS_UI_RegisterExternalFontOptions { int32_t ApiVersion; };
struct EOS_UI_SetFocusOptions { int32_t ApiVersion; };
struct EOS_UI_SetModeOptions { int32_t ApiVersion; };
struct EOS_UI_UpdateElementsOptions { int32_t ApiVersion; };
struct EOS_UI_IsValidButtonCombinationOptions { int32_t ApiVersion; };
