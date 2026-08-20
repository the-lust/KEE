-- premake5.lua – KEE emulator build script (gbe_fork-aligned)
-- Works with Premake 5.0-alpha16+
-- Usage:
--   premake5 vs2022          (Windows, generates Visual Studio 2022 solution)
--   premake5 vs2019          (Windows, generates Visual Studio 2019 solution)
--   premake5 gmake2          (Linux/macOS, generates Makefiles)
--   premake5 xcode4          (macOS, generates Xcode project)
--
-- Options:
--   --without-protobuf       Force build without protobuf (network stubs only)

newoption {
   trigger     = "without-protobuf",
   description = "Force build without protobuf (network stubs only)"
}

-- ── Dependency detection ─────────────────────────────────────────────────
local function has_protobuf()
   if _OPTIONS["without-protobuf"] then return false end
   local proto_header = os.stat("third_party/protobuf/src/google/protobuf/message.h")
   return proto_header ~= nil
end

local HAS_PROTOBUF = has_protobuf()

-- Protobuf lib path helpers
local function protobuf_lib_path(arch)
   if arch == "x64" then
      return "third_party/protobuf/build_lite/Release/protobuf_combined.lib"
   else
      return "third_party/protobuf/build_lite_x86/Release/protobuf_combined.lib"
   end
end

workspace "KEE"
   configurations { "Debug", "Release" }
   platforms      { "x86", "x64" }
   language       "C++"
   cppdialect     "C++17"
   characterset   "MBCS"

   -- ── Windows flags ──────────────────────────────────────────────────────
   filter "system:windows"
      systemversion "latest"
      defines {
         "_CRT_SECURE_NO_WARNINGS",
         "_SCL_SECURE_NO_WARNINGS",
         "__WINDOWS__",
      }
      buildoptions { "/FS", "/bigobj" }

   -- ── Linux flags ────────────────────────────────────────────────────────
   filter "system:linux"
      defines { "__LINUX__" }
      buildoptions { "-fvisibility=hidden" }

   -- ── macOS flags ────────────────────────────────────────────────────────
   filter "system:macosx"
      buildoptions { "-fvisibility=hidden" }

   -- ── Architecture ───────────────────────────────────────────────────────
   filter "platforms:x86"
      architecture "x86"
      targetdir    "bin/x86/%{cfg.buildcfg}"
      objdir       "obj/x86/%{cfg.buildcfg}"
      defines      { "KEE_32BIT" }

   filter "platforms:x64"
      architecture "x86_64"
      targetdir    "bin/x64/%{cfg.buildcfg}"
      objdir       "obj/x64/%{cfg.buildcfg}"
      defines      { "KEE_64BIT" }

   -- ── Configurations ─────────────────────────────────────────────────────
   filter "configurations:Debug"
      defines  { "DEBUG", "_DEBUG" }
      symbols  "On"

   filter "configurations:Release"
      defines  { "NDEBUG" }
      optimize "Speed"
      filter { "configurations:Release", "toolset:msc*" }
         flags { "LinkTimeOptimization" }

   filter {}

   -- ── Protobuf detection (global) ────────────────────────────────────────
   if HAS_PROTOBUF then
      filter "platforms:x64"
         defines { "NETWORK_PROTOBUF_AVAILABLE" }
         includedirs {
            "third_party/protobuf/build_lite/config",
            "third_party/protobuf/build_lite/_deps/absl-src",
         }
      filter "platforms:x86"
         defines { "NETWORK_PROTOBUF_AVAILABLE" }
         includedirs {
            "third_party/protobuf/build_lite_x86/config",
            "third_party/protobuf/build_lite_x86/_deps/absl-src",
         }
      filter {}
      -- Shared protobuf include paths (headers are the same for both archs)
      includedirs {
         "third_party/protobuf/src",
         "third_party/protobuf/third_party/utf8_range",
      }
   end

   -- ── Static runtime for Release (gbe_fork style) ────────────────────────
   filter { "configurations:Release", "system:windows" }
      staticruntime "On"
   filter {}

-- ── MinHook (Windows-only static lib) ──────────────────────────────────────
project "MinHook"
   kind        "StaticLib"
   language    "C"
   location    "build/minhook"
   files {
      "third_party/minhook/src/buffer.c",
      "third_party/minhook/src/hook.c",
      "third_party/minhook/src/trampoline.c",
      "third_party/minhook/src/hde/hde32.c",
      "third_party/minhook/src/hde/hde64.c",
   }
   includedirs { "third_party/minhook/include" }

   filter { "platforms:x64" }
      defines { "m_M_X64" }
   filter { "platforms:x86" }
      defines { "m_M_IX86" }
   filter {} 

   filter "system:not windows"
      kind "None"

