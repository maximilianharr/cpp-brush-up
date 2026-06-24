#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
build_dir="$script_dir/build"

cmake -S "$script_dir" -B "$build_dir" -DCMAKE_BUILD_TYPE=Release
cmake --build "$build_dir" --config Release

if [[ -x "$build_dir/main" ]]; then
  app="$build_dir/main"
else
  app="$(find "$build_dir" -maxdepth 3 -type f -perm -111 ! -path "*/CMakeFiles/*" | head -n 1)"
fi

if [[ -z "${app:-}" ]]; then
  echo "No executable was produced in '$build_dir'." >&2
  exit 1
fi

"$app"
