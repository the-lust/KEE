#include "utils/common_includes.h"
#include "kws/eossdk_kws.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(void) EOS_KWS_QueryPermissions(EOS_HKWS Handle, const EOS_KWS_QueryPermissionsOptions* Options, void* ClientData, const EOS_KWS_OnQueryPermissionsCallback CompletionDelegate)
{ GetEOS_KWS().QueryPermissions(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(int32_t) EOS_KWS_GetPermissionsCount(EOS_HKWS Handle, const EOS_KWS_GetPermissionsCountOptions* Options)
{ return GetEOS_KWS().GetPermissionsCount(Options); }

EOS_DECLARE_FUNC(EOS_EResult) EOS_KWS_CopyPermissionByIndex(EOS_HKWS Handle, const EOS_KWS_CopyPermissionByIndexOptions* Options, EOS_KWS_PermissionStatus** OutPermission)
{ return GetEOS_KWS().CopyPermissionByIndex(Options, OutPermission); }

EOS_DECLARE_FUNC(EOS_EResult) EOS_KWS_GetPermissionByKey(EOS_HKWS Handle, const EOS_KWS_GetPermissionByKeyOptions* Options, EOS_EKWSPermissionStatus* OutPermission)
{ return GetEOS_KWS().GetPermissionByKey(Options, OutPermission); }

EOS_DECLARE_FUNC(void) EOS_KWS_CreateUser(EOS_HKWS Handle, const EOS_KWS_CreateUserOptions* Options, void* ClientData, const EOS_KWS_OnCreateUserCallback CompletionDelegate)
{ GetEOS_KWS().CreateUser(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_KWS_QueryAgeGate(EOS_HKWS Handle, const EOS_KWS_QueryAgeGateOptions* Options, void* ClientData, const EOS_KWS_OnQueryAgeGateCallback CompletionDelegate)
{ GetEOS_KWS().QueryAgeGate(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_KWS_RequestPermissions(EOS_HKWS Handle, const EOS_KWS_RequestPermissionsOptions* Options, void* ClientData, const EOS_KWS_OnRequestPermissionsCallback CompletionDelegate)
{ GetEOS_KWS().RequestPermissions(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(void) EOS_KWS_UpdateParentEmail(EOS_HKWS Handle, const EOS_KWS_UpdateParentEmailOptions* Options, void* ClientData, const EOS_KWS_OnUpdateParentEmailCallback CompletionDelegate)
{ GetEOS_KWS().UpdateParentEmail(Options, ClientData, CompletionDelegate); }

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_KWS_AddNotifyPermissionsUpdateReceived(EOS_HKWS Handle, const EOS_KWS_AddNotifyPermissionsUpdateReceivedOptions* Options, void* ClientData, const EOS_KWS_OnPermissionsUpdateReceivedCallback NotificationFn)
{ return GetEOS_KWS().AddNotifyPermissionsUpdateReceived(Options, ClientData, NotificationFn); }

EOS_DECLARE_FUNC(void) EOS_KWS_RemoveNotifyPermissionsUpdateReceived(EOS_HKWS Handle, EOS_NotificationId Id)
{ GetEOS_KWS().RemoveNotifyPermissionsUpdateReceived(Id); }

EOS_DECLARE_FUNC(void) EOS_KWS_PermissionStatus_Release(EOS_KWS_PermissionStatus* p) { delete p; }
