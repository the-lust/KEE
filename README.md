# KEE — Kaneki's Epic Online Services SDK Emulator

Drop-in replacement for `EOSSDK-Win64-Shipping.dll` / `EOSSDK-Win32-Shipping.dll`. Runs Epic games offline without the launcher or account.

## Binaries

| File | Role |
|------|------|
| `EOSSDK-Win64-Shipping.dll` / `EOSSDK-Win32-Shipping.dll` | Emulated EOS SDK |
| `winmm.dll`, `version.dll` (x64/x86) | Proxy loaders: inject args + load emulator |
| `egclient64.dll` / `egclient.dll` | EGS client emulation (like `steamclient` in gbe_fork) |
| `cold_loader64.exe` / `cold_loader.exe` | Game launcher |

## Building

```
premake5 vs2022
msbuild KEE.sln /p:Configuration=Release /p:Platform=x64 /m
msbuild KEE.sln /p:Configuration=Release /p:Platform=Win32 /m
```

Requires: VS Build Tools 2019+, Premake5, protobuf 7.35.1 (vendored in `third_party/protobuf`, not tracked).

Optional: `premake5 vs2022 --without-protobuf` — builds local-only, no LAN relay.

## Configuration

Copy `kee_settings.EXAMPLE` to `kee_settings/` and edit:

```ini
[UserInfo]
Username=KEE-User

[Ecom]
UnlockDLCs=1

[Platform]
CountryCode=US
LocaleCode=en
```

## Architecture

```
src/
  platform/       # EOS entry points, launcher bypass
  <service>/      # per-service impl (auth, connect, lobby, p2p, ...)
  network/        # LAN relay: UDP broadcast discovery + TCP/protobuf relay
  kee.def         # emulator export table (1072 exports)
  egclient.def    # EGS client exports
include/
  sdk/            # EOS API headers
  utils/          # portable_api.h (cross-platform sockets, time, etc.)
  network/        # LAN relay headers
third_party/      # minhook, mini_detour, nlohmann json, fifo_map, protobuf
tools/
  inventory_exports.ps1   # export coverage vs official SDK
  generate_interfaces.cpp
```

## LAN Relay (protobuf)

- UDP broadcast discovery on `network_port..max_network_port` (default 40000..40009)
- TCP relay with 4-byte length prefix + protobuf (`src/network/proto/network_proto.proto`)
- `Network` class manages peers, sockets, and message routing
- Regenerate: `protoc -I src/network/proto --cpp_out=src/network/proto src/network/proto/network_proto.proto`

## Verification

```powershell
powershell -File tools/inventory_exports.ps1   # -> build/export_gap_report.json
```

Checks: every `kee.def` export has implementation; every SDK function is exported.

## Experimental / Differences

See `EXPERIMENTAL.md` for features not in stock EOS or known divergences.

## License

MIT — see `LICENSE`.
