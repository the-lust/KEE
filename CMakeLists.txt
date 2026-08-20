cmake_minimum_required(VERSION 3.16)
project(KEE VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---------------------------------------------------------------------------
# Platform / architecture detection
# ---------------------------------------------------------------------------
if(WIN32)
    set(KEE_WINDOWS ON)
    add_compile_definitions(__WINDOWS__)
elseif(APPLE)
    set(KEE_MACOS ON)
    add_compile_definitions(__APPLE__)
elseif(UNIX)
    set(KEE_LINUX ON)
    add_compile_definitions(__LINUX__)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(KEE_ARCH "x64")
    add_compile_definitions(KEE_64BIT)
else()
    set(KEE_ARCH "x86")
    add_compile_definitions(KEE_32BIT)
endif()

message(STATUS "Building KEE for ${CMAKE_SYSTEM_NAME} ${KEE_ARCH}")

# ---------------------------------------------------------------------------
# Output naming - must match what games expect:
#   EOSSDK-Win64-Shipping / EOSSDK-Win32-Shipping / EOSSDK (Unix)
# ---------------------------------------------------------------------------
if(KEE_WINDOWS)
    if(KEE_ARCH STREQUAL "x64")
        set(KEE_OUTPUT_NAME "EOSSDK-Win64-Shipping")
    else()
        set(KEE_OUTPUT_NAME "EOSSDK-Win32-Shipping")
    endif()
else()
    set(KEE_OUTPUT_NAME "EOSSDK")
endif()

# ---------------------------------------------------------------------------
# Compiler flags
# ---------------------------------------------------------------------------
if(MSVC)
    add_compile_options(/W3 /wd4996 /wd4267 /wd4244 /MP /FS /bigobj)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG")
    # Static runtime everywhere; the DLLs get dropped into game folders,
    # so they must not depend on a specific MSVC runtime being installed.
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    add_compile_options(-Wall -Wno-unused-parameter -Wno-missing-field-initializers)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2)
    else()
        add_compile_options(-g)
    endif()
    if(KEE_LINUX)
        # Hide all symbols by default; only EOS_* are exported via visibility attributes
        add_compile_options(-fvisibility=hidden)
    endif()
endif()

# ---------------------------------------------------------------------------
# Protobuf - system copy first, bundled prebuilt on Windows otherwise
# ---------------------------------------------------------------------------
find_package(Protobuf QUIET)

if(Protobuf_FOUND)
    message(STATUS "Using system protobuf ${Protobuf_VERSION}")
    set(PROTOBUF_LIB protobuf::libprotobuf-lite)
else()
    message(STATUS "System protobuf not found - using bundled prebuilt (Windows only)")
    if(KEE_WINDOWS)
        if(KEE_ARCH STREQUAL "x64")
            set(PROTOBUF_LIB "${CMAKE_SOURCE_DIR}/third_party/protobuf/build_lite/Release/protobuf_combined.lib")
        else()
            set(PROTOBUF_LIB "${CMAKE_SOURCE_DIR}/third_party/protobuf/build_lite_x86/Release/protobuf_combined.lib")
        endif()
    else()
        message(FATAL_ERROR "Protobuf is required. Install libprotobuf-dev (apt) or protobuf-devel (dnf).")
    endif()
endif()

# ---------------------------------------------------------------------------
# MinHook (Windows only)
# ---------------------------------------------------------------------------
if(KEE_WINDOWS)
    set(MINHOOK_SOURCES
        third_party/minhook/src/buffer.c
        third_party/minhook/src/hook.c
        third_party/minhook/src/trampoline.c
        third_party/minhook/src/hde/hde32.c
        third_party/minhook/src/hde/hde64.c
    )
    add_library(MinHook STATIC ${MINHOOK_SOURCES})
    target_include_directories(MinHook PUBLIC third_party/minhook/include)
    set_target_properties(MinHook PROPERTIES C_STANDARD 99)
endif()

# ---------------------------------------------------------------------------
# Emulator sources (shared by the SDK DLL and the EGClient DLL)
# ---------------------------------------------------------------------------
file(GLOB_RECURSE KEE_SOURCES
    "src/*.cpp"
    "src/*.c"
    "include/network/proto/*.cc"
)

# Exclude the proxy loader and tools from the library builds
list(FILTER KEE_SOURCES EXCLUDE REGEX "src/proxy/proxy\\.cpp$")
list(FILTER KEE_SOURCES EXCLUDE REGEX "src/cold_loader/cold_loader\\.cpp$")
list(FILTER KEE_SOURCES EXCLUDE REGEX "src/tools/generate_interfaces\\.cpp$")

# Exclude Windows-specific overlay DX hooks on non-Windows
if(NOT KEE_WINDOWS)
    list(FILTER KEE_SOURCES EXCLUDE REGEX "src/overlay/dx_hooks\\.cpp$")
endif()

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
set(KEE_INCLUDE_DIRS
    src
    include
    include/sdk
    third_party
    third_party/fifo_map/src
    third_party/minhook/include
    third_party/protobuf/third_party/utf8_range
)

# Auto-discover all include/* and src/* subdirectories
file(GLOB INCLUDE_SUBDIRS LIST_DIRECTORIES true "include/*")
file(GLOB SRC_SUBDIRS    LIST_DIRECTORIES true "src/*")
foreach(d IN LISTS INCLUDE_SUBDIRS SRC_SUBDIRS)
    if(IS_DIRECTORY ${d})
        list(APPEND KEE_INCLUDE_DIRS ${d})
    endif()
endforeach()

