
#include "presence/eossdk_presence.h"

using namespace sdk;

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetStatus(EOS_HPresenceModification Handle, const EOS_PresenceModification_SetStatusOptions* Options)
{
    if (Handle == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    auto pInst = reinterpret_cast<EOSSDK_PresenceModification*>(Handle);
    return pInst->SetStatus(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetRawRichText(EOS_HPresenceModification Handle, const EOS_PresenceModification_SetRawRichTextOptions* Options)
{
    if (Handle == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    auto pInst = reinterpret_cast<EOSSDK_PresenceModification*>(Handle);
    return pInst->SetRawRichText(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetData(EOS_HPresenceModification Handle, const EOS_PresenceModification_SetDataOptions* Options)
{
    if (Handle == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    auto pInst = reinterpret_cast<EOSSDK_PresenceModification*>(Handle);
    return pInst->SetData(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_DeleteData(EOS_HPresenceModification Handle, const EOS_PresenceModification_DeleteDataOptions* Options)
{
    if (Handle == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    auto pInst = reinterpret_cast<EOSSDK_PresenceModification*>(Handle);
    return pInst->DeleteData(Options);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_PresenceModification_SetJoinInfo(EOS_HPresenceModification Handle, const EOS_PresenceModification_SetJoinInfoOptions* Options)
{
    if (Handle == nullptr)
        return EOS_EResult::EOS_InvalidParameters;
    auto pInst = reinterpret_cast<EOSSDK_PresenceModification*>(Handle);
    return pInst->SetJoinInfo(Options);
}

EOS_DECLARE_FUNC(void) EOS_PresenceModification_Release(EOS_HPresenceModification PresenceModificationHandle)
{
    if (PresenceModificationHandle == nullptr)
        return;
    auto pInst = reinterpret_cast<EOSSDK_PresenceModification*>(PresenceModificationHandle);
    delete pInst;
}
