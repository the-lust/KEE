# KEE — project notes

## Overview

**KEE = Kaneki's Epic Emulator** — a drop-in replacement for the Epic Online Services SDK
(`EOSSDK-Win64-Shipping.dll` / `EOSSDK-Win32-Shipping.dll`). Epic games run offline without the
Epic Games Launcher, while keeping multiplayer, achievements, stats and presence behavior.

Origin: built from the **S-EGS** codebase, refactored into one unified project in this folder.

## Repository layout (as committed)

- `KEE.sln` — premake5-generated Visual Studio solution (regenerate: `premake5 vs2022`)
- `premake5.lua` — the build definition; produces 6 DLLs + loader + tools
- `src/kee.def`, `src/egclient.def` — export tables (1072 emulator exports)
- `tools/inventory_exports.ps1` — export-coverage verification vs the official SDK headers
- `info-KEE.md`, `README.md` — docs

## Emulator architecture

- One DLL per role:
  - `KEE-Emulator` → `EOSSDK-Win64-Shipping.dll` / `EOSSDK-Win32-Shipping.dll`
  - `Proxy-WinMM` / `Proxy-Version` → `winmm.dll` / `version.dll` (command-line injection +
    loading of the real SDK DLL)
  - `EGClient` → `egclient64.dll` / `egclient.dll` (EGS client emulation)
  - `ColdLoader` → `cold_loader64.exe` / `cold_loader.exe` (boots the target game)
- Services live in `src/<service>/`; each has an `eossdk_<svc>.cpp` implementation class and
  `*_exports.cpp` wrapping the C ABI.
- The LAN relay engine (`src/network/network.cpp`) does UDP broadcast discovery on the
  `network_port..max_network_port` range (see `include/network/network.h`) and TCP relay of
  length-prefixed protobuf messages (`src/network/proto/network_proto.proto` — regenerate with
  protoc 7.35.1, i.e. `protoc -I src/network/proto --cpp_out=... network_proto.proto`).
- When protobuf is unavailable (`--without-protobuf`), `src/eossdk_class_stubs.cpp` provides
  no-op Network/service stubs; all exports still exist and return safely.

## Build state / history of fixes

1. **Gap analysis** — `tools/inventory_exports.ps1` compares SDK headers vs `kee.def` vs
   implementations. Result: 0 SDK functions missing, 0 def exports without definition
   (1072/1072 exports defined; 1011 SDK functions, 61 extra platform/helpers).
2. **Removed fake exports** — `EOS_PresenceModification_SetTemplateData` /
   `EOS_PresenceModification_SetTemplateId` do not exist in the real EOS SDK; deleted from
   `src/kee.def` and the exports file.
3. **Network module migration** — the import carried a `network.cpp` written against an older
   PortableAPI (exception-based sockets, `Socket` poll class, `ipv4m_addr` setters). Rewritten
   for the current poll-based `portable_api.h` with the same wire protocol; the `Network` class
   header (`include/network/network.h`) received its full member set back.
4. **Missing generated protobuf TU** — `network_proto.pb.cc` was never matched by premake
   (`src/**.cpp` does not match `.cc`), causing LNK2001 for every protobuf message. Added
   explicitly to `premake5.lua`; regenerated both `.pb.cc` and `.pb.h` with protoc 35.1
   (the checked-in `.pb.h` was already 7.35.1-identical), keeping them in sync.
5. `src/stubs.cpp` gained `#include <cstring>`; ColdLoader vcxproj subsystem fixed via premake
   (`WindowedApp` now emits `SubSystem=Windows`).

## Verification workflow

```powershell
powershell -File tools/inventory_exports.ps1    # coverage report -> build/export_gap_report.json
msbuild KEE.sln /p:Configuration=Release /p:Platform=x64 /m
```

## Roadmap

- [x] Import source, git init, initial commit
- [x] Rebuild all 6 DLLs clean (x86 + x64)
- [x] Export-coverage gap analysis and fixes
- [ ] Test titles (UE5 sample) + per-game config examples
- [ ] Full emulated-API reference doc
- [ ] Optional self-hosted relay server for cross-machine play (relay core exists in-tree)