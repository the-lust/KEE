
#pragma once

#include "eos_kws_types.h"

EOS_DECLARE_FUNC(void) EOS_KWS_QueryAgeGate(EOS_HKWS Handle, const EOS_KWS_QueryAgeGateOptions* Options, void* ClientData, const EOS_KWS_OnQueryAgeGateCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_KWS_CreateUser(EOS_HKWS Handle, const EOS_KWS_CreateUserOptions* Options, void* ClientData, const EOS_KWS_OnCreateUserCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_KWS_QueryPermissions(EOS_HKWS Handle, const EOS_KWS_QueryPermissionsOptions* Options, void* ClientData, const EOS_KWS_OnQueryPermissionsCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_KWS_UpdateParentEmail(EOS_HKWS Handle, const EOS_KWS_UpdateParentEmailOptions* Options, void* ClientData, const EOS_KWS_OnUpdateParentEmailCallback CompletionDelegate);

EOS_DECLARE_FUNC(void) EOS_KWS_RequestPermissions(EOS_HKWS Handle, const EOS_KWS_RequestPermissionsOptions* Options, void* ClientData, const EOS_KWS_OnRequestPermissionsCallback CompletionDelegate);

EOS_DECLARE_FUNC(int32_t) EOS_KWS_GetPermissionsCount(EOS_HKWS Handle, const EOS_KWS_GetPermissionsCountOptions* Options);

EOS_DECLARE_FUNC(EOS_EResult) EOS_KWS_CopyPermissionByIndex(EOS_HKWS Handle, const EOS_KWS_CopyPermissionByIndexOptions* Options, EOS_KWS_PermissionStatus ** OutPermission);

EOS_DECLARE_FUNC(EOS_EResult) EOS_KWS_GetPermissionByKey(EOS_HKWS Handle, const EOS_KWS_GetPermissionByKeyOptions* Options, EOS_EKWSPermissionStatus* OutPermission);

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_KWS_AddNotifyPermissionsUpdateReceived(EOS_HKWS Handle, const EOS_KWS_AddNotifyPermissionsUpdateReceivedOptions* Options, void* ClientData, const EOS_KWS_OnPermissionsUpdateReceivedCallback NotificationFn);

EOS_DECLARE_FUNC(void) EOS_KWS_RemoveNotifyPermissionsUpdateReceived(EOS_HKWS Handle, EOS_NotificationId InId);
