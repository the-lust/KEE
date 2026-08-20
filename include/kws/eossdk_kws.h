#pragma once
#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_kws.h"
#include "sdk/eos_kws_types.h"

namespace sdk
{
    class EOSSDK_KWS : public IRunCallback
    {
    public:
        EOSSDK_KWS();
        ~EOSSDK_KWS();

        virtual bool CBRunFrame()                    override;
        virtual bool RunCallbacks(pFrameResult_t res) override;
        virtual void FreeCallback(pFrameResult_t res) override;

        void        QueryPermissions(const EOS_KWS_QueryPermissionsOptions*, void*, EOS_KWS_OnQueryPermissionsCallback);
        int32_t     GetPermissionsCount(const EOS_KWS_GetPermissionsCountOptions*);
        EOS_EResult CopyPermissionByIndex(const EOS_KWS_CopyPermissionByIndexOptions*, EOS_KWS_PermissionStatus**);
        EOS_EResult GetPermissionByKey(const EOS_KWS_GetPermissionByKeyOptions*, EOS_EKWSPermissionStatus*);
        void        CreateUser(const EOS_KWS_CreateUserOptions*, void*, EOS_KWS_OnCreateUserCallback);
        void        QueryAgeGate(const EOS_KWS_QueryAgeGateOptions*, void*, EOS_KWS_OnQueryAgeGateCallback);
        void        RequestPermissions(const EOS_KWS_RequestPermissionsOptions*, void*, EOS_KWS_OnRequestPermissionsCallback);
        void        UpdateParentEmail(const EOS_KWS_UpdateParentEmailOptions*, void*, EOS_KWS_OnUpdateParentEmailCallback);

        EOS_NotificationId AddNotifyPermissionsUpdateReceived(const EOS_KWS_AddNotifyPermissionsUpdateReceivedOptions*, void*, EOS_KWS_OnPermissionsUpdateReceivedCallback);
        void               RemoveNotifyPermissionsUpdateReceived(EOS_NotificationId);
    };
}
