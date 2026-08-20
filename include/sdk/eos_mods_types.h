
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_ModsHandle* EOS_HMods;

#define EOS_MOD_IDENTIFIER_API_LATEST 1

EOS_STRUCT(EOS_Mod_Identifier, (
	
	int32_t ApiVersion;
	
	const char* NamespaceId;
	
	const char* ItemId;
	
	const char* ArtifactId;
	
	const char* Title;
	
	const char* Version;
));

#define EOS_MODS_INSTALLMOD_API_LATEST 1

EOS_STRUCT(EOS_Mods_InstallModOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	const EOS_Mod_Identifier* Mod;
	
	EOS_Bool bRemoveAfterExit;
));

EOS_STRUCT(EOS_Mods_InstallModCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	EOS_EpicAccountId LocalUserId;
	
	void* ClientData;
	
	const EOS_Mod_Identifier* Mod;
));

EOS_DECLARE_CALLBACK(EOS_Mods_OnInstallModCallback, const EOS_Mods_InstallModCallbackInfo* Data);

#define EOS_MODS_UNINSTALLMOD_API_LATEST 1

EOS_STRUCT(EOS_Mods_UninstallModOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	const EOS_Mod_Identifier* Mod;
));

EOS_STRUCT(EOS_Mods_UninstallModCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	EOS_EpicAccountId LocalUserId;
	
	void* ClientData;
	
	const EOS_Mod_Identifier* Mod;
));

EOS_DECLARE_CALLBACK(EOS_Mods_OnUninstallModCallback, const EOS_Mods_UninstallModCallbackInfo* Data);

EOS_ENUM(EOS_EModEnumerationType,
	
	EOS_MET_INSTALLED = 0,
	
	EOS_MET_ALL_AVAILABLE
);

#define EOS_MODS_ENUMERATEMODS_API_LATEST 1

EOS_STRUCT(EOS_Mods_EnumerateModsOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EModEnumerationType Type;
));

EOS_STRUCT(EOS_Mods_EnumerateModsCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	EOS_EpicAccountId LocalUserId;
	
	void* ClientData;
	
	EOS_EModEnumerationType Type;
));

EOS_DECLARE_CALLBACK(EOS_Mods_OnEnumerateModsCallback, const EOS_Mods_EnumerateModsCallbackInfo* Data);

#define EOS_MODS_COPYMODINFO_API_LATEST 1

EOS_STRUCT(EOS_Mods_CopyModInfoOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	EOS_EModEnumerationType Type;
));

#define EOS_MODS_MODINFO_API_LATEST 1

EOS_STRUCT(EOS_Mods_ModInfo, (
	
	int32_t ApiVersion;
	
	int32_t ModsCount;
	
	EOS_Mod_Identifier* Mods;
	
	EOS_EModEnumerationType Type;
));

EOS_DECLARE_FUNC(void) EOS_Mods_ModInfo_Release(EOS_Mods_ModInfo* ModInfo);

#define EOS_MODS_UPDATEMOD_API_LATEST 1

EOS_STRUCT(EOS_Mods_UpdateModOptions, (
	
	int32_t ApiVersion;
	
	EOS_EpicAccountId LocalUserId;
	
	const EOS_Mod_Identifier* Mod;
));

EOS_STRUCT(EOS_Mods_UpdateModCallbackInfo, (
	
	EOS_EResult ResultCode;
	
	EOS_EpicAccountId LocalUserId;
	
	void* ClientData;
	
	const EOS_Mod_Identifier* Mod;
));

EOS_DECLARE_CALLBACK(EOS_Mods_OnUpdateModCallback, const EOS_Mods_UpdateModCallbackInfo* Data);

#pragma pack(pop)