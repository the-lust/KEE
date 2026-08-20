

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_P2PHandle* EOS_HP2P;

#define EOS_P2P_MAX_PACKET_SIZE 1170

#define EOS_P2P_MAX_CONNECTIONS 32

EOS_ENUM(EOS_ENATType,
	
	EOS_NAT_Unknown = 0,
	
	EOS_NAT_Open = 1,
	
	EOS_NAT_Moderate = 2,
	
	EOS_NAT_Strict = 3
);

#define EOS_P2P_SOCKETID_API_LATEST 1

#define EOS_P2P_SOCKETID_SOCKETNAME_SIZE 33

EOS_STRUCT(EOS_P2P_SocketId, (
	
	int32_t ApiVersion;
	
	char SocketName[EOS_P2P_SOCKETID_SOCKETNAME_SIZE];
));

EOS_ENUM(EOS_EPacketReliability,
	
	EOS_PR_UnreliableUnordered = 0,
	
	EOS_PR_ReliableUnordered = 1,
	
	EOS_PR_ReliableOrdered = 2
);

#define EOS_P2P_SENDPACKET_API_LATEST 3

EOS_STRUCT(EOS_P2P_SendPacketOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId RemoteUserId;
	
	const EOS_P2P_SocketId* SocketId;
	
	uint8_t Channel;
	
	uint32_t DataLengthBytes;
	
	const void* Data;
	
	EOS_Bool bAllowDelayedDelivery;
	
	EOS_EPacketReliability Reliability;
	
	EOS_Bool bDisableAutoAcceptConnection;
));

#define EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST 2

EOS_STRUCT(EOS_P2P_GetNextReceivedPacketSizeOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const uint8_t* RequestedChannel;

));

#define EOS_P2P_RECEIVEPACKET_API_LATEST 2

EOS_STRUCT(EOS_P2P_ReceivePacketOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	uint32_t MaxDataSizeBytes;
	
	const uint8_t* RequestedChannel;
));

#define EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST 1

