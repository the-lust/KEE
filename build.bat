@echo off
setlocal EnableDelayedExpansion

:: ─── build.bat ─────────────────────────────────────────────────────────────
:: KEE emulator build script (Windows)
:: usage: build.bat [all] [Debug|Release]
::   build.bat          -> builds x64 Release
::   build.bat x86      -> builds 32-bit Release
::   build.bat all      -> builds both platforms and collects the outputs
::                        into build-release\
:: ───────────────────────────────────────────────────────────────────────────

set TARGET=%~1
set CONFIG=%~2
if "%CONFIG%"=="" set CONFIG=Release

if "%TARGET%"=="all" (
    call :build x64 %CONFIG%
    call :build x86 %CONFIG%
    call :collect
    goto :eof
)

if "%TARGET%"=="" set TARGET=x64
call :build %TARGET% %CONFIG%
goto :eof

:build
set ARCH=%1
set CFG=%2
echo [KEE] Building %ARCH% %CFG%...

:: locate Visual Studio
set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist %VSWHERE% set VSWHERE="%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`%VSWHERE% -latest -property installationPath`) do set VS_PATH=%%i

if "%ARCH%"=="x64" (
    call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
    set PLAT=x64
) else (
    call "%VS_PATH%\VC\Auxiliary\Build\vcvars32.bat" >nul 2>&1
    set PLAT=Win32
)

:: refresh the solution with premake when available; otherwise reuse KEE.sln
premake5 vs2022 >nul 2>&1
if errorlevel 1 premake5 vs2019 >nul 2>&1

:: detect the installed platform toolset (premake hard-codes v143 which may not
:: be installed on newer VS versions); real toolsets live under
:: MSBuild\Microsoft\VC\v*\Platforms\x64\PlatformToolsets\
set TS=v143
for /d %%v in ("%VS_PATH%\MSBuild\Microsoft\VC\v*") do (
    if exist "%%v\Platforms\x64\PlatformToolsets\v145\" set TS=v145
    if exist "%%v\Platforms\x64\PlatformToolsets\v144\" set TS=v144
    if exist "%%v\Platforms\x64\PlatformToolsets\v143\" set TS=v143
    if exist "%%v\Platforms\x64\PlatformToolsets\v142\" set TS=v142
)
echo [KEE] Using platform toolset: %TS%

:: build all projects
msbuild KEE.sln /p:Configuration=%CFG% /p:Platform=%PLAT% /p:PlatformToolset=%TS% /m
exit /b

:collect
echo [KEE] Collecting binaries to build-release...
if not exist build-release mkdir build-release
if not exist build-release\x64 mkdir build-release\x64
if not exist build-release\x86 mkdir build-release\x86

copy /y bin\x64\%CONFIG%\EOSSDK-Win64-Shipping.dll build-release\x64\
copy /y bin\x64\%CONFIG%\winmm.dll build-release\x64\
copy /y bin\x64\%CONFIG%\version.dll build-release\x64\

copy /y bin\x86\%CONFIG%\EOSSDK-Win32-Shipping.dll build-release\x86\
copy /y bin\x86\%CONFIG%\winmm.dll build-release\x86\
copy /y bin\x86\%CONFIG%\version.dll build-release\x86\

echo.
echo [KEE] All binaries are ready in the build-release folder!
exit /b