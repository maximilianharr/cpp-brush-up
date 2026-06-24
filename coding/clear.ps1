param()

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

$directoryPatterns = @(
    "build",
    "CMakeFiles",
    "Debug",
    "Release",
    "x64",
    "*.dir"
)

$filePatterns = @(
    "CMakeCache.txt",
    "cmake_install.cmake",
    "*.sln",
    "*.vcxproj",
    "*.vcxproj.filters"
)

foreach ($pattern in $directoryPatterns) {
    Get-ChildItem -Path (Join-Path $scriptDir $pattern) -Force -ErrorAction SilentlyContinue |
        Where-Object { $_.PSIsContainer } |
        ForEach-Object { Remove-Item -LiteralPath $_.FullName -Recurse -Force }
}

foreach ($pattern in $filePatterns) {
    Get-ChildItem -Path (Join-Path $scriptDir $pattern) -Force -File -ErrorAction SilentlyContinue |
        ForEach-Object { Remove-Item -LiteralPath $_.FullName -Force }
}
