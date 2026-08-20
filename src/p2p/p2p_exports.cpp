#include "p2p/eossdk_p2p.h"

using namespace sdk;

extern "C"
{
    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_SetPortRange(EOS_HP2P Handle, const EOS_P2P_SetPortRangeOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->SetPortRange(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_GetPortRange(EOS_HP2P Handle, const EOS_P2P_GetPortRangeOptions* Options, uint16_t* OutPort, uint16_t* OutMaxAdditionalPortsToTry)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->GetPortRange(Options, OutPort, OutMaxAdditionalPortsToTry);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_SetRelayControl(EOS_HP2P Handle, const EOS_P2P_SetRelayControlOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->SetRelayControl(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_GetRelayControl(EOS_HP2P Handle, const EOS_P2P_GetRelayControlOptions* Options, EOS_ERelayControl* OutRelayControl)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->GetRelayControl(Options, OutRelayControl);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_SendPacket(EOS_HP2P Handle, const EOS_P2P_SendPacketOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->SendPacket(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_GetNextReceivedPacketSize(EOS_HP2P Handle, const void* Options, uint32_t* OutPacketSizeBytes)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        return EOS_EResult::EOS_NotFound;
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_ReceivePacket(EOS_HP2P Handle, const EOS_P2P_ReceivePacketOptions* Options, EOS_ProductUserId* OutPeerId, EOS_P2P_SocketId* OutSocketId, uint8_t* OutChannel, void* OutData, uint32_t* OutBytesWritten)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->ReceivePacket(Options, OutPeerId, OutSocketId, OutChannel, OutData, OutBytesWritten);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_P2P_AddNotifyPeerConnectionRequest(EOS_HP2P Handle, const EOS_P2P_AddNotifyPeerConnectionRequestOptions* Options, void* ClientData, EOS_P2P_OnIncomingConnectionRequestCallback ConnectionRequestHandler)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->AddNotifyPeerConnectionRequest(Options, ClientData, ConnectionRequestHandler);
    }

    EOS_DECLARE_FUNC(void) EOS_P2P_RemoveNotifyPeerConnectionRequest(EOS_HP2P Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        pInst->RemoveNotifyPeerConnectionRequest(InId);
    }

    EOS_DECLARE_FUNC(EOS_NotificationId) EOS_P2P_AddNotifyPeerConnectionClosed(EOS_HP2P Handle, const EOS_P2P_AddNotifyPeerConnectionClosedOptions* Options, void* ClientData, EOS_P2P_OnRemoteConnectionClosedCallback ConnectionClosedHandler)
    {
        if (Handle == nullptr)
            return EOS_INVALID_NOTIFICATIONID;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->AddNotifyPeerConnectionClosed(Options, ClientData, ConnectionClosedHandler);
    }

    EOS_DECLARE_FUNC(void) EOS_P2P_RemoveNotifyPeerConnectionClosed(EOS_HP2P Handle, EOS_NotificationId InId)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        pInst->RemoveNotifyPeerConnectionClosed(InId);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_AcceptConnection(EOS_HP2P Handle, const EOS_P2P_AcceptConnectionOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->AcceptConnection(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_CloseConnection(EOS_HP2P Handle, const EOS_P2P_CloseConnectionOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->CloseConnection(Options);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_CloseConnections(EOS_HP2P Handle, const EOS_P2P_CloseConnectionsOptions* Options)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->CloseConnections(Options);
    }

    EOS_DECLARE_FUNC(void) EOS_P2P_QueryNATType(EOS_HP2P Handle, const EOS_P2P_QueryNATTypeOptions* Options, void* ClientData, EOS_P2P_OnQueryNATTypeCompleteCallback NATTypeQueriedHandler)
    {
        if (Handle == nullptr)
            return;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        pInst->QueryNATType(Options, ClientData, NATTypeQueriedHandler);
    }

    EOS_DECLARE_FUNC(EOS_EResult) EOS_P2P_GetNATType(EOS_HP2P Handle, const EOS_P2P_GetNATTypeOptions* Options, EOS_ENATType* OutNATType)
    {
        if (Handle == nullptr)
            return EOS_EResult::EOS_InvalidParameters;

        auto pInst = reinterpret_cast<EOSSDK_P2P*>(Handle);
        return pInst->GetNATType(Options, OutNATType);
    }
}
