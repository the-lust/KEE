# inventory_exports.ps1 - Export gap report for the KEE emulator.
#
# Compares the intended export list (def_functions.txt, scraped from real EOS
# SDK headers) against:
#   1. src/kee.def                 - actually exported symbols
#   2. source tree (src/**)        - functions with a real definition
#
# Output: a per-service summary plus a detailed list of exports that are
# declared in kee.def but have no definition in any .cpp file (link risk),
# and exports the real SDK has but kee.def does not (missing coverage).
#
# Usage: powershell -File tools/inventory_exports.ps1 [-Detailed]

param([switch]$Detailed)

$root = Split-Path -Parent $PSScriptRoot
$defFile = Join-Path $root "def_functions.txt"
$defExport = Join-Path $root "src\kee.def"

function Get-Lines($path) {
    if (-not (Test-Path $path)) { return @() }
    Get-Content $path | ForEach-Object { $_.Trim() } | Where-Object { $_ -ne "" -and $_ -notmatch "^;" -and $_ -notmatch "^(EXPORTS|LIBRARY)" }
}

$sdkFuncs    = Get-Lines $defFile
$exports     = Get-Lines $defExport

Write-Host "def_functions.txt (real SDK surface):  $($sdkFuncs.Count)"
Write-Host "kee.def exports:                       $($exports.Count)"

# --- 1. SDK functions not exported -----------------------------------------
$notExported = $sdkFuncs | Where-Object { $exports -notcontains $_ }
Write-Host ""
Write-Host "SDK functions NOT exported by kee.def: $($notExported.Count)"
if ($Detailed) { $notExported | ForEach-Object { Write-Host "  MISSING_EXPORT  $_" } }

# --- 2. Exports with no definition in any source file ----------------------
$cppFiles = Get-ChildItem (Join-Path $root "src") -Recurse -Include *.cpp
$defined = @{}
foreach ($file in $cppFiles) {
    foreach ($line in (Get-Content $file.FullName)) {
        if ($line -match 'EOS_DECLARE_FUNC\([^)]*\)\s*(?:[A-Za-z0-9_\s]*\s)?(EOS_[A-Za-z0-9_]+)\s*\(') { $defined[$Matches[1]] = $file.Name }
        if ($line -match '__declspec\s*\(\s*dllexport\s*\)\s*[^;(]*\b(EOS_[A-Za-z0-9_]+)\s*\(') { $defined[$Matches[1]] = $file.Name }
        if ($line -match '\b(EGS_Client_[A-Za-z0-9_]+)\s*\(') { $defined[$Matches[1]] = $file.Name }
    }
}
$noDef = $exports | Where-Object { -not $defined.ContainsKey($_) }

Write-Host ""
Write-Host "Exports with NO function definition found (link risk): $($noDef.Count)"
if ($Detailed) { $noDef | ForEach-Object { Write-Host "  NO_DEF  $_" } }

# --- 3. Summary by service prefix ------------------------------------------
Write-Host ""
Write-Host "Coverage by service (exports defined in source):"
$services = $sdkFuncs | ForEach-Object {
    if ($_ -match '^EOS_([A-Za-z]+)_') { $Matches[1].ToLower() } else { "misc" }
} | Sort-Object -Unique

foreach ($svc in $services) {
    $total  = ($sdkFuncs | Where-Object { $_ -match "^EOS_$svc" -or ($_ -eq "misc") } | Measure-Object).Count
    $definedCount = ($sdkFuncs | Where-Object { ($_ -match "^EOS_$svc" -or ($_ -eq "misc")) -and $defined.ContainsKey($_) } | Measure-Object).Count
    $exportedCount = ($sdkFuncs | Where-Object { ($_ -match "^EOS_$svc" -or ($_ -eq "misc")) -and $exports -contains $_ } | Measure-Object).Count
    "{0,-22} sdk={1,-4} exported={2,-4} defined={3,-4}" -f $svc, $total, $exportedCount, $definedCount
}

# --- 4. Write a machine-readable report -------------------------------------
$report = [ordered]@{
    sdk_functions_total  = $sdkFuncs.Count
    exports_total        = $exports.Count
    sdk_not_exported     = @($notExported)
    exports_no_definition = @($noDef)
}
$outFile = Join-Path $root "build\export_gap_report.json"
$null = New-Item -ItemType Directory -Force -Path (Split-Path $outFile)
$report | ConvertTo-Json -Depth 3 | Set-Content $outFile -Encoding UTF8
Write-Host ""
Write-Host "Report written to $outFile"