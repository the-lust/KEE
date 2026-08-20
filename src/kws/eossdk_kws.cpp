
#include "kws/eossdk_kws.h"
#include "eossdk_platform.h"
#include "settings.h"

namespace sdk {

static const char* s_permission_keys[] = {
    "Social",
    "Commerce",
    "Communication",
    "UserGeneratedContent",
    "Tracking",
    nullptr
};
static const size_t s_permission_count = 5;

EOSSDK_KWS::EOSSDK_KWS()
{
    GetCB_Manager().register_callbacks(this);
}

EOSSDK_KWS::~EOSSDK_KWS()
{
    GetCB_Manager().unregister_callbacks(this);
}

void EOSSDK_KWS::QueryPermissions(
    const EOS_KWS_QueryPermissionsOptions*  Options,
    void*                                   ClientData,
    EOS_KWS_OnQueryPermissionsCallback      CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_KWS_QueryPermissionsCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_KWS_QueryPermissionsCallbackInfo*>(data));
        });
    cb.ResultCode         = EOS_EResult::EOS_Success;
    cb.ClientData         = ClientData;
    cb.LocalUserId        = Options ? Options->LocalUserId : nullptr;
    cb.KWSUserId          = "kee-kws-user";
    cb.DateOfBirth        = "1990-01-01";
    cb.bIsMinor           = EOS_FALSE;
    
    SE_CB_END(this);
}

int32_t EOSSDK_KWS::GetPermissionsCount(
    const EOS_KWS_GetPermissionsCountOptions* )
{
    return static_cast<int32_t>(s_permission_count);
}

EOS_EResult EOSSDK_KWS::CopyPermissionByIndex(
    const EOS_KWS_CopyPermissionByIndexOptions* Options,
    EOS_KWS_PermissionStatus**                  OutPermission)
{
    if (!Options || !OutPermission) return EOS_EResult::EOS_InvalidParameters;
    if (Options->Index >= s_permission_count) return EOS_EResult::EOS_NotFound;

    auto* ps = new EOS_KWS_PermissionStatus{};
    ps->ApiVersion    = EOS_KWS_PERMISSIONSTATUS_API_LATEST;
    ps->Name = s_permission_keys[Options->Index];
    ps->Status        = EOS_EKWSPermissionStatus::EOS_KPS_GRANTED;
    *OutPermission    = ps;
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_KWS::GetPermissionByKey(
    const EOS_KWS_GetPermissionByKeyOptions* Options,
    EOS_EKWSPermissionStatus*                OutPermission)
{
    if (!Options || !OutPermission) return EOS_EResult::EOS_InvalidParameters;
    *OutPermission = EOS_EKWSPermissionStatus::EOS_KPS_GRANTED;
    return EOS_EResult::EOS_Success;
}

void EOSSDK_KWS::CreateUser(
    const EOS_KWS_CreateUserOptions*    Options,
    void*                               ClientData,
    EOS_KWS_OnCreateUserCallback        CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_KWS_CreateUserCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_KWS_CreateUserCallbackInfo*>(data));
        });
    cb.ResultCode  = EOS_EResult::EOS_Success;
    cb.ClientData  = ClientData;
    cb.LocalUserId = Options ? Options->LocalUserId : nullptr;
    cb.KWSUserId   = "kee-kws-user";
    SE_CB_END(this);
}

void EOSSDK_KWS::QueryAgeGate(
    const EOS_KWS_QueryAgeGateOptions* Options,
    void*                              ClientData,
    EOS_KWS_OnQueryAgeGateCallback     CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_KWS_QueryAgeGateCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_KWS_QueryAgeGateCallbackInfo*>(data));
        });
    cb.ResultCode   = EOS_EResult::EOS_Success;
    cb.ClientData   = ClientData;
    cb.CountryCode  = "US";
    cb.AgeOfConsent = 0; 
    SE_CB_END(this);
}

void EOSSDK_KWS::RequestPermissions(
    const EOS_KWS_RequestPermissionsOptions* Options,
    void*                                    ClientData,
    EOS_KWS_OnRequestPermissionsCallback     CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_KWS_RequestPermissionsCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_KWS_RequestPermissionsCallbackInfo*>(data));
        });
    cb.ResultCode  = EOS_EResult::EOS_Success;
    cb.ClientData  = ClientData;
    cb.LocalUserId = Options ? Options->LocalUserId : nullptr;
    //cb.KWSUserId   = "kee-kws-user"; // not in SDK struct
    SE_CB_END(this);
}

void EOSSDK_KWS::UpdateParentEmail(
    const EOS_KWS_UpdateParentEmailOptions* Options,
    void*                                   ClientData,
    EOS_KWS_OnUpdateParentEmailCallback     CompletionDelegate)
{
    if (!CompletionDelegate) return;

    pFrameResult_t res(new FrameResult);
    auto& cb = res->CreateCallback<EOS_KWS_UpdateParentEmailCallbackInfo>(
        [CompletionDelegate](void* data) {
            CompletionDelegate(reinterpret_cast<EOS_KWS_UpdateParentEmailCallbackInfo*>(data));
        });
    cb.ResultCode  = EOS_EResult::EOS_Success;
    cb.ClientData  = ClientData;
    cb.LocalUserId = Options ? Options->LocalUserId : nullptr;
    //cb.KWSUserId   = "kee-kws-user"; // not in SDK struct
    SE_CB_END(this);
}

EOS_NotificationId EOSSDK_KWS::AddNotifyPermissionsUpdateReceived(
    const EOS_KWS_AddNotifyPermissionsUpdateReceivedOptions*,
    void* ClientData,
    EOS_KWS_OnPermissionsUpdateReceivedCallback NotificationFn)
{
    if (!NotificationFn) return EOS_INVALID_NOTIFICATIONID;
    pFrameResult_t res(new FrameResult);
    res->CreateCallback<EOS_KWS_PermissionsUpdateReceivedCallbackInfo>(
        [NotificationFn](void* data) {
            NotificationFn(reinterpret_cast<EOS_KWS_PermissionsUpdateReceivedCallbackInfo*>(data));
        }).ClientData = ClientData;
    return GetCB_Manager().add_notification(this, res);
}

void EOSSDK_KWS::RemoveNotifyPermissionsUpdateReceived(EOS_NotificationId Id)
{
    GetCB_Manager().remove_notification(this, Id);
}

bool EOSSDK_KWS::CBRunFrame()  { return false; }
bool EOSSDK_KWS::RunCallbacks(pFrameResult_t res)
{
    res->GetCallbackMsg().cb_func(res->GetFuncParam());
    return true;
}
void EOSSDK_KWS::FreeCallback(pFrameResult_t) {}

} 