EOS_STRUCT(EOS_P2P_AddNotifyPeerConnectionRequestOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

EOS_STRUCT(EOS_P2P_OnIncomingConnectionRequestInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId RemoteUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

EOS_DECLARE_CALLBACK(EOS_P2P_OnIncomingConnectionRequestCallback, const EOS_P2P_OnIncomingConnectionRequestInfo* Data);

#define EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_LATEST 1

EOS_STRUCT(EOS_P2P_AddNotifyPeerConnectionEstablishedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

EOS_ENUM(EOS_EConnectionEstablishedType,
	
	EOS_CET_NewConnection = 0,
	
	EOS_CET_Reconnection = 1
);

EOS_ENUM(EOS_ENetworkConnectionType,
	
	EOS_NCT_NoConnection = 0,
	
	EOS_NCT_DirectConnection = 1,
	
	EOS_NCT_RelayedConnection = 2
);

EOS_STRUCT(EOS_P2P_OnPeerConnectionEstablishedInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId RemoteUserId;
	
	const EOS_P2P_SocketId* SocketId;
	
	EOS_EConnectionEstablishedType ConnectionType;
	
	EOS_ENetworkConnectionType NetworkType;
));

EOS_DECLARE_CALLBACK(EOS_P2P_OnPeerConnectionEstablishedCallback, const EOS_P2P_OnPeerConnectionEstablishedInfo* Data);

#define EOS_P2P_ADDNOTIFYPEERCONNECTIONINTERRUPTED_API_LATEST 1

EOS_STRUCT(EOS_P2P_AddNotifyPeerConnectionInterruptedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

EOS_STRUCT(EOS_P2P_OnPeerConnectionInterruptedInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId RemoteUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

EOS_DECLARE_CALLBACK(EOS_P2P_OnPeerConnectionInterruptedCallback, const EOS_P2P_OnPeerConnectionInterruptedInfo* Data);

#define EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST 1

EOS_STRUCT(EOS_P2P_AddNotifyPeerConnectionClosedOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

EOS_ENUM(EOS_EConnectionClosedReason,
	
	EOS_CCR_Unknown = 0,
	
	EOS_CCR_ClosedByLocalUser = 1,
	
	EOS_CCR_ClosedByPeer = 2,
	
	EOS_CCR_TimedOut = 3,
	
	EOS_CCR_TooManyConnections = 4,
	
	EOS_CCR_InvalidMessage = 5,
	
	EOS_CCR_InvalidData = 6,
	
	EOS_CCR_ConnectionFailed = 7,
	
	EOS_CCR_ConnectionClosed = 8,
	
	EOS_CCR_NegotiationFailed = 9,
	
	EOS_CCR_UnexpectedError = 10,
	
	EOS_CCR_ConnectionIgnored = 11
);

EOS_STRUCT(EOS_P2P_OnRemoteConnectionClosedInfo, (
	
	void* ClientData;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId RemoteUserId;
	
	const EOS_P2P_SocketId* SocketId;
	
	EOS_EConnectionClosedReason Reason;
));

EOS_DECLARE_CALLBACK(EOS_P2P_OnRemoteConnectionClosedCallback, const EOS_P2P_OnRemoteConnectionClosedInfo* Data);

#define EOS_P2P_ACCEPTCONNECTION_API_LATEST 1

EOS_STRUCT(EOS_P2P_AcceptConnectionOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId RemoteUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

#define EOS_P2P_CLOSECONNECTION_API_LATEST 1

EOS_STRUCT(EOS_P2P_CloseConnectionOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId RemoteUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

#define EOS_P2P_CLOSECONNECTIONS_API_LATEST 1

EOS_STRUCT(EOS_P2P_CloseConnectionsOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

#define EOS_P2P_QUERYNATTYPE_API_LATEST 1

EOS_STRUCT(EOS_P2P_QueryNATTypeOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_P2P_OnQueryNATTypeCompleteInfo, (
	
	EOS_EResult ResultCode;
	
	void* ClientData;
	
	EOS_ENATType NATType;
));

EOS_DECLARE_CALLBACK(EOS_P2P_OnQueryNATTypeCompleteCallback, const EOS_P2P_OnQueryNATTypeCompleteInfo* Data);

#define EOS_P2P_GETNATTYPE_API_LATEST 1

EOS_STRUCT(EOS_P2P_GetNATTypeOptions, (
	
	int32_t ApiVersion;
));

EOS_ENUM(EOS_ERelayControl,
	
	EOS_RC_NoRelays = 0,
	
	EOS_RC_AllowRelays = 1,
	
	EOS_RC_ForceRelays = 2
);

#define EOS_P2P_SETRELAYCONTROL_API_LATEST 1

EOS_STRUCT(EOS_P2P_SetRelayControlOptions, (
	
	int32_t ApiVersion;
	
	EOS_ERelayControl RelayControl;
));

#define EOS_P2P_GETRELAYCONTROL_API_LATEST 1

EOS_STRUCT(EOS_P2P_GetRelayControlOptions, (
	
	int32_t ApiVersion;
));

#define EOS_P2P_SETPORTRANGE_API_LATEST 1

EOS_STRUCT(EOS_P2P_SetPortRangeOptions, (
	
	int32_t ApiVersion;
	
	uint16_t Port;
	
	uint16_t MaxAdditionalPortsToTry;
));

#define EOS_P2P_GETPORTRANGE_API_LATEST 1

EOS_STRUCT(EOS_P2P_GetPortRangeOptions, (
	
	int32_t ApiVersion;
));

#define EOS_P2P_MAX_QUEUE_SIZE_UNLIMITED 0

#define EOS_P2P_SETPACKETQUEUESIZE_API_LATEST 1

EOS_STRUCT(EOS_P2P_SetPacketQueueSizeOptions, (
	
	int32_t ApiVersion;
	
	uint64_t IncomingPacketQueueMaxSizeBytes;
	
	uint64_t OutgoingPacketQueueMaxSizeBytes;
));

#define EOS_P2P_GETPACKETQUEUEINFO_API_LATEST 1

EOS_STRUCT(EOS_P2P_GetPacketQueueInfoOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_P2P_PacketQueueInfo, (
	
	uint64_t IncomingPacketQueueMaxSizeBytes;
	
	uint64_t IncomingPacketQueueCurrentSizeBytes;
	
	uint64_t IncomingPacketQueueCurrentPacketCount;
	
	uint64_t OutgoingPacketQueueMaxSizeBytes;
	
	uint64_t OutgoingPacketQueueCurrentSizeBytes;
	
	uint64_t OutgoingPacketQueueCurrentPacketCount;
));

#define EOS_P2P_ADDNOTIFYINCOMINGPACKETQUEUEFULL_API_LATEST 1

EOS_STRUCT(EOS_P2P_AddNotifyIncomingPacketQueueFullOptions, (
	
	int32_t ApiVersion;
));

EOS_STRUCT(EOS_P2P_OnIncomingPacketQueueFullInfo, (
	
	void* ClientData;
	
	uint64_t PacketQueueMaxSizeBytes;
	
	uint64_t PacketQueueCurrentSizeBytes;
	
	EOS_ProductUserId OverflowPacketLocalUserId;
	
	uint8_t OverflowPacketChannel;
	
	uint32_t OverflowPacketSizeBytes;
));

EOS_DECLARE_CALLBACK(EOS_P2P_OnIncomingPacketQueueFullCallback, const EOS_P2P_OnIncomingPacketQueueFullInfo* Data);

#define EOS_P2P_CLEARPACKETQUEUE_API_LATEST 1

EOS_STRUCT(EOS_P2P_ClearPacketQueueOptions, (
	
	int32_t ApiVersion;
	
	EOS_ProductUserId LocalUserId;
	
	EOS_ProductUserId RemoteUserId;
	
	const EOS_P2P_SocketId* SocketId;
));

#pragma pack(pop)
