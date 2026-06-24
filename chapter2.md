# Chapter 2: C++ Basics: Functions and Files

## 2.1 Introduction to functions
- A **function** is a named block of code that performs one task.
- Functions help you split programs into smaller, reusable pieces.
- Every program starts in `main()`.
- Basic parts:
  - **return type**
  - **function name**
  - **parameter list**
  - **function body**
- Calling a function transfers control to it, then back to the caller.

```cpp
#include <iostream>

void sayHi()
{
    std::cout << "Hi!\n";
}

int main()
{
    sayHi();
    return 0;
}
```

## 2.2 Function return values (value-returning functions)
- A value-returning function sends a result back to the caller with `return`.
- The returned value must match the function's return type.
- Returned values can be printed, stored, or used in expressions.
- Reaching the end of a non-`void` function without returning a value is an error/bug.

```cpp
#include <iostream>

int getMeaning()
{
    return 42;
}

int add(int x, int y)
{
    return x + y;
}

int main()
{
    int value { getMeaning() };
    std::cout << add(value, 8) << '\n';
    return 0;
}
```

## 2.3 Void functions (non-value returning functions)
- A `void` function performs an action but does not produce a result.
- Use `return;` only if you want to exit early.
- Good for printing, prompting, logging, or other side-effect work.

```cpp
#include <iostream>

void printBanner()
{
    std::cout << "=== App start ===\n";
}

void printIfPositive(int x)
{
    if (x <= 0)
        return;

    std::cout << x << " is positive\n";
}

int main()
{
    printBanner();
    printIfPositive(5);
    return 0;
}
```

## 2.4 Introduction to function parameters and arguments
- A **parameter** is a variable in a function declaration/definition.
- An **argument** is the value passed into the function call.
- Parameter count, order, and types should match the call.
- Parameters let one function work with many inputs.

```cpp
#include <iostream>

int multiply(int a, int b) // a and b are parameters
{
    return a * b;
}

int main()
{
    std::cout << multiply(3, 4) << '\n'; // 3 and 4 are arguments
    return 0;
}
```

## 2.5 Introduction to local scope
- Variables declared inside a function or block have **local scope**.
- Local names are usable only from their declaration point to the end of the block.
- Locals are created when the block begins execution and destroyed when it ends.
- Function parameters are local to the function too.
- Separate functions can use the same local variable names without conflict.

```cpp
#include <iostream>

void demo(int value)
{
    int doubled { value * 2 };
    std::cout << doubled << '\n';
}

int main()
{
    demo(7);

    if (true)
    {
        int temp { 1 };
        std::cout << temp << '\n';
    }

    // temp is not visible here
    return 0;
}
```

## 2.6 Why functions are useful, and how to use them effectively
- Functions reduce duplication and keep logic centralized.
- They make code easier to read, test, debug, and change.
- Prefer functions that do **one clear job**.
- Use descriptive names and simple interfaces.
- Break large problems into smaller helper functions.
- If code repeats, consider extracting a function.

```cpp
#include <iostream>

int square(int x)
{
    return x * x;
}

void printSquare(int x)
{
    std::cout << "square(" << x << ") = " << square(x) << '\n';
}

int main()
{
    printSquare(3);
    printSquare(5);
    return 0;
}
```

## 2.7 Forward declarations and definitions
- The compiler must know a function exists before it is called.
- A **forward declaration** introduces the function signature before the definition.
- A **definition** provides the actual body.
- Common pattern: declare first, define later.

```cpp
#include <iostream>

int add(int x, int y); // forward declaration

int main()
{
    std::cout << add(2, 3) << '\n';
    return 0;
}

int add(int x, int y) // definition
{
    return x + y;
}
```

## 2.8 Programs with multiple code files
- Real programs are usually split across multiple `.cpp` files.
- Each source file is compiled separately, then the **linker** combines them.
- Put declarations in headers and definitions in source files.
- Include the header wherever the declarations are needed.

```cpp
// add.h
int add(int x, int y);

// add.cpp
#include "add.h"

int add(int x, int y)
{
    return x + y;
}

// main.cpp
#include <iostream>
#include "add.h"

int main()
{
    std::cout << add(10, 20) << '\n';
}
```

## 2.9 Naming collisions and an introduction to namespaces
- A **naming collision** happens when two things with the same name are visible in the same scope.
- **Namespaces** group names to avoid collisions.
- The standard library lives in `std`.
- Use the scope resolution operator `::` to qualify names.
- Avoid `using namespace std;` in headers and generally avoid broad global `using` directives.

```cpp
#include <iostream>

namespace math
{
    int value()
    {
        return 10;
    }
}

namespace config
{
    int value()
    {
        return 20;
    }
}

int main()
{
    std::cout << math::value() << ' ' << config::value() << '\n';
    return 0;
}
```

## 2.10 Introduction to the preprocessor
- The **preprocessor** runs before compilation.
- It handles lines beginning with `#`.
- Common directives:
  - `#include` — inserts another file
  - `#define` — creates macros
  - `#ifdef`, `#ifndef`, `#endif` — conditional compilation
- Preprocessor work is mostly textual substitution; it is not type-aware.

```cpp
#include <iostream>

#define APP_NAME "Demo"

int main()
{
    std::cout << APP_NAME << '\n';
    return 0;
}
```

## 2.11 Header files
- Header files usually contain **declarations** shared across source files.
- Typical contents:
  - function declarations
  - type declarations
  - constants
  - templates / inline code when appropriate
- Include user headers with quotes: `#include "file.h"`
- Include standard headers with angle brackets: `#include <iostream>`
- Avoid putting normal non-inline function definitions in headers.

```cpp
// math.h
#ifndef MATH_H
#define MATH_H

int add(int x, int y);
int subtract(int x, int y);

#endif

// main.cpp
#include "math.h"
```

## 2.12 Header guards
- If the same header is included more than once in one translation unit, duplicate declarations/definitions may cause errors.
- **Header guards** prevent repeated inclusion.
- Classic pattern:
  - `#ifndef`
  - `#define`
  - header contents
  - `#endif`
- `#pragma once` is a widely supported alternative with cleaner syntax.

```cpp
// classic guards
#ifndef PERSON_H
#define PERSON_H

struct Person
{
    int age {};
};

#endif
```

```cpp
// alternative
#pragma once

struct Person
{
    int age {};
};
```

## 2.13 How to design your first programs
- Start with the problem, not the code.
- Define:
  - required inputs
  - expected outputs
  - core steps
- Break the task into smaller pieces.
- Write the program in stages and test each stage.
- Use functions to separate input, processing, and output.
- Keep the first version simple, then refine.

```cpp
#include <iostream>

int readNumber()
{
    int x {};
    std::cin >> x;
    return x;
}

int add(int a, int b)
{
    return a + b;
}

int main()
{
    std::cout << "Enter two numbers: ";
    int a { readNumber() };
    int b { readNumber() };
    std::cout << "Sum: " << add(a, b) << '\n';
    return 0;
}
```

- Simple design workflow:
  1. State the problem clearly.
  2. List steps in plain English or pseudocode.
  3. Turn each step into code.
  4. Extract repeated logic into functions.
  5. Compile, test, and clean up.
