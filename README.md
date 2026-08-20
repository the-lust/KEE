# S-EGS Emulator — The Ultra Modern EOS/EGS Core

A lightweight, high-performance, and completely launcher-independent replacement for the Epic Online Services (EOS) SDK. Designed with a focus on "Perfect Emulation," this core allows games to run without the Epic Games Launcher, an internet connection, or an active account.

---

## ⚡ Core Philosophy

- **Zero Boilerplate**: Optimized using a sharp, modern C++ architecture with universal `se` (sdk) namespace aliasing.
- **Launcher Independence**: Forcibly bypasses launcher checks and Integrated Platform requirements.
- **Total Parity**: Implements all critical EOS services (Auth, Connect, Ecom, Lobby, etc.) with functional logic, not just empty stubs.
- **Human-Written Quality**: The codebase is refactored for maximum readability and maintainability, featuring sharp, declarative logic flows.

---

## 🚀 Feature Matrix

| Service | Emulation Level | Notes |
| :--- | :--- | :--- |
| **Auth / Identity** | ✅ Full | Dynamic JWT generation, offline identity spoofing. |
| **Connect** | ✅ Full | Automatic device ID management and PUID mapping. |
| **Ecom / DLC** | ✅ Full | Automatic ownership granting and catalog querying. |
| **Lobby & Sessions** | ✅ Full | UDP-based LAN multiplayer and peer discovery. |
| **Achievements** | ✅ Full | Persistence to disk; compatible with Goldberg formats. |
| **Stats & Leaderboards**| ✅ Full | Local persistence and score tracking. |
| **Overlay** | ✅ Full | Integrated social overlay and notification system (DX11/DX12). |
| **P2P Networking** | ✅ Full | Reliable UDP transport for local multiplayer. |
| **RTC & Voice** | ✅ Stubbed | Full room management; audio transport bypassed. |
| **Anti-Cheat** | ✅ Bypassed | Client/Server modules report "Success" to bypass EAC/BE checks. |

---

## 🛠 Quick Build

The build system is designed to be as "sharp" as the code. No complex configuration required.

### Windows (Visual Studio)
```powershell
./build.bat          # Builds x64 Release
./build.bat x86      # Builds 32-bit (x86)
```

### Linux / macOS
```bash
./build.sh           # Native build
```

---

## ⚙️ Configuration

S-EGS is configured via `egs_settings/configs.user.ini`. 

```ini
[UserInfo]
Username=S-EGS-User

[Ecom]
UnlockDLCs=1         ; Force unlock all entitlements

[Overlay]
EnableOverlay=1      ; Shift+F3 to toggle in-game
ToggleKey=F3

[Debug]
LogLevel=4           ; 4=Info, 5=Debug, 6=Trace
```

### Advanced Spoofing
You can override regional settings directly in the ini to bypass regional locks:
```ini
[Platform]
CountryCode=US
LocaleCode=en
```

---

## 📂 Directory Structure

- `src/platform/` — EOS entry points and launcher-bypass logic.
- `src/auth/` — Identity and JWT generation core.
- `src/connect/` — Cross-platform account mapping.
- `src/ecom/` — Storefront and entitlement emulation.
- `src/overlay/` — Social overlay and notification hooks.
- `include/utils/` — The `se` namespace and modern C++ aliases.

---

## 💎 Human-Written Excellence

The S-EGS codebase is maintained with a focus on code as documentation. Every line is written to be sharp, easy to read, and free of legacy boilerplate. This isn't just an emulator; it's a modern C++ implementation of the Epic Online Services ecosystem.

---
*Developed for the high-end emulation community. Sharper. Cleaner. Faster.*
