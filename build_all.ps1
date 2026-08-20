# KEE emulator build script
# Usage: .\build_all.ps1 [-Clean]
# Builds both platforms with MSBuild and organizes the outputs into build-release
# (mirrors the release layout of gbe_fork).

param(
    [switch]$Clean
)

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    $vswhere = Join-Path $env:ProgramFiles "Microsoft Visual Studio\Installer\vswhere.exe"
}
$vsPath = & $vswhere -latest -property installationPath
if (-not $vsPath) {
    Write-Host "Visual Studio installation not found!" -ForegroundColor Red
    exit 1
}
$MSBuild = Join-Path $vsPath "MSBuild\Current\Bin\MSBuild.exe"
if (-not (Test-Path $MSBuild)) {
    Write-Host "MSBuild.exe not found under $vsPath" -ForegroundColor Red
    exit 1
}

# Detect the installed platform toolset (premake hard-codes v143 which may not
# exist on newer VS installs). The real toolset folders live under the MSBuild
# VC tree's Platforms\<plat>\PlatformToolsets (e.g. v143/v142/v145...).
$TargetToolset = "v143"
$vcTrees = Get-ChildItem (Join-Path $vsPath "MSBuild\Microsoft\VC") -Directory -ErrorAction SilentlyContinue |
    Where-Object { $_.Name -match '^v\d+$' }
foreach ($vcTree in ($vcTrees | Sort-Object { [int]($_.Name -replace '^v', '') } -Descending)) {
    $tsDir = Join-Path $vcTree.FullName "Platforms\x64\PlatformToolsets"
    if (Test-Path $tsDir) {
        $ts = Get-ChildItem $tsDir -Directory -ErrorAction SilentlyContinue |
            Where-Object { $_.Name -match '^v\d+$' } |
            Sort-Object { [int]($_.Name -replace '^v', '') } -Descending |
            Select-Object -First 1
        if ($ts) { $TargetToolset = $ts.Name; break }
    }
}
Write-Host "Using platform toolset: $TargetToolset" -ForegroundColor Cyan

$RootDir = $PSScriptRoot
$SlnFile = Join-Path $RootDir "KEE.sln"
$BinDir = Join-Path $RootDir "bin"
$OutDir = Join-Path $RootDir "build-release"

$Config = "Release"
$SolutionPlatforms = @("x64", "Win32")
$PlatformDirs = @{ x64 = "x64"; Win32 = "x86" }

function Build-Platform {
    param($SlnPlatform)

    $action = if ($Clean) { "Clean;Build" } else { "Build" }
    Write-Host "=== Building KEE ($SlnPlatform) ===" -ForegroundColor Cyan

    $args = @(
        $SlnFile,
        "/p:Configuration=$Config",
        "/p:Platform=$SlnPlatform",
        "/p:PlatformToolset=$TargetToolset",
        "/t:$action",
        "/v:q",
        "/m",
        "/nologo"
    )

    $output = & $MSBuild $args 2>&1 | Out-String
    $success = ($output -match "Build succeeded")

    if ($success) {
        $warnings = [regex]::Match($output, "(\d+) Warning").Groups[1].Value
        $errors   = [regex]::Match($output, "(\d+) Error").Groups[1].Value
        Write-Host "  PASS  W:$warnings E:$errors" -ForegroundColor Green
        return $true
    } else {
        Write-Host "  FAIL" -ForegroundColor Red
        $output | Select-String -Pattern "error " | Get-Unique |
            ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
        return $false
    }
}

function Copy-ToRelease {
    $releaseDir = Join-Path $OutDir "regular"
    $null = New-Item -ItemType Directory -Force -Path $releaseDir
    $null = New-Item -ItemType Directory -Force -Path (Join-Path $OutDir "cold_loader")
    $null = New-Item -ItemType Directory -Force -Path (Join-Path $OutDir "tools")

    # Emulator DLLs + proxy DLLs for both platforms
    foreach ($plat in @("x64", "x86")) {
        $srcDir = Join-Path $BinDir $plat $Config
        $dstDir = Join-Path $releaseDir $plat
        $null = New-Item -ItemType Directory -Force -Path $dstDir

        foreach ($file in @("EOSSDK-Win64-Shipping.dll", "EOSSDK-Win64-Shipping.lib",
                            "EOSSDK-Win32-Shipping.dll", "EOSSDK-Win32-Shipping.lib",
                            "winmm.dll", "version.dll")) {
            $src = Join-Path $srcDir $file
            if (Test-Path $src) { Copy-Item $src $dstDir -Force }
        }
    }

    # Cold loader EXEs + ini
    foreach ($name in @("cold_loader64.exe", "cold_loader.exe")) {
        $src = Get-ChildItem $BinDir -Recurse -Filter $name -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($src) { Copy-Item $src.FullName (Join-Path $OutDir "cold_loader/$($src.Name)") -Force }
    }
    $iniSrc = Join-Path $RootDir "src/cold_loader/cold_loader.ini"
    if (Test-Path $iniSrc) { Copy-Item $iniSrc (Join-Path $OutDir "cold_loader/cold_loader.ini") -Force }

    # Interface generator tools
    foreach ($name in @("generate_interfaces_x64.exe", "generate_interfaces.exe")) {
        $src = Get-ChildItem $BinDir -Recurse -Filter $name -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($src) { Copy-Item $src.FullName (Join-Path $OutDir "tools/$($src.Name)") -Force }
    }

    # Example settings
    $keeExample = Join-Path $RootDir "kee_settings.EXAMPLE"
    if (Test-Path $keeExample) {
        Copy-Item "$keeExample/*" (Join-Path $OutDir "kee_settings.EXAMPLE") -Recurse -Force
    }

    Write-Host "`n=== Release organized in: $OutDir ===" -ForegroundColor Yellow
}

# -- Main --
Write-Host "========================================" -ForegroundColor Yellow
Write-Host "  KEE Emulator Build" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Yellow

$allOk = $true
foreach ($plat in $SolutionPlatforms) {
    if (-not (Build-Platform $plat)) {
        $allOk = $false
    }
}

if ($allOk) {
    Write-Host "`n=== All builds passed ===" -ForegroundColor Green
    Copy-ToRelease
} else {
    Write-Host "`n=== Some builds FAILED ===" -ForegroundColor Red
    exit 1
}