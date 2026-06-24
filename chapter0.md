# Chapter 0: Introduction / Getting Started

## 0.1 Introduction to these tutorials
- LearnCpp is structured as a step-by-step path from absolute beginner topics to modern C++ practice.
- The goal is not just syntax memorization, but understanding how C++ programs are built, run, and debugged.
- Expect to learn by reading, typing code, compiling, breaking things, and fixing them.
- Small gaps compound quickly in C++, so it is worth understanding each foundation topic before rushing ahead.
- Use the tutorials as active practice: write examples yourself, experiment, and verify behavior.
- Modern C++ style matters: prefer current language features and standard-library solutions over older C-style habits.

## 0.2 Introduction to programs and programming languages
- A **program** is a set of instructions that tells a computer what to do.
- Source code is written in a human-readable programming language; computers ultimately run machine code.
- A programming language gives you a vocabulary and rules for expressing logic precisely.
- Programs usually:
  - take input,
  - process data,
  - produce output.
- Programming is partly about solving problems, and partly about expressing solutions clearly enough for a compiler and computer to execute.
- Bugs are normal:
  - **syntax errors**: invalid code,
  - **runtime errors**: problems while running,
  - **logic errors**: program runs, but gives the wrong result.

## 0.3 Introduction to C/C++
- C++ grew out of C, but modern C++ is its own language with much stronger abstraction and library support.
- C is smaller and closer to the hardware model; C++ adds features for safer, larger, and more expressive programs.
- C++ supports multiple styles:
  - procedural,
  - object-oriented,
  - generic,
  - functional-leaning patterns.
- C++ is widely used where performance and control matter: games, engines, finance, embedded, tooling, browsers, graphics, and infrastructure.
- Prefer writing **idiomatic modern C++**, not “C with classes”.
- Portability is a major strength when you stay within the language standard and standard library.

## 0.4 Introduction to C++ development
- A typical C++ workflow is:
  1. write source code,
  2. compile it,
  3. link it,
  4. run it,
  5. debug/fix it,
  6. repeat.
- Core tools:
  - **editor / IDE**: write code,
  - **compiler**: translate source to object code,
  - **linker**: combine pieces into an executable,
  - **debugger**: inspect program behavior.
- Common file types:
  - `.cpp` / `.cc`: source files,
  - `.h` / `.hpp`: headers.
- Real programs are often split across multiple files and built as a project rather than one giant source file.

## 0.5 Introduction to the compiler, linker, and libraries
- The **compiler** translates each source file into an object file and reports compile-time errors.
- The **linker** combines object files and required libraries into the final executable.
- A **library** is prewritten code you can reuse instead of writing everything from scratch.
- The C++ **standard library** provides essentials such as I/O, strings, containers, algorithms, and utilities.
- Rough pipeline:
  - source files -> compiler -> object files
  - object files + libraries -> linker -> executable
- Failure types:
  - **compile error**: source code is invalid,
  - **link error**: declarations exist, but definitions are missing,
  - **runtime problem**: executable built, but behavior is wrong.

## 0.6 Installing an Integrated Development Environment (IDE)
- An **IDE** bundles editor, compiler integration, build commands, and debugger into one workflow.
- Use a mainstream toolchain for your platform:
  - **Windows**: Visual Studio Community,
  - **macOS**: Xcode or another IDE using Clang,
  - **Linux**: a GCC/Clang-based setup with an IDE/editor you like.
- Important installation detail: make sure the **C++ toolchain/workload** is actually installed, not just the editor shell.
- After installation, verify you can create a simple console project, build it, and run it.
- If something fails early, it is usually a toolchain/setup issue rather than a C++ language issue.

## 0.7 Compiling your first program
- Start with a minimal console program and make sure the full build/run loop works.
- Typical loop:
  1. create project,
  2. write code,
  3. save,
  4. build,
  5. fix errors,
  6. run.
- Minimal example:

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello, world!\n";
    return 0;
}
```

- Expected result: the program compiles successfully and prints one line.
- If it does not compile, read the **first** error carefully; later errors are often just fallout.
- Getting this first program working proves your editor, compiler, linker, and run configuration are all basically usable.

## 0.8 A few common C++ problems
- **Console window opens and closes immediately**
  - Prefer running from a terminal or using **Start Without Debugging** in your IDE.
  - For temporary local testing, you can pause at the end of `main()`.
- **“unresolved external symbol main” / missing `main`**
  - Your program entry point is missing, misspelled, or the project type is wrong.
- **multiple definitions of `main`**
  - A program can only have one `main()`.
- **Compiles, but no visible output**
  - Sometimes execution is being blocked by environment/setup issues.
- **Compiles, but behaves incorrectly**
  - That is a debugging problem, not a compiler problem.
- Temporary pause example:

```cpp
#include <iostream>
#include <limits>

int main()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
```

- Avoid `system("pause")`: it is non-portable and not good practice.

## 0.9 Configuring your compiler: Build configurations
- The two common build configurations are **Debug** and **Release**.
- **Debug**:
  - little or no optimization,
  - full debug information,
  - easier stepping, inspection, and troubleshooting.
- **Release**:
  - optimizations enabled,
  - faster/smaller executable,
  - debugging is harder and behavior can differ if your code has undefined behavior.
- Rule of thumb:
  - develop mostly in **Debug**,
  - also test in **Release** before considering code “done”.
- Do not assume “works in Debug” means “works everywhere”.

## 0.10 Configuring your compiler: Compiler extensions
- **Compiler extensions** are non-standard features a compiler accepts beyond standard C++.
- They can be convenient, but they reduce portability and can hide bad assumptions.
- If code depends on one compiler’s extensions, it may fail on another compiler or platform.
- Best practice for learning and reusable code:
  - prefer standard C++,
  - disable language extensions or enable standards-conformance mode where practical.
- Use extensions only intentionally, with a clear portability tradeoff.

## 0.11 Configuring your compiler: Warning and error levels
- Turn warning levels **up**, not down.
- Warnings catch suspicious code that is legal enough to compile but still likely wrong.
- Good defaults:
  - **MSVC**: `/W4` (optionally `/WX` to treat warnings as errors),
  - **GCC/Clang**: `-Wall -Wextra` (optionally `-Werror`).
- Fix warnings early; ignored warnings often become real bugs later.
- A clean build means:
  - no errors,
  - ideally no warnings.
- Treat compiler diagnostics as feedback, not noise.

## 0.12 Configuring your compiler: Choosing a language standard
- C++ evolves through named standards such as C++11, C++14, C++17, C++20, and C++23.
- Do not rely on your compiler’s default standard; set the project standard explicitly.
- In practice, choose the newest stable standard your compiler and project support.
- Typical flags:
  - **GCC/Clang**: `-std=c++20` or `-std=c++23`
  - **MSVC**: `/std:c++20` or `/std:c++latest`
- Setting the standard explicitly helps with:
  - predictable behavior,
  - modern features,
  - portability across machines and toolchains.

## 0.13 What language standard is my compiler using?
- The active standard can be checked in project settings or by inspecting `__cplusplus`.
- Handy probe:

```cpp
#include <iostream>

int main()
{
    std::cout << __cplusplus << '\n';
}
```

- Common values:
  - `201703L` -> C++17
  - `202002L` -> C++20
  - `202302L` -> C++23
- MSVC caveat: `__cplusplus` may be inaccurate unless `/Zc:__cplusplus` is enabled.
- If the reported value is older than expected, check your project settings and compiler flags first.
