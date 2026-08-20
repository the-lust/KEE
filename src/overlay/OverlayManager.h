#pragma once
#include <string>
#include <deque>
#include <mutex>

struct Notification {
    std::string title;
    std::string message;
    std::string icon_path;
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

    void ShowNotification(const std::string& text, int duration_seconds = 5);
    void PushNotification(const std::string& title, const std::string& message, const std::string& icon = "", bool achievement = false);

    void TogglePanel();
    void ToggleOverlay();
    bool WantKeyboardCapture();
    bool WantMouseCapture();
    bool IsVisible() const { return m_visible; }

    friend OverlayManager& GetOverlayManager();

private:
    OverlayManager();
    ~OverlayManager();

    bool m_visible = false;
    bool m_initialized = false;
    uint32_t m_width = 0, m_height = 0;

    std::deque<Notification> m_notifications;
    std::mutex m_notif_mutex;

    void DrawOverlay();
    void DrawNotifications();
};

OverlayManager& GetOverlayManager();
