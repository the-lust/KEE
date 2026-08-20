// EGS Client exports — return real data from the config system.
// These are the 62 functions the EGS launcher calls to get account info,
// identity, paths, entitlements, etc.

#include <eos_common.h>
#include <cstring>
#include <string>

#include "config/settings.h"

static const char* copy_str(const std::string& s)
{
    static thread_local std::string cache;
    cache = s;
    return cache.c_str();
}

extern "C" {

int EGS_Client_Initialize()
{
    Settings::Inst().load_settings();
    return 0;
}

void EGS_Client_Shutdown() {}

const char* EGS_Client_GetVersion()
{
    return "kee 2.1.0 (EOS SDK Emulator)";
}

void EGS_Client_SetConfig() {}
void EGS_Client_GetConfig() {}

int EGS_Client_Authenticate()
{
    return 0;
}

void EGS_Client_GetAuthToken() {}
void EGS_Client_RefreshToken() {}

const char* EGS_Client_GetAccountID()
{
    return copy_str(Settings::Inst().account_id);
}

const char* EGS_Client_GetDisplayName()
{
    return copy_str(Settings::Inst().account_name);
}

const char* EGS_Client_GetCountryCode()
{
    return copy_str(Settings::Inst().country_code);
}

const char* EGS_Client_GetLanguageCode()
{
    return copy_str(Settings::Inst().language);
}

void EGS_Client_GetEntitlements() {}

int EGS_Client_CheckEntitlement()
{
    return 1;
}

void EGS_Client_GetCatalogItems() {}
void EGS_Client_GetOwnershipCache() {}

int EGS_Client_GetInstallationStatus()
{
    return 0;
}

const char* EGS_Client_GetGameVersion()
{
    return "1.0.0.0";
}

void EGS_Client_GetGameSettings() {}

void EGS_Client_OpenOverlay() {}
void EGS_Client_OpenStore() {}
void EGS_Client_OpenFriends() {}
void EGS_Client_OpenSettings() {}

void EGS_Client_GetFriendList() {}
void EGS_Client_GetPresence() {}
void EGS_Client_SetPresence() {}
void EGS_Client_GetAchievements() {}
void EGS_Client_UnlockAchievement() {}
void EGS_Client_GetLeaderboardScores() {}
void EGS_Client_SubmitScore() {}
void EGS_Client_GetStats() {}
void EGS_Client_SetStat() {}
void EGS_Client_GetCloudSaveFiles() {}
void EGS_Client_OpenCloudSaveFolder() {}

const char* EGS_Client_GetUserDataPath()
{
    return copy_str(Settings::Inst().save_path);
}

const char* EGS_Client_GetInstallPath()
{
    return ".";
}

int EGS_Client_IsDLCEnabled()
{
    return Settings::Inst().unlock_dlcs ? 1 : 0;
}

void EGS_Client_SetDLCEnabled() {}

const char* EGS_Client_GetBuildId()
{
    return "1";
}

const char* EGS_Client_GetPlatformId()
{
    return "Windows";
}

const char* EGS_Client_GetSandboxId()
{
    return copy_str(Settings::Inst().sandbox_id);
}

const char* EGS_Client_GetDeploymentId()
{
    return copy_str(Settings::Inst().deployment_id);
}

void EGS_Client_StartPurchase() {}
void EGS_Client_FinalizePurchase() {}
void EGS_Client_GetTransactions() {}
void EGS_Client_GetTransaction() {}

int EGS_Client_GetEntitlementsCount()
{
    return 0;
}

void EGS_Client_GetEntitlementByName() {}
void EGS_Client_GetEntitlementById() {}
void EGS_Client_GetItemById() {}
void EGS_Client_GetItemByIndex() {}

int EGS_Client_GetItemsCount()
{
    return 0;
}

void EGS_Client_CreateArtifactCategory() {}
void EGS_Client_GetArtifactCategory() {}

int EGS_Client_GetNumArtifactCategories()
{
    return 0;
}

void EGS_Client_GetArtifactCategoryByIndex() {}
void EGS_Client_LibraryEnumerate() {}
void EGS_Client_LibraryGetItem() {}
void EGS_Client_LibraryInstallItem() {}
void EGS_Client_LibraryUninstallItem() {}
void EGS_Client_LibraryLaunchItem() {}

int EGS_Client_LibraryGetItemProgress()
{
    return 100;
}

}
