# KEE — Kaneki's Epic Emulator

**KEE** is a drop-in replacement for the **Epic Online Services (EOS) SDK**. It ships the
`EOSSDK-Win64-Shipping.dll` / `EOSSDK-Win32-Shipping.dll` binaries plus optional proxy loaders
(`winmm.dll` / `version.dll`) so Epic games run fully offline — without the Epic Games Launcher,
without an account, and often without any internet connection — while preserving multiplayer
behavior, achievements, stats, presence, lobbies and more.

Built from the S-EGS codebase, refactored into a unified project with a single build.

---

## What works

The emulator implements the EOS SDK surface with **100% export coverage**: every function in the
official EOS headers is exported by `kee.def` and has an implementation (see
`tools/inventory_exports.ps1` for the verification report).

| Service | Level | Notes |
| :--- | :--- | :--- |
| Auth / Identity | Full | Offline identity spoofing, login flows |
| Connect | Full | Device accounts, PUID mapping, friends/presence wiring |
| Ecom / Entitlements | Full | Ownership granting, catalog queries |
| Lobby & Sessions | Full | LAN peer discovery + relay, invites, search |
| P2P Networking | Full | Reliable UDP/TCP transport over LAN relay |
| PlayerData / TitleStorage | Full | Local persistence |
| Achievements / Stats / Leaderboards | Full | Local persistence |
| Presence / UserInfo / Friends / CustomInvites | Full | Local + LAN-synced |
| RTC / RTCAdmin / Voice | Stubbed | Room management; audio transport bypassed |
| Anti-Cheat (EAC) | Bypassed | Client/server modules report success |
| Integrated Platform / Launcher checks | Bypassed | `CheckForLauncherAndRestart` → no restart |

Two emulation layers are compiled in:

- **`NETWORK_PROTOBUF_AVAILABLE` (default, protobuf 7.35.1 vendored):** real LAN relay engine —
  UDP broadcast discovery + TCP message relay between KEE instances on the same network.
- **`--without-protobuf` (network stubs):** everything still works locally; no cross-machine relay.

## The 6 binaries

| Binary | Role |
| :--- | :--- |
| `EOSSDK-Win64-Shipping.dll` / `EOSSDK-Win32-Shipping.dll` | The emulated EOS SDK |
| `winmm.dll`, `version.dll` (x64 + x86) | Proxy loaders: inject launcher command-line args and load the real SDK DLL |
| `egclient64.dll` / `egclient.dll` | EGS client emulation (like `steamclient` in gbe_fork) |
| `cold_loader64.exe` / `cold_loader.exe` | Loader that boots a target game with the emulator |

---

## Building

Requirements: Visual Studio (2019+ Build Tools), **Premake5**, and the vendored protobuf
(`third_party/protobuf` — download v35.1 separately, see `.gitignore`).

```powershell
# premake-generated solution (committed)
build.bat          # x64 Release
build.bat x86      # 32-bit Release
build.bat all      # both archs, collects the 6 DLLs into build-release/

# or directly:
msbuild KEE.sln /p:Configuration=Release /p:Platform=x64 /m
```

Protobuf is optional: `premake5 vs2022 --without-protobuf` builds a fully local-only emulator.

## Configuration

Settings folder: **`kee_settings/`** (copy from `kee_settings.EXAMPLE`).

```ini
[UserInfo]
Username=KEE-User

[Ecom]
UnlockDLCs=1

[Platform]
CountryCode=US
LocaleCode=en
```

## Directory layout

- `src/platform/` — EOS entry points, platform creation, launcher bypass
- `src/<service>/` — one folder per EOS service (`auth`, `connect`, `lobby`, `p2p`, ...)
- `src/network/` — LAN relay engine (UDP discovery + TCP relay over protobuf messages)
- `include/` — public headers: `include/sdk` (EOS API), `include/utils`, `include/network`
- `src/kee.def` — export table for the emulator DLL
- `src/egclient.def` — export table for the EGS client DLL
- `tools/` — `inventory_exports.ps1` (export-coverage report), `generate_interfaces.cpp`
- `third_party/` — vendored deps (minhook, mini_detour, nlohmann json, fifo_map, protobuf)

## Verification

```powershell
powershell -File tools/inventory_exports.ps1  # writes build/export_gap_report.json
```

Reports: SDK functions missing from `kee.def`, and def exports without an implementation.

---

*Developed for the soft-modding / offline-play community.*