
#include "overlay/OverlayManager.h"
#include "eossdk_platform.h"
#include "settings.h"
#include "utils/Log.h"

#if defined(__WINDOWS__)
#if __has_include("imgui/imgui.h")
    #include "imgui/imgui.h"
    #include "imgui/backends/imgui_impl_dx11.h"
    #include "imgui/backends/imgui_impl_win32.h"
    #define KEE_HAS_IMGUI 1
#else
    #define KEE_HAS_IMGUI 0
#endif
#else
    #define KEE_HAS_IMGUI 0
#endif

void OverlayManager::DrawOverlay()
{
#if KEE_HAS_IMGUI
    if (!m_visible) return;

    ImGuiIO& io = ImGui::GetIO();

    ImGui::PushStyleColor(ImGuiCol_WindowBg,      ImVec4(0.05f, 0.07f, 0.15f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive,  ImVec4(0.10f, 0.40f, 0.90f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Tab,            ImVec4(0.08f, 0.12f, 0.25f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TabActive,      ImVec4(0.10f, 0.40f, 0.90f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TabHovered,     ImVec4(0.15f, 0.50f, 1.00f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Button,         ImVec4(0.10f, 0.35f, 0.80f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  ImVec4(0.15f, 0.50f, 1.00f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Header,         ImVec4(0.10f, 0.35f, 0.80f, 0.60f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered,  ImVec4(0.15f, 0.50f, 1.00f, 0.80f));

    constexpr float W = 640.0f, H = 500.0f;
    ImGui::SetNextWindowPos(
        {io.DisplaySize.x * 0.5f - W * 0.5f, io.DisplaySize.y * 0.5f - H * 0.5f},
        ImGuiCond_Once);
    ImGui::SetNextWindowSize({W, H}, ImGuiCond_Once);
    ImGui::SetNextWindowBgAlpha(0.95f);

    bool open = m_visible;
    if (ImGui::Begin("KEE  |  Epic Games", &open,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        m_visible = open;

        ImGui::TextColored({0.4f, 0.8f, 1.0f, 1.0f},
            "  %s", Settings::Inst().account_name.c_str());
        ImGui::SameLine(W - 160.0f);
        ImGui::TextDisabled("KEE Emulator");
        ImGui::Separator();

        if (ImGui::BeginTabBar("##kee_tabs"))
        {
            
            if (ImGui::BeginTabItem("  Friends  "))
            {
                ImGui::Spacing();

                auto& fi = GetEOS_Friends();
                EOS_Friends_GetFriendsCountOptions cnt_opts{};
                cnt_opts.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
                cnt_opts.LocalUserId = Settings::Inst().userid;
                int32_t count = fi.GetFriendsCount(&cnt_opts);

                if (count == 0)
                {
                    ImGui::TextDisabled("  No friends found.");
                    ImGui::TextDisabled("  Add friends via kee_settings/friends.json");
                }
                else
                {
                    for (int32_t i = 0; i < count; ++i)
                    {
                        EOS_Friends_GetFriendAtIndexOptions idx_opts{};
                        idx_opts.ApiVersion = EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST;
                        idx_opts.LocalUserId = Settings::Inst().userid;
                        idx_opts.Index = i;
                        EOS_EpicAccountId fid = fi.GetFriendAtIndex(&idx_opts);
                        if (!fid) continue;

                        EOS_Friends_GetStatusOptions st_opts{};
                        st_opts.ApiVersion = EOS_FRIENDS_GETSTATUS_API_LATEST;
                        st_opts.LocalUserId  = Settings::Inst().userid;
                        st_opts.TargetUserId = fid;
                        EOS_EFriendsStatus status = fi.GetStatus(&st_opts);

                        ImVec4 col = (status == EOS_EFriendsStatus::EOS_FS_Friends)
                            ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f)
                            : ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

                        ImGui::Bullet();
                        ImGui::SameLine();
                        ImGui::TextColored(col, "%s", fid->to_string().c_str());
                    }
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("  Achievements  "))
            {
                auto& ai = GetEOS_Achievements();

                EOS_Achievements_GetAchievementDefinitionCountOptions tot_opts{};
                tot_opts.ApiVersion = EOS_ACHIEVEMENTS_GETACHIEVEMENTDEFINITIONCOUNT_API_LATEST;
                uint32_t total = ai.GetAchievementDefinitionCount(&tot_opts);

                EOS_Achievements_GetUnlockedAchievementCountOptions ul_opts{};
                ul_opts.ApiVersion = EOS_ACHIEVEMENTS_GETUNLOCKEDACHIEVEMENTCOUNT_API_LATEST;
                ul_opts.UserId = Settings::Inst().productuserid;
                uint32_t unlocked = ai.GetUnlockedAchievementCount(&ul_opts);

                ImGui::Spacing();
                ImGui::Text("Unlocked: %u / %u", unlocked, total);
                ImGui::Separator();
                ImGui::Spacing();

                for (uint32_t i = 0; i < unlocked; ++i)
                {
                    EOS_Achievements_CopyUnlockedAchievementByIndexOptions idx{};
                    idx.ApiVersion = EOS_ACHIEVEMENTS_COPYUNLOCKEDACHIEVEMENTBYINDEX_API_LATEST;
                    idx.UserId = Settings::Inst().productuserid;
                    idx.AchievementIndex = i;
                    EOS_Achievements_UnlockedAchievement* ach = nullptr;
                    if (ai.CopyUnlockedAchievementByIndex(&idx, &ach) == EOS_EResult::EOS_Success && ach)
                    {
                        ImGui::TextColored({1.0f, 0.85f, 0.0f, 1.0f},
                            " [%s]", ach->AchievementId ? ach->AchievementId : "?");
                        EOS_Achievements_UnlockedAchievement_Release(ach);
                    }
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("  Info  "))
            {
                ImGui::Spacing();
                ImGui::Text("Account Name  : %s", Settings::Inst().account_name.c_str());
                ImGui::Text("Epic Account  : %s", Settings::Inst().userid_str.c_str());
                ImGui::Text("Product User  : %s", Settings::Inst().productuserid_str.c_str());
                ImGui::Text("Product ID    : %s", Settings::Inst().appid.c_str());
                ImGui::Separator();
                ImGui::Text("Connected peers: %zu", GetEOS_Connect().m_users.size() > 0 ? GetEOS_Connect().m_users.size() - 1 : 0);
                ImGui::Spacing();
                ImGui::TextDisabled("Toggle: Shift + F3");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
    ImGui::PopStyleColor(9);

#else
    static bool warned = false;
    if (!warned && m_visible)
    {
        APP_LOG(Log::LogLevel::INFO,
            "[Overlay] ImGui not present — overlay panel disabled. "
            "git submodule add https://github.com/ocornut/imgui.git third_party/imgui");
        warned = true;
    }
#endif
}

void OverlayManager::DrawNotifications()
{
    std::lock_guard<std::mutex> lock(m_notif_mutex);
    if (m_notifications.empty()) return;

#if KEE_HAS_IMGUI
    ImGuiIO& io = ImGui::GetIO();
    float y = 20.0f;
    int   id = 0;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.15f, 0.05f, 0.88f));

    for (auto& n : m_notifications)
    {
        ImGui::SetNextWindowPos({io.DisplaySize.x - 340.0f, y});
        ImGui::SetNextWindowSize({320.0f, 64.0f});
        ImGui::SetNextWindowBgAlpha(0.88f);

        char wid[32];
        snprintf(wid, sizeof(wid), "##notif_%d", id++);

        ImGui::Begin(wid, nullptr,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoMove       | ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBringToDisplayFront | ImGuiWindowFlags_NoNav);

        if (n.is_achievement)
            ImGui::TextColored({1.0f, 0.85f, 0.0f, 1.0f}, "Achievement Unlocked!");
        else
            ImGui::TextColored({0.4f, 0.8f, 1.0f, 1.0f}, "%s", n.title.c_str());

        ImGui::TextWrapped("%s", n.message.c_str());
        ImGui::End();

        y += 72.0f;
    }
    ImGui::PopStyleColor(1);
#endif
}
