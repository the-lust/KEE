#pragma once

#include "common_includes.h"
#include "callback_manager.h"
#include "sdk/eos_integratedplatform.h"
#include "sdk/eos_integratedplatform_types.h"

// Forward-declare custom types not in SDK 1.19.x
namespace sdk {
    struct EOS_IntegratedPlatform_ClearUserPreLoginOptionsOptions;
    struct EOS_IntegratedPlatform_SetUserPreLoginOptionsOptions;
}

namespace sdk
{
    class EOSSDK_IntegratedPlatform : public IRunCallback
    {
    public:
        EOSSDK_IntegratedPlatform();
        ~EOSSDK_IntegratedPlatform();

        virtual bool CBRunFrame() override { return false; }
        virtual bool RunCallbacks(pFrameResult_t res) override { return false; }
        virtual void FreeCallback(pFrameResult_t res) override {}

        EOS_EResult ClearUserPreLoginOptions(const EOS_IntegratedPlatform_ClearUserPreLoginOptionsOptions* Options);
        EOS_EResult SetUserPreLoginOptions(const EOS_IntegratedPlatform_SetUserPreLoginOptionsOptions* Options);
        EOS_EResult SetUserLoginStatus(const EOS_IntegratedPlatform_SetUserLoginStatusOptions* Options);
    };
}
