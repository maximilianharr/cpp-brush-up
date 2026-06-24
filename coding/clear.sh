#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

shopt -s nullglob

dir_patterns=(
  "build"
  "CMakeFiles"
  "Debug"
  "Release"
  "x64"
  "*.dir"
)

file_patterns=(
  "CMakeCache.txt"
  "cmake_install.cmake"
  "*.sln"
  "*.vcxproj"
  "*.vcxproj.filters"
)

for pattern in "${dir_patterns[@]}"; do
  for path in "$script_dir"/$pattern; do
    [[ -d "$path" ]] && rm -rf "$path"
  done
done

for pattern in "${file_patterns[@]}"; do
  for path in "$script_dir"/$pattern; do
    [[ -f "$path" ]] && rm -f "$path"
  done
done
