

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef void* EOS_AntiCheatCommon_ClientHandle;

EOS_ENUM(EOS_EAntiCheatCommonClientType,
	
	EOS_ACCCT_ProtectedClient = 0,
	
	EOS_ACCCT_UnprotectedClient = 1,
	
	EOS_ACCCT_AIBot = 2
);

EOS_ENUM(EOS_EAntiCheatCommonClientPlatform,
	
	EOS_ACCCP_Unknown = 0,
	
	EOS_ACCCP_Windows = 1,
	
	EOS_ACCCP_Mac = 2,
	
	EOS_ACCCP_Linux = 3,
	
	EOS_ACCCP_Xbox = 4,
	
	EOS_ACCCP_PlayStation = 5,
	
	EOS_ACCCP_Nintendo = 6,
	
	EOS_ACCCP_iOS = 7,
	
	EOS_ACCCP_Android = 8
);

EOS_ENUM(EOS_EAntiCheatCommonClientAction,
	
	EOS_ACCCA_Invalid = 0,
	
	EOS_ACCCA_RemovePlayer = 1
);

EOS_ENUM(EOS_EAntiCheatCommonClientActionReason,
	
	EOS_ACCCAR_Invalid = 0,
	
	EOS_ACCCAR_InternalError = 1,
	
	EOS_ACCCAR_InvalidMessage = 2,
	
	EOS_ACCCAR_AuthenticationFailed = 3,
	
	EOS_ACCCAR_NullClient = 4,
	
	EOS_ACCCAR_HeartbeatTimeout = 5,
	
	EOS_ACCCAR_ClientViolation = 6,
	
	EOS_ACCCAR_BackendViolation = 7,
	
	EOS_ACCCAR_TemporaryCooldown = 8,
	
	EOS_ACCCAR_TemporaryBanned = 9,
	
	EOS_ACCCAR_PermanentBanned = 10
);

EOS_ENUM(EOS_EAntiCheatCommonClientAuthStatus,
	
	EOS_ACCCAS_Invalid = 0,
	
	EOS_ACCCAS_LocalAuthComplete = 1,
	
	EOS_ACCCAS_RemoteAuthComplete = 2
);

EOS_ENUM(EOS_EAntiCheatCommonClientFlags,
	
	EOS_ACCCF_None = 0,
	
	EOS_ACCCF_Admin = (1 << 0)
);
EOS_ENUM_BOOLEAN_OPERATORS(EOS_EAntiCheatCommonClientFlags);

EOS_ENUM(EOS_EAntiCheatCommonClientInput,
	
	EOS_ACCCI_Unknown = 0,
	
	EOS_ACCCI_MouseKeyboard = 1,
	
	EOS_ACCCI_Gamepad = 2,
	
	EOS_ACCCI_TouchInput = 3
);

EOS_ENUM(EOS_EAntiCheatCommonEventType,
	
	EOS_ACCET_Invalid = 0,
	
	EOS_ACCET_GameEvent = 1,
	
	EOS_ACCET_PlayerEvent = 2
);

EOS_ENUM(EOS_EAntiCheatCommonEventParamType,
	
	EOS_ACCEPT_Invalid = 0,
	
	EOS_ACCEPT_ClientHandle = 1,
	
	EOS_ACCEPT_String = 2,
	
	EOS_ACCEPT_UInt32 = 3,
	
	EOS_ACCEPT_Int32 = 4,
	
	EOS_ACCEPT_UInt64 = 5,
	
	EOS_ACCEPT_Int64 = 6,
	
	EOS_ACCEPT_Vector3f = 7,
	
	EOS_ACCEPT_Quat = 8,
	
	EOS_ACCEPT_Float = 9
);

EOS_ENUM(EOS_EAntiCheatCommonGameRoundCompetitionType,
	
	EOS_ACCGRCT_None = 0,
	
	EOS_ACCGRCT_Casual = 1,
	
	EOS_ACCGRCT_Ranked = 2,
	
	EOS_ACCGRCT_Competitive = 3
);

EOS_ENUM(EOS_EAntiCheatCommonPlayerMovementState,
	
	EOS_ACCPMS_None = 0,
	
	EOS_ACCPMS_Crouching = 1,
	
	EOS_ACCPMS_Prone = 2,
	
	EOS_ACCPMS_Mounted = 3,
	
	EOS_ACCPMS_Swimming = 4,
	
	EOS_ACCPMS_Falling = 5,
	
	EOS_ACCPMS_Flying = 6,
	
	EOS_ACCPMS_OnLadder = 7
);

