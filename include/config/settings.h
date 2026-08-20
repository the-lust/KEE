#pragma once
#include <string>
#include <vector>

#include "utils/common_includes.h"

// ── gbe_fork-style 3-file config system ─────────────────────────────────────
// Config files (in kee_settings/):
//   configs.main.ini  → emulator behavior (network, overlay, debug)
//   configs.user.ini  → user identity (account, display, language)
//   configs.app.ini   → per-game overrides (app name, DLC, entitlements)

class Settings
{
    Settings();
    Settings(Settings const&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings const&) = delete;
    Settings& operator=(Settings&&) = delete;

public:
    static Settings& Inst();
    ~Settings();

    void load_settings();
    void save_settings();
    void parse_args(int argc, char** argv);

    // ── User identity (configs.user.ini [user]) ──────────────────────────
    string account_name{"SAI_User"};
    string account_id;         // auto-generated from name if empty
    string product_user_id;    // auto-generated from name+app if empty
    string language{"en"};
    string country_code{"US"};

    // ── Online tokens (configs.user.ini [online]) ────────────────────────
    string exchange_code;
    string access_token;
    string refresh_token;
    string id_token;
    string auth_password;

    // ── Emulator behavior (configs.main.ini [main]) ──────────────────────
    string save_path{"./saves"};
    string kee_settings_path{"kee_settings"};
    bool   enable_logging{true};
    string log_level{"info"};
    bool   unlock_dlcs{true};
    bool   unlock_entitlements{true};
    bool   disable_online_networking{false};
    bool   enable_overlay{true};
    bool   enable_overlay_notification_sounds{false};
    bool   force_lan{true};
    bool   allow_unknown_stats{true};
    bool   allow_unknown_achievements{true};

    // ── Network (configs.main.ini [network]) ─────────────────────────────
    uint16_t p2p_port_min{7777};
    uint16_t p2p_port_max{7877};
    uint16_t broadcast_port{55789};
    uint32_t advertise_rate_ms{2000};
    int32_t  relay_control{0};  // 0=AllowRelays, 1=ForceRelays, 2=NoRelays
    string   broadcast_address{"255.255.255.255"};

    // ── Per-app (configs.app.ini [app]) ──────────────────────────────────
    string gamename{"DefaultGameName"};
    string app_id;
    string deployment_id;
    string sandbox_id;
    string achievements_file{"achievements.json"};
    string stats_file{"stats.json"};
    string leaderboards_file{"leaderboards.txt"};

    // ── Derived / cached ─────────────────────────────────────────────────
    string userid_str;
    string productuserid_str;
    EOS_EpicAccountId userid{nullptr};
    EOS_ProductUserId productuserid{nullptr};

    // Compatibility aliases
    string& username    = account_name;
    string& appid       = gamename;

    const string& get_broadcast_address() const { return broadcast_address; }

    EOS_EpicAccountId GetEpicIdHandle();
    EOS_ProductUserId GetProductIdHandle();

private:
    void load_text_file(const string& filename, string& out_value);
    void load_config_main(const string& path);
    void load_config_user(const string& path);
    void load_config_app(const string& path);
    void auto_generate_configs();
    void generate_smart_ids();
};