-- ── Main emulator DLL ──────────────────────────────────────────────────────
project "KEE-Emulator"
   kind          "SharedLib"
   targetprefix  ""
   location      "build"

   filter { "platforms:x64", "system:windows" }
      targetname "EOSSDK-Win64-Shipping"
   filter { "platforms:x86", "system:windows" }
      targetname "EOSSDK-Win32-Shipping"
   filter "system:linux"
      targetname "EOSSDK"
   filter "system:macosx"
      targetname "EOSSDK"
   filter {}

   -- All sources
   files {
      "src/**.h", "src/**.hpp",
      "src/**.c", "src/**.cpp",
      "include/**.h",
      "third_party/mini_detour/mini_detour.cpp",
      "src/stubs.cpp",
   }

   -- Exclude non-library build artifacts
   removefiles {
      "src/proxy/proxy.cpp",
      "src/cold_loader/cold_loader.cpp",
      "src/tools/generate_interfaces.cpp",
   }

   -- Exclude protobuf-dependent files if protobuf not available
   if not HAS_PROTOBUF then
      removefiles {
         "src/network/proto/network_proto.pb.cc",
         "src/network/proto/network_proto.pb.h",
         "src/network/network.cpp",
         "src/connect/eossdk_connect.cpp",
         "src/presence/eossdk_presence.cpp",
         "src/presence/eossdk_presencemodification.cpp",
         "src/presence/presence_exports.cpp",
         "src/presencemodification/presencemodification_exports.cpp",
         "src/userinfo/eossdk_userinfo.cpp",
         "src/sessions/eossdk_sessions.cpp",
         "src/sessions/eossdk_sessionssearch.cpp",
         "src/sessions/eossdk_sessionmodifications.cpp",
         "src/sessions/eossdk_sessiondetails.cpp",
         "src/sessions/eossdk_activesession.cpp",
         "src/rtcvideo/rtcvideo_exports.cpp",
         "src/rtcvideo/eossdk_rtcvideo.cpp",
         "src/rtcaudio/rtcaudio_exports.cpp",
         "src/rtcaudio/eossdk_rtcaudio.cpp",
         "src/textchatclient/textchat_exports.cpp",
         "src/rtcadmin/rtcadmin_exports.cpp",
         "src/rtcadmin/eossdk_rtcadmin.cpp",
         "src/p2p/eossdk_p2p.cpp",
         "src/lobby/eossdk_lobby.cpp",
         "src/lobby/eossdk_lobbysearch.cpp",
         "src/lobby/eossdk_lobbydetails.cpp",
         "src/lobby/eossdk_lobbymodification.cpp",
         "src/lobby/lobby_exports.cpp",
         "src/friends/eossdk_friends.cpp",
         "src/custominvites/eossdk_custominvites.cpp",
         "src/progressionsnapshot/eossdk_progressionsnapshot.cpp",
         "src/progressionsnapshot/progressionsnapshot_exports.cpp",
         "src/rtc/eossdk_rtc.cpp",
         "src/rtc/rtc_exports.cpp",
         "src/rtc/eossdk_rtcmoderation.cpp",
         "src/rtc/eossdk_rtcaudio.cpp",
         "src/integratedplatform/eossdk_integratedplatform.cpp",
         "src/parties/parties_exports.cpp",
      }
   end

   -- Windows .def export file
   filter "system:windows"
      files { "src/kee.def" }
   filter {}

   -- ── Include directories ──────────────────────────────────────────────
   local incdirs = {
      "src", "src/platform",
      "include", "include/sdk", "include/utils",
      "third_party",
      "third_party/fifo_map/src",
      "third_party/minhook/include",
   }
   -- Add protobuf includes if available
   if HAS_PROTOBUF then
      table.insert(incdirs, "third_party/protobuf/src")
      table.insert(incdirs, "third_party/protobuf/build_lite/config")
      table.insert(incdirs, "third_party/protobuf/third_party/utf8_range")
      table.insert(incdirs, "src/network/proto")
   end
   -- Auto-add all include/* subdirs
   for _, d in ipairs(os.matchdirs("include/*")) do
      table.insert(incdirs, d)
   end
   -- Auto-add all src/* subdirs
   for _, d in ipairs(os.matchdirs("src/*")) do
      table.insert(incdirs, d)
   end
   includedirs(incdirs)

   defines { "EOS_USE_DLLEXPORT=1", "EOS_BUILDING_SDK=1" }

   -- ── Windows linking ──────────────────────────────────────────────────
   filter "system:windows"
      links {
         "MinHook",
         "ws2_32", "winmm", "advapi32", "ole32",
         "iphlpapi", "crypt32", "wintrust", "winhttp",
         "dbghelp", "shell32", "d3d11", "dxgi",
      }
   filter {}

   -- ── Protobuf lib linking ─────────────────────────────────────────────
   if HAS_PROTOBUF then
      filter { "system:windows", "platforms:x64" }
         links { protobuf_lib_path("x64") }
      filter { "system:windows", "platforms:x86" }
         links { protobuf_lib_path("x86") }
      filter {}
   end

   -- ── Linux linking ────────────────────────────────────────────────────
   filter "system:linux"
      links { "pthread", "dl" }
      if HAS_PROTOBUF then
         links { "protobuf" }
      end
      linkoptions { "-Wl,--exclude-libs,ALL" }

   -- ── macOS linking ────────────────────────────────────────────────────
   filter "system:macosx"
      if HAS_PROTOBUF then
         links { "protobuf" }
      end
      linkoptions { "-framework CoreFoundation", "-framework SystemConfiguration" }

   filter {}

-- ── Proxy DLLs (Windows-only) ──────────────────────────────────────────────
project "Proxy-WinMM"
   kind          "SharedLib"
   targetname    "winmm"
   targetprefix  ""
   location      "build/proxy"
   files         { "src/proxy/proxy.cpp" }
   includedirs   { "third_party/minhook/include" }
   filter "system:windows"
      links { "MinHook" }
   filter "system:not windows"
      kind "None"

project "Proxy-Version"
   kind          "SharedLib"
   targetname    "version"
   targetprefix  ""
   location      "build/proxy"
   files         { "src/proxy/proxy.cpp" }
   includedirs   { "third_party/minhook/include" }
   filter "system:windows"
      links { "MinHook" }
   filter "system:not windows"
      kind "None"

-- ── EGClient DLL (client emulation, like gbe_fork's steamclient) ──────────
project "EGClient"
   kind          "SharedLib"
   targetprefix  ""
   location      "build/egclient"

   filter { "platforms:x64", "system:windows" }
      targetname "egclient64"
   filter { "platforms:x86", "system:windows" }
      targetname "egclient"
   filter {}

   -- Same sources as main emulator
   files {
      "src/**.h", "src/**.hpp",
      "src/**.c", "src/**.cpp",
      "include/**.h",
      "third_party/mini_detour/mini_detour.cpp",
      "src/stubs.cpp",
   }

   -- Exclude protobuf-dependent files (same as main emu)
   removefiles {
      "src/network/proto/network_proto.pb.cc",
      "src/network/network.cpp",
      "src/proxy/proxy.cpp",
      "src/cold_loader/cold_loader.cpp",
      "src/tools/generate_interfaces.cpp",
   }

   -- Windows .def export file (EGS client exports)
   filter "system:windows"
      files { "src/egclient.def" }
   filter {}

   -- Include directories
   local incdirs = {
      "src", "src/platform",
      "include", "include/sdk", "include/utils",
      "third_party",
      "third_party/fifo_map/src",
      "third_party/minhook/include",
   }
   if HAS_PROTOBUF then
      table.insert(incdirs, "src/network/proto")
   end
   -- Auto-add all include/* subdirs
   for _, d in ipairs(os.matchdirs("include/*")) do
      table.insert(incdirs, d)
   end
   -- Auto-add all src/* subdirs
   for _, d in ipairs(os.matchdirs("src/*")) do
      table.insert(incdirs, d)
   end
   includedirs(incdirs)

   defines { "EOS_USE_DLLEXPORT=1", "EOS_BUILDING_SDK=1", "EMU_CLIENT_DLL=1" }

   filter "system:windows"
      links { "MinHook", "ws2_32", "winmm", "advapi32", "ole32",
              "iphlpapi", "crypt32", "wintrust", "winhttp",
              "dbghelp", "shell32", "d3d11", "dxgi" }
   filter {}

   -- ── Protobuf lib linking (same as main emu) ──────────────────────────
   if HAS_PROTOBUF then
      filter { "system:windows", "platforms:x64" }
         links { protobuf_lib_path("x64") }
      filter { "system:windows", "platforms:x86" }
         links { protobuf_lib_path("x86") }
      filter {}
   end

-- ── Cold Loader EXE (like gbe_fork's coldclient_loader) ────────────────────
project "ColdLoader"
   kind          "WindowedApp"
   targetprefix  ""
   location      "build/cold_loader"

   filter { "platforms:x64", "system:windows" }
      targetname "cold_loader64"
   filter { "platforms:x86", "system:windows" }
      targetname "cold_loader"
   filter {}

   files { "src/cold_loader/cold_loader.cpp" }

   filter "system:not windows"
      kind "None"

-- ── Tools ──────────────────────────────────────────────────────────────────
project "GenerateInterfaces"
   kind          "ConsoleApp"
   targetprefix  ""
   location      "build/tools"

   filter { "platforms:x64", "system:windows" }
      targetname "generate_interfaces_x64"
   filter { "platforms:x86", "system:windows" }
      targetname "generate_interfaces"
   filter {}

   files { "src/tools/generate_interfaces.cpp" }

   filter "system:not windows"
      kind "None"

