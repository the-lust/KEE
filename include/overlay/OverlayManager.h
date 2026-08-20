#pragma once
#include "common_includes.h"
#include <vector>
#include <string>
#include <mutex>

namespace sdk {

struct Notification {
    string title;
    string message;
    string icon_path;
    float duration;
    float elapsed;
    bool is_achievement;
};

class OverlayManager {
public:
    static OverlayManager& Inst();

    void Init();
    void Shutdown();
    void InstallHooks();

    void OnPresent();
    void OnResize(uint32_t width, uint32_t height);

    void PushNotification(const string& title, const string& message, const string& icon = "", bool achievement = false);

    void ToggleOverlay();
    bool IsVisible() const { return m_visible; }

private:
    OverlayManager();
    ~OverlayManager();

    bool m_visible;
    bool m_initialized;
    uint32_t m_width, m_height;

    std::vector<Notification> m_notifications;
    std::mutex m_notif_mutex;

    void DrawOverlay();
    void DrawNotifications();
};

} 
