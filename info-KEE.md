# KEE (Kaneki's Epic Emulator)

## Overview
**KEE = Kaneki's Epic Emulator** — a drop-in replacement for the **Epic Online Services SDK**
(`EOSSDK-Win64-Shipping.dll` / `EOSSDK-Win32-Shipping.dll` and equivalents). Lets Epic games run fully
offline without the Epic Games Launcher while preserving multiplayer, achievements, stats, and
connectivity behavior. Built from the **S-EGS** codebase (found on D drive at `D:\s-egs`).

## Where the code is
- **Local working copy: `D:\s-egs\s-egs`** (full source tree) — with detailed history/design doc at `D:\s-egs\info.md`
- GitHub: referenced as "by segs" — related repos: `the-lust/femboy` ("a simple emulator for games to
  work better"), `the-lust/eosanalyze` (`D:\eosanalyze`, an EOS analysis tool).
- This workspace folder is the home for the refactored/unified KEE project.

## Research notes (from D:\s-egs\info.md)
### What S-EGS does
- Emulates the EOS SDK surface: platform init, Auth, Connect, Ecom, PlayerDataStorage, Achievements,
  Stats, Presence, Sessions, Lobbies, P2P, Leaderboards, UserInfo, TitleStorage, CustomInvites etc.
- **Dual-architecture proxy loader DLLs**: `winmm.dll` / `version.dll` (x86 + x64) that hijack
  `GetCommandLineW` and append Epic Launcher args when missing:
  ```
  -AUTH_LOGIN=unused -AUTH_PASSWORD=segs-token -AUTH_TYPE=exchangecode -epicapp=segs -epicenv=Prod -EpicPortal
  ```
  then load the real `EOSSDK-Win64-Shipping.dll` / `EOSSDK-Win32-Shipping.dll`.
- **Magic side-loader**: on startup checks for `andreh.dll`, `cirno.dll`, `0xzeon.dll`, `kirigiri.dll`,
  `mojtaba.dll` in the game folder, else loads everything from `egs_settings/load_dlls/`.
- **Launcher bypass**: `CheckForLauncherAndRestart` always returns `EOS_NoChange`; regional defaults
  `GetActiveCountryCode` -> "US", `GetActiveLocaleCode` -> "en" (prevents UE5 crashes).
- **Codebase modernization done**: `se` namespace + type aliases (`se::string`, `se::path`, `se::json`,
  `se::lock`, `se::clock`...), `m_` member prefix, `dispatch_cb<T>` / `SE_CB_BEGIN/END` callback helpers
  (40%+ code reduction), callback manager, human-style comments.
- **Build**: `build.bat all` builds x86 + x64 emulators + both proxies -> 6 DLLs:
  - `build-release/x64/EOSSDK-Win64-Shipping.dll`, `winmm.dll`, `version.dll`
  - `build-release/x86/EOSSDK-Win32-Shipping.dll`, `winmm.dll`, `version.dll`
- Config: `egs_settings/` (with `egs_settings.EXAMPLE`) — regional overrides (CountryCode, LocaleCode),
  load_dlls folder.
- Also uses premake5 + CMake (`premake5.lua`, `CMakeLists.txt`), `S-EGS.sln`.

## Target feature set for KEE
- [ ] Full EOS SDK emulation surface (all services listed above) with offline-first behavior
- [ ] Proxy loader pair per arch (winmm/version) + command-line injection
- [ ] Magic side-loader + egs_settings config
- [ ] Unified build: one command -> 6 DLLs; CI-friendly
- [ ] Multiplayer stubs (LAN/local P2P) where feasible
- [ ] Optional real network relay mode (play with friends via self-hosted relay)
- [ ] Overlay support (global overlay hook)

## Roadmap
1. Bring `D:\s-egs\s-egs` source into this folder (git init, commit as base)
2. Re-run build, verify all 6 DLLs compile clean (x86+x64)
3. Inventory emulated exports vs real EOS SDK headers (see `def_functions.txt` / `impl_functions.txt`
   in source root) and fill gaps
4. Add test titles (e.g. a UE5 sample) + per-game config examples
5. Docs: full emulated API reference, per-game setup guide
