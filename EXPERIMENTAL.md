# Experimental Features & Known Divergences

This document lists behavior that differs from the official Epic Online Services SDK,
or features that are experimental/untested.

## Not Implemented / Stubbed

| Area | Status | Notes |
|------|--------|-------|
| RTC Voice (`EOS_RTCAudio_*`) | Stubbed | Room join/leave works; audio callbacks return success but no transport |
| RTCAdmin (`EOS_RTCAdmin_*`) | Stubbed | Admin APIs return success no-op |
| WebLinks (`EOS_WebLinks_*`) | Stubbed | URL open/close reported; no browser spawn |
| TextChat (`EOS_TextChat_*`) | Partial | Connect/disconnect works; message send/receive not routed |
| Mods (`EOS_Mods_*`) | Partial | Install/enumerate works; no actual mod fetching |
| Custom Invites (`EOS_CustomInvites_*`) | Stubbed | Returns success; no actual invite system |

## Deviations from Official SDK

### Identity & Auth
- **Offline identity only**: `EOS_Auth_Login` with `EOS_LCT_DeviceCode` or `EOS_LCT_ExchangeCode` succeeds locally; no real token exchange.
- **EpicAccountId**: Generated from local config (`Username` + salt) — deterministic per machine, not globally unique.
- **No account linking**: `EOS_Connect_LinkAccount` is no-op.

### Ecom / Entitlements
- **All DLCs owned if `UnlockDLCs=1`**: `EOS_Ecom_QueryOwnership` returns owned for all catalog items.
- **No receipt validation**: `EOS_Ecom_RedeemEntitlements` succeeds without server contact.

### Lobby & Sessions
- **LAN-only peer discovery**: Works on local subnet; no relay server, no NAT traversal.
- **Invites**: `EOS_Lobby_SendInvite` broadcasts UDP packet on LAN; no overlay integration.
- **Search**: `EOS_Sessions_Search` filters local lobby list; no backend pagination.

### P2P
- **Reliability layer**: Implemented over TCP relay; packet ordering/ack same as official.
- **Max packet size**: 1200 bytes (fits UDP MTU); larger fragmented.
- **No ICE/STUN**: Direct connect not implemented.

### Anti-Cheat
- **EAC Client/Server**: All calls return `EOS_Success`; no actual verification.

### Integrated Platform
- `EOS_IntegratedPlatform_CheckForLauncherAndRestart` → immediately returns `EOS_NotFound` (no restart).
- `EOS_IntegratedPlatform_GetOverlayHandle` → returns 0.

### Metrics / PlayerData / TitleStorage
- Persisted to `kee_settings/storage/` as JSON files (per-app, per-user).
- No cloud sync; local only.

## Known Missing SDK Exports (3)

The following official EOS functions are **not** exported in `kee.def`:

1. `EOS_Platform_GetDesktopCrossplayStatusOptions`
2. `EOS_Platform_GetCrossplayStatusOptions`
3. `EOS_UI_ShowNativeProfile` (stubbed in `src/ui/eossdk_ui.cpp` but not exported)

These are either internal/undocumented or require backend services.

## Extra Exports (61)

`kee.def` includes 61 non-SDK exports for internal/platform use:
- `EOS_EpicConnect_*` — legacy EGS account APIs
- `EOS_Crypto_*` — symmetric encryption helpers
- `EOS_Mercury_*` — legacy voice relay
- `EOS_K3S_*` — legacy session service
- `EOS_BroadcastAudio_*` — local audio routing
- `EOS_UI_ShowReportVoice` — removed (was fake)

## Protobuf Dependency

- **With protobuf** (default): LAN relay active, cross-machine play works.
- **Without protobuf**: Network module stubbed; `EOS_P2P_*`/`EOS_Lobby_*` still return success but no packets leave the process.

Build without: `premake5 vs2022 --without-protobuf`

## x86 Build Status

x64 Release: **passing** (4 DLLs built, 0 link errors).

x86 Release: **blocked** — `third_party/protobuf/build_lite_x86/` contains stale paths from original build machine. To fix:

```cmd
rd /s /q third_party\protobuf\build_lite_x86
cmake -S third_party\protobuf -B third_party\protobuf\build_lite_x86 -DCMAKE_GENERATOR_PLATFORM=x86 -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_PROTOC_BINARIES=ON -Dprotobuf_ABSL_PROVIDER=package
cmake --build third_party\protobuf\build_lite_x86 --config Release
```

Then rebuild KEE for Win32.