if(Protobuf_FOUND)
    list(APPEND KEE_INCLUDE_DIRS ${Protobuf_INCLUDE_DIRS})
else()
    list(APPEND KEE_INCLUDE_DIRS third_party/protobuf/src)
endif()

# ---------------------------------------------------------------------------
# Windows system libraries used by the emulator
# ---------------------------------------------------------------------------
if(KEE_WINDOWS)
    set(KEE_SYSTEM_LIBS
        ws2_32
        winmm
        advapi32
        ole32
        iphlpapi
        crypt32
        wintrust
        winhttp
        dbghelp
        shell32
        pathcch
    )
elseif(KEE_MACOS)
    set(KEE_SYSTEM_LIBS
        "-framework CoreFoundation"
        "-framework SystemConfiguration"
    )
else()
    set(KEE_SYSTEM_LIBS pthread dl)
endif()

# ---------------------------------------------------------------------------
# Main emulator DLL
# ---------------------------------------------------------------------------
add_library(KEE SHARED ${KEE_SOURCES})

if(KEE_WINDOWS)
    target_sources(KEE PRIVATE src/kee.def)
endif()

set_target_properties(KEE PROPERTIES
    OUTPUT_NAME ${KEE_OUTPUT_NAME}
    PREFIX ""       # no "lib" prefix on Linux/macOS - games won't find "libEOSSDK.so"
)

target_include_directories(KEE PRIVATE ${KEE_INCLUDE_DIRS})
target_compile_definitions(KEE PRIVATE EOS_USE_DLLEXPORT=1 EOS_BUILDING_SDK=1)

if(KEE_WINDOWS)
    target_link_libraries(KEE PRIVATE MinHook ${PROTOBUF_LIB} ${KEE_SYSTEM_LIBS})
else()
    target_link_libraries(KEE PRIVATE ${PROTOBUF_LIB} ${KEE_SYSTEM_LIBS})
endif()

# ---------------------------------------------------------------------------
# Proxy loader DLLs (winmm.dll / version.dll) - Windows only
# ---------------------------------------------------------------------------
if(KEE_WINDOWS)
    add_library(KEE-Proxy-WinMM SHARED src/proxy/proxy.cpp)
    target_sources(KEE-Proxy-WinMM PRIVATE src/proxy/proxy_winmm.def)
    set_target_properties(KEE-Proxy-WinMM PROPERTIES OUTPUT_NAME "winmm")
    target_include_directories(KEE-Proxy-WinMM PRIVATE third_party/minhook/include)
    target_link_libraries(KEE-Proxy-WinMM PRIVATE MinHook)

    add_library(KEE-Proxy-Version SHARED src/proxy/proxy.cpp)
    target_sources(KEE-Proxy-Version PRIVATE src/proxy/proxy_version.def)
    set_target_properties(KEE-Proxy-Version PROPERTIES OUTPUT_NAME "version")
    target_include_directories(KEE-Proxy-Version PRIVATE third_party/minhook/include)
    target_link_libraries(KEE-Proxy-Version PRIVATE MinHook)

    if(MSVC)
        # Keep the proxies free of any CRT dependency beyond the static one.
        set_property(TARGET KEE-Proxy-WinMM KEE-Proxy-Version PROPERTY
            LINK_OPTIONS "/NODEFAULTLIB:msvcrt.lib;/NODEFAULTLIB:msvcprt.lib")
    endif()

    # -----------------------------------------------------------------------
    # EGClient DLL - emulates the EGS client API for games that talk to it
    # -----------------------------------------------------------------------
    add_library(KEE-EGClient SHARED ${KEE_SOURCES})
    target_sources(KEE-EGClient PRIVATE src/egclient.def)
    if(KEE_ARCH STREQUAL "x64")
        set_target_properties(KEE-EGClient PROPERTIES OUTPUT_NAME "egclient64")
    else()
        set_target_properties(KEE-EGClient PROPERTIES OUTPUT_NAME "egclient")
    endif()
    target_include_directories(KEE-EGClient PRIVATE ${KEE_INCLUDE_DIRS})
    target_compile_definitions(KEE-EGClient PRIVATE EOS_USE_DLLEXPORT=1 EOS_BUILDING_SDK=1 EMU_CLIENT_DLL=1)
    target_link_libraries(KEE-EGClient PRIVATE MinHook ${PROTOBUF_LIB} ${KEE_SYSTEM_LIBS})

    # -----------------------------------------------------------------------
    # Cold loader - launches a game suspended and injects the emulator DLLs
    # -----------------------------------------------------------------------
    add_executable(KEE-ColdLoader WIN32 src/cold_loader/cold_loader.cpp)
    if(KEE_ARCH STREQUAL "x64")
        set_target_properties(KEE-ColdLoader PROPERTIES OUTPUT_NAME "cold_loader64")
    else()
        set_target_properties(KEE-ColdLoader PROPERTIES OUTPUT_NAME "cold_loader")
    endif()

    # -----------------------------------------------------------------------
    # Interface generator tool (regenerates stub/export lists)
    # -----------------------------------------------------------------------
    add_executable(KEE-GenerateInterfaces src/tools/generate_interfaces.cpp)
    if(KEE_ARCH STREQUAL "x64")
        set_target_properties(KEE-GenerateInterfaces PROPERTIES OUTPUT_NAME "generate_interfaces_x64")
    else()
        set_target_properties(KEE-GenerateInterfaces PROPERTIES OUTPUT_NAME "generate_interfaces")
    endif()
endif()

# ---------------------------------------------------------------------------
# Output placement
# ---------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${KEE_ARCH}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${KEE_ARCH}/$<CONFIG>")

install(TARGETS KEE
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
