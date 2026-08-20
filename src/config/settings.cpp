#include "settings.h"
#include "mini/ini.h"
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <shellapi.h>

#include <eos_common.h>

// ── Smart ID generation (ported from psfree/eos_sdk_emu) ───────────────────
// Generates a deterministic EpicAccountId from a username string.
// Generates a deterministic ProductUserId from (appid + epicid).
static string generate_id_from_name(const string& name, uint64_t seed_offset)
{
    union {
        struct { uint64_t part1; uint64_t part2; };
        uint8_t id[16];
    } buf;
    buf.part1 = 0x0000001201030307 + seed_offset;
    buf.part2 = 0x0000001201030307 + seed_offset;

    for (size_t i = 0; i < name.size(); ++i)
    {
        buf.id[i % 16] ^= (uint8_t)(name[i] + (i * 31));
        buf.id[15 - (i % 16)] ^= (uint8_t)(name[i] + (i * 17));
    }

    // Avoid all-zero IDs
    bool all_zero = true;
    for (int i = 0; i < 16 && all_zero; ++i)
        if (buf.id[i] != 0) all_zero = false;
    if (all_zero) buf.id[0] = 0x42;

    std::stringstream sstr;
    sstr << std::hex << std::setfill('0') << std::setw(16) << buf.part2
         << std::setw(16) << buf.part1;
    return sstr.str();
}

static string generate_account_id_from_name(const string& name)
{
    return generate_id_from_name(name, 0);
}

static string generate_product_id_from_name(const string& raw)
{
    return generate_id_from_name(raw, 0x1000);
}

Settings::Settings()
{
    load_settings();
}

Settings::~Settings() {}

Settings& Settings::Inst()
{
    static Settings inst;
    return inst;
}

void Settings::load_text_file(const string& filename, string& out_value)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        string line;
        if (std::getline(file, line))
        {
            if (line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF)
                line = line.substr(3);
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (!line.empty())
                out_value = line;
        }
    }
}

void Settings::load_config_main(const string& path)
{
    mINI::INIFile file(path);
    mINI::INIStructure ini;
    if (!file.read(ini)) return;

    if (ini.has("main"))
    {
        auto& s = ini["main"];
        if (s.has("save_path"))                    save_path = s["save_path"];
        if (s.has("enable_logging"))               enable_logging = (s["enable_logging"] != "0");
        if (s.has("log_level"))                    log_level = s["log_level"];
        if (s.has("unlock_dlcs"))                  unlock_dlcs = (s["unlock_dlcs"] != "0");
        if (s.has("unlock_entitlements"))          unlock_entitlements = (s["unlock_entitlements"] != "0");
        if (s.has("disable_online_networking"))   disable_online_networking = (s["disable_online_networking"] != "0");
        if (s.has("enable_overlay"))               enable_overlay = (s["enable_overlay"] != "0");
        if (s.has("enable_overlay_notification_sounds")) enable_overlay_notification_sounds = (s["enable_overlay_notification_sounds"] != "0");
        if (s.has("force_lan"))                    force_lan = (s["force_lan"] != "0");
        if (s.has("allow_unknown_stats"))          allow_unknown_stats = (s["allow_unknown_stats"] != "0");
        if (s.has("allow_unknown_achievements"))   allow_unknown_achievements = (s["allow_unknown_achievements"] != "0");
    }

    if (ini.has("network"))
    {
        auto& s = ini["network"];
        if (s.has("p2p_port_min"))                p2p_port_min = (uint16_t)std::stoul(s["p2p_port_min"]);
        if (s.has("p2p_port_max"))                p2p_port_max = (uint16_t)std::stoul(s["p2p_port_max"]);
        if (s.has("broadcast_port"))              broadcast_port = (uint16_t)std::stoul(s["broadcast_port"]);
        if (s.has("advertise_rate_ms"))           advertise_rate_ms = (uint32_t)std::stoul(s["advertise_rate_ms"]);
        if (s.has("relay_control"))               relay_control = std::stoi(s["relay_control"]);
        if (s.has("broadcast_address") && !s["broadcast_address"].empty())
            broadcast_address = s["broadcast_address"];
    }
}

