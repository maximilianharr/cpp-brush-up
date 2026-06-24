param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release"
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildDir = Join-Path $scriptDir "build"

cmake -S $scriptDir -B $buildDir
cmake --build $buildDir --config $Configuration

$preferredExe = Join-Path $buildDir "$Configuration\main.exe"
if (Test-Path $preferredExe) {
    & $preferredExe
    exit 0
}

$exe = Get-ChildItem -Path $buildDir -Recurse -File -Filter *.exe |
    Where-Object { $_.FullName -notmatch "\\CMakeFiles\\" } |
    Sort-Object LastWriteTime -Descending |
    Select-Object -First 1

if (-not $exe) {
    throw "No executable was produced in '$buildDir'."
}

& $exe.FullName
