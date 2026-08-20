#include "overlay/OverlayManager.h"
#include "utils/common_includes.h"
#include "eossdk_platform.h"
#include "settings.h"

#if defined(__WINDOWS__)
#if __has_include("imgui/imgui.h")
    #include "imgui/imgui.h"
    #define KEE_OVERLAY_HAS_IMGUI 1
#else
    #define KEE_OVERLAY_HAS_IMGUI 0
#endif
#else
    #define KEE_OVERLAY_HAS_IMGUI 0
#endif

OverlayManager::OverlayManager()
    : m_visible(false)
    , m_initialized(false)
    , m_width(0)
    , m_height(0)
{
}

OverlayManager::~OverlayManager()
{
}

OverlayManager& OverlayManager::Inst()
{
    static OverlayManager inst;
    return inst;
}

void OverlayManager::Init()
{
    if (m_initialized) return;
    m_initialized = true;
    APP_LOG(Log::LogLevel::INFO, "KEE Overlay initialised");
}

void OverlayManager::Shutdown()
{
    m_initialized = false;
}

void OverlayManager::InstallHooks()
{
}

void OverlayManager::OnPresent()
{
    if (!m_initialized) return;
    DrawNotifications();
    DrawOverlay();
}

void OverlayManager::OnResize(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;
}

void OverlayManager::ShowNotification(const string& text, int duration_seconds)
{
    PushNotification(text, "", "");
}

void OverlayManager::PushNotification(const string& title, const string& message, const string& icon, bool achievement)
{
    APP_LOG(Log::LogLevel::INFO, "Overlay notify: %s", title.c_str());
    std::lock_guard<std::mutex> lk(m_notif_mutex);

    Notification n;
    n.title = title;
    n.message = message;
    n.icon_path = icon;
    n.duration = static_cast<float>(5);
    n.elapsed = 0.0f;
    n.is_achievement = achievement;
    m_notifications.push_back(n);

    while (m_notifications.size() > 5)
        m_notifications.pop_front();
}

void OverlayManager::TogglePanel()
{
    m_visible = !m_visible;
}

void OverlayManager::ToggleOverlay()
{
    m_visible = !m_visible;
}

bool OverlayManager::WantKeyboardCapture()
{
#if KEE_OVERLAY_HAS_IMGUI
    return m_visible && ImGui::GetIO().WantCaptureKeyboard;
#else
    return false;
#endif
}

bool OverlayManager::WantMouseCapture()
{
#if KEE_OVERLAY_HAS_IMGUI
    return m_visible && ImGui::GetIO().WantCaptureMouse;
#else
    return false;
#endif
}

OverlayManager& GetOverlayManager()
{
    return OverlayManager::Inst();
}
