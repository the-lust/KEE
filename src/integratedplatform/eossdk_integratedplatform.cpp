#include "eossdk_integratedplatform.h"

namespace sdk {

EOSSDK_IntegratedPlatform::EOSSDK_IntegratedPlatform()
{
}

EOSSDK_IntegratedPlatform::~EOSSDK_IntegratedPlatform()
{
}

EOS_EResult EOSSDK_IntegratedPlatform::ClearUserPreLoginOptions(const EOS_IntegratedPlatform_ClearUserPreLoginOptionsOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_IntegratedPlatform::SetUserPreLoginOptions(const EOS_IntegratedPlatform_SetUserPreLoginOptionsOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

EOS_EResult EOSSDK_IntegratedPlatform::SetUserLoginStatus(const EOS_IntegratedPlatform_SetUserLoginStatusOptions* Options)
{
    return EOS_EResult::EOS_Success;
}

}