void Settings::load_config_user(const string& path)
{
    mINI::INIFile file(path);
    mINI::INIStructure ini;
    if (!file.read(ini)) return;

    if (ini.has("user"))
    {
        auto& s = ini["user"];
        if (s.has("epic_account_id") && !s["epic_account_id"].empty()) account_id = s["epic_account_id"];
        if (s.has("product_user_id") && !s["product_user_id"].empty()) product_user_id = s["product_user_id"];
        if (s.has("display_name"))   account_name = s["display_name"];
        if (s.has("language"))       language = s["language"];
        if (s.has("country_code"))   country_code = s["country_code"];
    }

    if (ini.has("online"))
    {
        auto& s = ini["online"];
        if (s.has("exchange_code"))  exchange_code = s["exchange_code"];
        if (s.has("access_token"))   access_token = s["access_token"];
        if (s.has("refresh_token"))  refresh_token = s["refresh_token"];
        if (s.has("id_token"))       id_token = s["id_token"];
        if (s.has("auth_password"))  auth_password = s["auth_password"];
    }
}

void Settings::load_config_app(const string& path)
{
    mINI::INIFile file(path);
    mINI::INIStructure ini;
    if (!file.read(ini)) return;

    if (ini.has("app"))
    {
        auto& s = ini["app"];
        if (s.has("app_name"))          gamename = s["app_name"];
        if (s.has("app_id"))            app_id = s["app_id"];
        if (s.has("deployment_id"))     deployment_id = s["deployment_id"];
        if (s.has("sandbox_id"))        sandbox_id = s["sandbox_id"];
        if (s.has("unlock_all_dlcs"))   unlock_dlcs = (s["unlock_all_dlcs"] != "0");
        if (s.has("unlock_all_entitlements")) unlock_entitlements = (s["unlock_all_entitlements"] != "0");
        if (s.has("achievements_file")) achievements_file = s["achievements_file"];
        if (s.has("stats_file"))        stats_file = s["stats_file"];
        if (s.has("leaderboards_file")) leaderboards_file = s["leaderboards_file"];
    }
}

void Settings::generate_smart_ids()
{
    // Auto-generate epic account ID from username if empty
    if (account_id.empty())
    {
        string id_src = account_name;
        std::transform(id_src.begin(), id_src.end(), id_src.begin(), ::tolower);
        account_id = generate_account_id_from_name(id_src);
    }

    // Auto-generate product user ID from (appid + epic_id) if empty
    if (product_user_id.empty())
    {
        string id_src = gamename + account_id;
        product_user_id = generate_product_id_from_name(id_src);
    }
}

void Settings::auto_generate_configs()
{
    // Create main config if missing
    string main_path = kee_settings_path + "/configs.main.ini";
    if (!std::filesystem::exists(main_path))
    {
        mINI::INIFile file(main_path);
        mINI::INIStructure ini;
        ini["main"]["save_path"] = "./saves";
        ini["main"]["enable_logging"] = "1";
        ini["main"]["log_level"] = "info";
        ini["main"]["unlock_dlcs"] = "1";
        ini["main"]["unlock_entitlements"] = "1";
        ini["main"]["disable_online_networking"] = "0";
        ini["main"]["enable_overlay"] = "1";
        ini["main"]["enable_overlay_notification_sounds"] = "0";
        ini["main"]["force_lan"] = "1";
        ini["main"]["allow_unknown_stats"] = "1";
        ini["main"]["allow_unknown_achievements"] = "1";
        ini["network"]["p2p_port_min"] = "7777";
        ini["network"]["p2p_port_max"] = "7877";
        ini["network"]["broadcast_port"] = "55789";
        ini["network"]["advertise_rate_ms"] = "2000";
        ini["network"]["relay_control"] = "0";
        file.generate(ini);
    }

    // Create user config if missing
    string user_path = kee_settings_path + "/configs.user.ini";
    if (!std::filesystem::exists(user_path))
    {
        mINI::INIFile file(user_path);
        mINI::INIStructure ini;
        ini["user"]["epic_account_id"] = account_id;
        ini["user"]["product_user_id"] = product_user_id;
        ini["user"]["display_name"] = account_name;
        ini["user"]["language"] = language;
        ini["user"]["country_code"] = country_code;
        ini["online"]["exchange_code"] = "auto";
        ini["online"]["access_token"] = "auto";
        ini["online"]["refresh_token"] = "auto";
        file.generate(ini);
    }

    // Create app config if missing
    string app_path = kee_settings_path + "/configs.app.ini";
    if (!std::filesystem::exists(app_path))
    {
        mINI::INIFile file(app_path);
        mINI::INIStructure ini;
        ini["app"]["app_name"] = gamename;
        ini["app"]["app_id"] = app_id.empty() ? gamename : app_id;
        ini["app"]["unlock_all_dlcs"] = "1";
        ini["app"]["unlock_all_entitlements"] = "1";
        ini["app"]["achievements_file"] = "achievements.json";
        ini["app"]["stats_file"] = "stats.json";
        file.generate(ini);
    }
}

