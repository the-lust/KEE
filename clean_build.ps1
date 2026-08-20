# Clean build script
# Removes all obj/ and bin/ artifacts, then does a full rebuild

Write-Host "=== Cleaning all build artifacts ===" -ForegroundColor Yellow

$RootDir = $PSScriptRoot

# Clean MSBuild obj/bin
foreach ($dir in @("obj", "bin")) {
    $full = Join-Path $RootDir $dir
    if (Test-Path $full) {
        Remove-Item -Recurse -Force $full -ErrorAction SilentlyContinue
        Write-Host "  Removed: $full" -ForegroundColor Gray
    }
}

# Clean build-release
$release = Join-Path $RootDir "build-release"
if (Test-Path $release) {
    Remove-Item -Recurse -Force $release -ErrorAction SilentlyContinue
    Write-Host "  Removed: $release" -ForegroundColor Gray
}

Write-Host "`n=== Clean complete. Run .\build_all.ps1 to rebuild. ===" -ForegroundColor Green
