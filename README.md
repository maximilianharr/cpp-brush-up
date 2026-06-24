# C++ brush up

Repo to help you quickly refresh you C++ coding skills using [Learn C++](https://learncpp.com/).

## How to use

### IDE

We recommend using VS Code. Open a chapter**.md in preview mode. Create the respective chapter**.cpp. Apply what you learned.

![usage example](docs/imgs/usage-example.png)

### Coding / Compilation

Linux
```bash
cd coding
clear && cmake -S . -B build && cmake --build build && ./build/chapter00
```

Windows
```powershell
cd coding
clear; cmake -S . -B build; cmake --build build --config Release; .\build\Release\lesson01.exe
```