void Settings::load_settings()
{
    // Defaults
    account_name    = "SAI_User";
    language        = "en";
    country_code    = "US";
    save_path       = "./saves";
    kee_settings_path = "kee_settings";
    enable_logging  = true;
    log_level       = "info";
    unlock_dlcs     = true;
    unlock_entitlements = true;
    disable_online_networking = false;
    enable_overlay  = true;
    gamename        = "DefaultGameName";
    p2p_port_min    = 7777;
    p2p_port_max    = 7877;
    relay_control   = 0;
    broadcast_address = "255.255.255.255";

    // Ensure config directory exists
    if (!std::filesystem::exists(kee_settings_path))
        std::filesystem::create_directories(kee_settings_path);

    // Legacy text file support (backward compat)
    load_text_file(kee_settings_path + "/account_name.txt", account_name);
    load_text_file(kee_settings_path + "/epic_id.txt", account_id);
    load_text_file(kee_settings_path + "/product_user_id.txt", product_user_id);
    load_text_file(kee_settings_path + "/language.txt", language);

    // Legacy single-file INI support
    string legacy_ini = kee_settings_path + "/kee_settings.ini";
    if (std::filesystem::exists(legacy_ini))
    {
        mINI::INIFile file(legacy_ini);
        mINI::INIStructure ini;
        if (file.read(ini))
        {
            if (ini.has("Account"))
            {
                if (ini["Account"].has("DisplayName")) account_name = ini["Account"]["DisplayName"];
                if (ini["Account"].has("EpicAccountID") && !ini["Account"]["EpicAccountID"].empty() && ini["Account"]["EpicAccountID"] != "00000000000000000000000000000000")
                    account_id = ini["Account"]["EpicAccountID"];
                if (ini["Account"].has("ProductUserID") && !ini["Account"]["ProductUserID"].empty() && ini["Account"]["ProductUserID"] != "00000000000000000000000000000001")
                    product_user_id = ini["Account"]["ProductUserID"];
            }
            if (ini.has("Online"))
            {
                if (ini["Online"].has("ExchangeCode")) exchange_code = ini["Online"]["ExchangeCode"];
                if (ini["Online"].has("AccessToken")) access_token = ini["Online"]["AccessToken"];
                if (ini["Online"].has("IdToken")) id_token = ini["Online"]["IdToken"];
                if (ini["Online"].has("AuthPassword")) auth_password = ini["Online"]["AuthPassword"];
            }
            if (ini.has("System"))
            {
                if (ini["System"].has("Language")) language = ini["System"]["Language"];
                if (ini["System"].has("SavePath")) save_path = ini["System"]["SavePath"];
                if (ini["System"].has("Logging")) enable_logging = (ini["System"]["Logging"] != "0");
                if (ini["System"].has("UnlockDLCs")) unlock_dlcs = (ini["System"]["UnlockDLCs"] != "0");
                if (ini["System"].has("DisableOnlineNetworking")) disable_online_networking = (ini["System"]["DisableOnlineNetworking"] != "0");
            }
            if (ini.has("Network") && ini["Network"].has("BroadcastAddress") && !ini["Network"]["BroadcastAddress"].empty())
                broadcast_address = ini["Network"]["BroadcastAddress"];
        }
    }

    // New 3-file config (overrides legacy)
    load_config_main(kee_settings_path + "/configs.main.ini");
    load_config_user(kee_settings_path + "/configs.user.ini");
    load_config_app(kee_settings_path + "/configs.app.ini");

    // Steam settings backward compat
    string steam_settings = "steam_settings";
    if (std::filesystem::exists(steam_settings))
    {
        if (account_name == "SAI_User") load_text_file(steam_settings + "/account_name.txt", account_name);
        if (language == "en") load_text_file(steam_settings + "/language.txt", language);
    }

    // Smart ID generation (fill in empty IDs deterministically)
    generate_smart_ids();

    // Parse command line for Epic auth args
    parse_args(__argc, __argv);

    // Re-parse command line args via Windows API for wider compat
    int argc;
    LPWSTR* argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argvW)
    {
        std::vector<string> args;
        for (int i = 0; i < argc; ++i)
        {
            int size = WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, nullptr, 0, nullptr, nullptr);
            string arg(size, 0);
            WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, &arg[0], size, nullptr, nullptr);
            if (!arg.empty() && arg.back() == '\0') arg.pop_back();
            args.push_back(arg);
        }
        LocalFree(argvW);

        std::vector<char*> argv;
        for (auto& s : args) argv.push_back(&s[0]);
        parse_args((int)argv.size(), argv.data());
    }

    // Re-generate IDs after command-line override
    generate_smart_ids();

    // Build EOS handles
    userid        = EOS_EpicAccountId_FromString(account_id.c_str());
    productuserid = EOS_ProductUserId_FromString(product_user_id.c_str());
    userid_str        = account_id;
    productuserid_str = product_user_id;

    // Auto-generate missing config files with current values
    auto_generate_configs();

    // Ensure save directory exists
    if (!std::filesystem::exists(save_path))
        std::filesystem::create_directories(save_path);
}

