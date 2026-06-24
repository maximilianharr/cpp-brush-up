# cpp-refresh

Minimal C++ sample repository with:
- reusable headers in `include/`
- shared implementation files in `src/lib/`
- executable entry points in `src/`

Every `*.cpp` file placed directly in `src/` is built automatically as its own executable. Shared code belongs in `src/lib/`.

### Linux
```bash
clear && cmake -S . -B build && cmake --build build && ./build/main
```

### Windows
```powershell
clear; cmake -S . -B build; cmake --build build --config Release; .\build\Release\lesson01.exe
```