EOS_ENUM(EOS_EAntiCheatCommonPlayerTakeDamageSource,
	
	EOS_ACCPTDS_None = 0,
	
	EOS_ACCPTDS_Player = 1,
	
	EOS_ACCPTDS_NonPlayerCharacter = 2,
	
	EOS_ACCPTDS_World = 3
);

EOS_ENUM(EOS_EAntiCheatCommonPlayerTakeDamageType,
	
	EOS_ACCPTDT_None = 0,
	
	EOS_ACCPTDT_PointDamage = 1,
	
	EOS_ACCPTDT_RadialDamage = 2,
	
	EOS_ACCPTDT_DamageOverTime = 3
);

EOS_ENUM(EOS_EAntiCheatCommonPlayerTakeDamageResult,
	
	EOS_ACCPTDR_None = 0,
	
	EOS_ACCPTDR_Downed_DEPRECATED = 1,
	
	EOS_ACCPTDR_Eliminated_DEPRECATED = 2,
	
	EOS_ACCPTDR_NormalToDowned = 3,
	
	EOS_ACCPTDR_NormalToEliminated = 4,
	
	EOS_ACCPTDR_DownedToEliminated = 5
);

EOS_STRUCT(EOS_AntiCheatCommon_Vec3f, (
	
	float x;
	
	float y;
	
	float z;
));

EOS_STRUCT(EOS_AntiCheatCommon_Quat, (
	
	float w;
	
	float x;
	
	float y;
	
	float z;
));

EOS_STRUCT(EOS_AntiCheatCommon_OnMessageToClientCallbackInfo, (
	
	void* ClientData;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	const void* MessageData;
	
	uint32_t MessageDataSizeBytes;
));

EOS_STRUCT(EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo, (
	
	void* ClientData;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	EOS_EAntiCheatCommonClientAction ClientAction;
	
	EOS_EAntiCheatCommonClientActionReason ActionReasonCode;
	
	const char* ActionReasonDetailsString;
));

EOS_STRUCT(EOS_AntiCheatCommon_OnClientAuthStatusChangedCallbackInfo, (
	
	void* ClientData;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	EOS_EAntiCheatCommonClientAuthStatus ClientAuthStatus;
));

#define EOS_ANTICHEATCOMMON_SETCLIENTDETAILS_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_SetClientDetailsOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	EOS_EAntiCheatCommonClientFlags ClientFlags;
	
	EOS_EAntiCheatCommonClientInput ClientInputMethod;
));

#define EOS_ANTICHEATCOMMON_SETGAMESESSIONID_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_SetGameSessionIdOptions, (
	
	int32_t ApiVersion;
	
	const char* GameSessionId;
));

#define EOS_ANTICHEATCOMMON_REGISTEREVENT_CUSTOMEVENTBASE 0x10000000

#define EOS_ANTICHEATCOMMON_REGISTEREVENT_MAX_PARAMDEFSCOUNT 12

EOS_STRUCT(EOS_AntiCheatCommon_RegisterEventParamDef, (
	
	const char* ParamName;
	
	EOS_EAntiCheatCommonEventParamType ParamType;
));

#define EOS_ANTICHEATCOMMON_REGISTEREVENT_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_RegisterEventOptions, (
	
	int32_t ApiVersion;
	
	uint32_t EventId;
	
	const char* EventName;
	
	EOS_EAntiCheatCommonEventType EventType;
	
	uint32_t ParamDefsCount;
	
	const EOS_AntiCheatCommon_RegisterEventParamDef* ParamDefs;
));

#define EOS_ANTICHEATCOMMON_LOGEVENT_STRING_MAX_LENGTH 39

EOS_STRUCT(EOS_AntiCheatCommon_LogEventParamPair, (
	
	EOS_EAntiCheatCommonEventParamType ParamValueType;
	
	union
	{
		
		EOS_AntiCheatCommon_ClientHandle ClientHandle;
		
		const char* String;
		
		uint32_t UInt32;
		
		int32_t Int32;
		
		uint64_t UInt64;
		
		int64_t Int64;
		
		EOS_AntiCheatCommon_Vec3f Vec3f;
		
		EOS_AntiCheatCommon_Quat Quat;
		
		float Float;
	} ParamValue;
));

#define EOS_ANTICHEATCOMMON_LOGEVENT_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_LogEventOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle ClientHandle;
	
	uint32_t EventId;
	
	uint32_t ParamsCount;
	
	const EOS_AntiCheatCommon_LogEventParamPair* Params;
));

#define EOS_ANTICHEATCOMMON_LOGGAMEROUNDSTART_API_LATEST 2