void Settings::save_settings()
{
    string main_path = kee_settings_path + "/configs.main.ini";
    mINI::INIFile file(main_path);
    mINI::INIStructure ini;
    file.read(ini);

    ini["main"]["save_path"] = save_path;
    ini["main"]["enable_logging"] = enable_logging ? "1" : "0";
    ini["main"]["log_level"] = log_level;
    ini["main"]["unlock_dlcs"] = unlock_dlcs ? "1" : "0";
    ini["main"]["unlock_entitlements"] = unlock_entitlements ? "1" : "0";
    ini["main"]["disable_online_networking"] = disable_online_networking ? "1" : "0";

    string user_path = kee_settings_path + "/configs.user.ini";
    mINI::INIFile ufile(user_path);
    mINI::INIStructure uini;
    ufile.read(uini);
    uini["user"]["epic_account_id"] = account_id;
    uini["user"]["product_user_id"] = product_user_id;
    uini["user"]["display_name"] = account_name;
    uini["user"]["language"] = language;
    uini["user"]["country_code"] = country_code;
    ufile.write(uini);

    file.write(ini);
}

void Settings::parse_args(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if (arg.find("-EpicAccountID=") == 0) account_id = arg.substr(15);
        else if (arg.find("-ProductUserID=") == 0) product_user_id = arg.substr(15);
        else if (arg.find("-DisplayName=") == 0) account_name = arg.substr(13);
        else if (arg.find("-Language=") == 0) language = arg.substr(10);
        else if (arg.find("-SavePath=") == 0) save_path = arg.substr(10);
        else if (arg.find("-AUTH_LOGIN=") == 0) account_id = arg.substr(12);
        else if (arg.find("-AUTH_PASSWORD=") == 0) exchange_code = arg.substr(15);
        else if (arg.find("-AUTH_TYPE=") == 0) { /* auth type determined by launcher */ }
        else if (arg.find("-epicapp=") == 0) gamename = arg.substr(9);
        else if (arg.find("-epicenv=") == 0) { /* environment */ }
        else if (arg.find("-EpicPortal") == 0) { }
        else if (arg.find("-epicusername=") == 0) account_name = arg.substr(14);
        else if (arg.find("-epicuserid=") == 0) account_id = arg.substr(12);
        else if (arg.find("-epiclocale=") == 0) language = arg.substr(12);
        else if (arg.find("-epicsandboxid=") == 0) sandbox_id = arg.substr(15);
        else if (arg.find("-epicdeploymentid=") == 0) deployment_id = arg.substr(18);
    }
}

EOS_EpicAccountId Settings::GetEpicIdHandle()
{
    return EOS_EpicAccountId_FromString(account_id.c_str());
}

EOS_ProductUserId Settings::GetProductIdHandle()
{
    return EOS_ProductUserId_FromString(product_user_id.c_str());
}