EOS_STRUCT(EOS_AntiCheatCommon_LogGameRoundStartOptions, (
	
	int32_t ApiVersion;
	
	const char* SessionIdentifier;
	
	const char* LevelName;
	
	const char* ModeName;
	
	uint32_t RoundTimeSeconds;
	
	EOS_EAntiCheatCommonGameRoundCompetitionType CompetitionType;
));

#define EOS_ANTICHEATCOMMON_LOGGAMEROUNDEND_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_LogGameRoundEndOptions, (
	
	int32_t ApiVersion;
	
	uint32_t WinningTeamId;
));

#define EOS_ANTICHEATCOMMON_LOGPLAYERSPAWN_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_LogPlayerSpawnOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle SpawnedPlayerHandle;
	
	uint32_t TeamId;
	
	uint32_t CharacterId;
));

#define EOS_ANTICHEATCOMMON_LOGPLAYERDESPAWN_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_LogPlayerDespawnOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle DespawnedPlayerHandle;
));

#define EOS_ANTICHEATCOMMON_LOGPLAYERREVIVE_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_LogPlayerReviveOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle RevivedPlayerHandle;
	
	EOS_AntiCheatCommon_ClientHandle ReviverPlayerHandle;
));

#define EOS_ANTICHEATCOMMON_LOGPLAYERTICK_API_LATEST 3

EOS_STRUCT(EOS_AntiCheatCommon_LogPlayerTickOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle PlayerHandle;
	
	EOS_AntiCheatCommon_Vec3f* PlayerPosition;
	
	EOS_AntiCheatCommon_Quat* PlayerViewRotation;
	
	EOS_Bool bIsPlayerViewZoomed;
	
	float PlayerHealth;
	
	EOS_EAntiCheatCommonPlayerMovementState PlayerMovementState;
	
	EOS_AntiCheatCommon_Vec3f* PlayerViewPosition;
));

#define EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_WEAPONNAME_MAX_LENGTH 32

EOS_STRUCT(EOS_AntiCheatCommon_LogPlayerUseWeaponData, (
	
	EOS_AntiCheatCommon_ClientHandle PlayerHandle;
	
	EOS_AntiCheatCommon_Vec3f* PlayerPosition;
	
	EOS_AntiCheatCommon_Quat* PlayerViewRotation;
	
	EOS_Bool bIsPlayerViewZoomed;
	
	EOS_Bool bIsMeleeAttack;
	
	const char* WeaponName;
));

#define EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_API_LATEST 2

EOS_STRUCT(EOS_AntiCheatCommon_LogPlayerUseWeaponOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_LogPlayerUseWeaponData* UseWeaponData;
));

#define EOS_ANTICHEATCOMMON_LOGPLAYERUSEABILITY_API_LATEST 1

EOS_STRUCT(EOS_AntiCheatCommon_LogPlayerUseAbilityOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle PlayerHandle;
	
	uint32_t AbilityId;
	
	uint32_t AbilityDurationMs;
	
	uint32_t AbilityCooldownMs;
));

#define EOS_ANTICHEATCOMMON_LOGPLAYERTAKEDAMAGE_API_LATEST 4

EOS_STRUCT(EOS_AntiCheatCommon_LogPlayerTakeDamageOptions, (
	
	int32_t ApiVersion;
	
	EOS_AntiCheatCommon_ClientHandle VictimPlayerHandle;
	
	EOS_AntiCheatCommon_Vec3f* VictimPlayerPosition;
	
	EOS_AntiCheatCommon_Quat* VictimPlayerViewRotation;
	
	EOS_AntiCheatCommon_ClientHandle AttackerPlayerHandle;
	
	EOS_AntiCheatCommon_Vec3f* AttackerPlayerPosition;
	
	EOS_AntiCheatCommon_Quat* AttackerPlayerViewRotation;
	
	EOS_Bool bIsHitscanAttack;
	
	EOS_Bool bHasLineOfSight;
	
	EOS_Bool bIsCriticalHit;
	
	uint32_t HitBoneId_DEPRECATED;
	
	float DamageTaken;
	
	float HealthRemaining;
	
	EOS_EAntiCheatCommonPlayerTakeDamageSource DamageSource;
	
	EOS_EAntiCheatCommonPlayerTakeDamageType DamageType;
	
	EOS_EAntiCheatCommonPlayerTakeDamageResult DamageResult;
	
	EOS_AntiCheatCommon_LogPlayerUseWeaponData* PlayerUseWeaponData;
	
	uint32_t TimeSincePlayerUseWeaponMs;
	
	EOS_AntiCheatCommon_Vec3f* DamagePosition;
	
	EOS_AntiCheatCommon_Vec3f* AttackerPlayerViewPosition;
));

#pragma pack(pop)
