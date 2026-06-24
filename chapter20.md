# Chapter 20: Functions

Concise refresher based on the current LearnCpp Chapter 20 lesson list (quizzes omitted).  
Note: LearnCpp does not currently have a separate Chapter 20 section for `std::function`, so it is included where it naturally fits.

## 20.1 Function Pointers

- A **function pointer** stores the address of a function.
- The pointer type must match the function signature exactly.
- Function names decay to pointers in most expressions.
- Useful for callbacks, dispatch tables, and selecting behavior at runtime.
- Call through a pointer with `ptr(args)` or `(*ptr)(args)`.
- Compared to lambdas:
  - function pointers are simple and low-overhead
  - lambdas are usually clearer for local one-off behavior
  - capturing lambdas cannot convert to function pointers

```cpp
#include <iostream>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

using Op = int(*)(int, int);

int run(Op op, int x, int y)
{
    return op(x, y);
}

int main()
{
    Op op{add};
    std::cout << op(2, 3) << '\n';      // 5

    op = sub;
    std::cout << run(op, 7, 4) << '\n'; // 3
}
```

Common uses:
- tables of operations
- passing comparison or callback logic
- optional behavior selected at runtime

```cpp
void greet() { std::cout << "hi\n"; }

void runLater(void (*fn)())
{
    fn();
}
```

### Quick note: `std::function`

- `std::function` is a **type-erased callable wrapper**.
- It can store:
  - regular functions
  - function pointers
  - captureless lambdas
  - capturing lambdas
- It is more flexible than a raw function pointer, but may add some overhead.

```cpp
#include <functional>

std::function<int(int, int)> op{add};
op = [](int a, int b) { return a * b; };
```

Use:
- **function pointer** when you only need plain function-call semantics
- **lambda** for local custom behavior
- **`std::function`** when you need to store/pass arbitrary callables with one interface

## 20.2 The stack and the heap

- The **stack** holds automatic storage:
  - function parameters
  - local variables
  - bookkeeping for each call (stack frame)
- Stack allocation is fast, but stack space is limited.
- The **heap** is used for dynamic allocation (`new` / `delete`).
- Heap memory usually has longer/flexible lifetime, but allocation is slower and lifetime management matters.
- Deep recursion can exhaust the stack (**stack overflow**).

```cpp
#include <iostream>

int main()
{
    int local{42};          // stack
    int* p{new int{99}};    // heap

    std::cout << local << ' ' << *p << '\n';

    delete p;
    p = nullptr;
}
```

Each function call gets its own stack frame:

```cpp
void foo(int x)
{
    int y{x + 1}; // lives in foo's stack frame
}
```

Modern C++ preference:
- prefer automatic storage when possible
- prefer containers and smart pointers over raw `new` / `delete`

```cpp
#include <memory>

auto p{std::make_unique<int>(99)}; // heap, but ownership is automatic
```

Rule of thumb:
- short-lived local data -> stack
- runtime-sized / shared / long-lived dynamic data -> heap-managed objects

## 20.3 Recursion

- A **recursive function** calls itself.
- Every recursive algorithm needs:
  - a **base case** that stops recursion
  - a recursive step that makes progress toward that base case
- Each recursive call adds another stack frame.
- Missing or bad base cases lead to infinite recursion and likely stack overflow.

Classic example:

```cpp
int factorial(int n)
{
    if (n <= 1) // base case
        return 1;

    return n * factorial(n - 1); // recursive step
}
```

Tracing `factorial(4)`:
- `factorial(4)` -> `4 * factorial(3)`
- `factorial(3)` -> `3 * factorial(2)`
- `factorial(2)` -> `2 * factorial(1)`
- `factorial(1)` -> `1`

Another natural recursive pattern:

```cpp
void countdown(int n)
{
    if (n < 0)
        return;

    std::cout << n << '\n';
    countdown(n - 1);
}
```

When recursion fits well:
- tree traversal
- divide-and-conquer algorithms
- naturally self-similar problems

When iteration is often better:
- simple counting loops
- linear processing where recursion adds call overhead

Important:
- C++ does **not** guarantee tail-call optimization
- avoid recursion depth that depends on large unbounded input unless you know it is safe

## 20.4 Main and command line arguments

- `main` can take command line arguments:
  - `int argc`
  - `char* argv[]` (or equivalent `char** argv`)
- `argc` = argument count
- `argv` = array of C-style strings
- `argv[0]` is usually the program path/name
- User-supplied arguments start at `argv[1]`

```cpp
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "argc = " << argc << '\n';

    for (int i{0}; i < argc; ++i)
        std::cout << i << ": " << argv[i] << '\n';
}
```

Simple parse example:

```cpp
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: app <x> <y>\n";
        return 1;
    }

    int x{std::stoi(argv[1])};
    int y{std::stoi(argv[2])};

    std::cout << x + y << '\n';
}
```

Useful habits:
- validate `argc` before indexing `argv`
- convert strings carefully (`std::stoi`, `std::stoll`, or `std::from_chars`)
- print usage/help when the input is wrong

Typical uses:
- filenames
- configuration flags
- numeric inputs
- build/test tool options

## 20.5 Ellipsis and why to avoid them

- **Ellipsis** (`...`) enables old-style C variadic functions.
- These functions usually use `<cstdarg>`:
  - `va_list`
  - `va_start`
  - `va_arg`
  - `va_end`
- Problem: the compiler cannot type-check the extra arguments.
- You must manually know:
  - how many arguments exist
  - what types they have
- Type mismatches cause undefined behavior.

```cpp
#include <cstdarg>

int sum(int count, ...)
{
    va_list args;
    va_start(args, count);

    int total{};
    for (int i{0}; i < count; ++i)
        total += va_arg(args, int);

    va_end(args);
    return total;
}
```

Why to avoid ellipsis:
- no type safety
- easy to pass the wrong count
- default argument promotions can surprise you
- poor support for many modern C++ types
- harder to read and maintain

Prefer modern alternatives:
- overloads
- `std::initializer_list`
- containers/ranges
- variadic templates

```cpp
template <typename... Ts>
auto sum(Ts... values)
{
    return (values + ...);
}
```

Rule of thumb:
- use ellipsis only for C compatibility or rare low-level interfaces
- in normal C++, prefer type-safe alternatives

## 20.6 Introduction to lambdas (anonymous functions)

- A **lambda** is an unnamed function object created inline.
- Great for short local behavior, especially callbacks and predicates.
- Basic syntax:

```cpp
[captures](parameters) -> return_type
{
    body
}
```

Minimal examples:

```cpp
auto printHi = []()
{
    std::cout << "hi\n";
};

auto square = [](int x)
{
    return x * x;
};
```

Using a lambda:

```cpp
#include <algorithm>
#include <array>
#include <iostream>

int main()
{
    std::array values{1, 2, 3, 4, 5};

    auto countEven = std::count_if(values.begin(), values.end(),
        [](int x) { return x % 2 == 0; });

    std::cout << countEven << '\n';
}
```

Key points:
- lambdas are objects with callable `operator()`
- each lambda has a unique compiler-generated type
- captureless lambdas can often convert to function pointers
- lambdas are usually better than function pointers for short, local custom logic

Generic lambda:

```cpp
auto less = [](const auto& a, const auto& b)
{
    return a < b;
};
```

Use a named function instead when:
- the logic is reused widely
- the body is long enough to deserve a real name

## 20.7 Lambda captures

- A lambda can **capture** variables from the surrounding scope.
- Capture by value:
  - `[x]`
  - makes a copy of `x`
- Capture by reference:
  - `[&x]`
  - uses the original object
- Capture all by value: `[=]`
- Capture all by reference: `[&]`

By-value vs by-reference:

```cpp
int x{10};

auto byValue = [x]()
{
    std::cout << x << '\n'; // copied value
};

auto byRef = [&x]()
{
    ++x; // modifies outer x
};
```

If a by-value capture needs to be modified inside the lambda, use `mutable`:

```cpp
int x{10};

auto f = [x]() mutable
{
    ++x;               // modifies the lambda's copy
    return x;
};
```

Capturing multiple ways:

```cpp
int a{1};
int b{2};

auto f1 = [=]() { return a + b; };   // all by value
auto f2 = [&]() { return ++a + ++b; }; // all by reference
auto f3 = [a, &b]() { return a + ++b; };
```

Useful modern pattern: init capture

```cpp
#include <memory>

auto ptr{std::make_unique<int>(42)};

auto f = [p = std::move(ptr)]()
{
    return *p;
};
```

Important lifetime rule:
- do **not** return/store a lambda with reference captures if the referenced objects may die first

```cpp
// bad: returned lambda would dangle if it captured locals by reference
```

### `std::function` with lambdas

- Capturing lambdas cannot convert to plain function pointers.
- `std::function` can store them cleanly:

```cpp
#include <functional>

int offset{5};

std::function<int(int)> addOffset = [offset](int x)
{
    return x + offset;
};
```

Capture guidance:
- capture only what you need
- prefer explicit captures over broad `[=]` / `[&]` when clarity matters
- avoid `[&]` for lambdas that escape the current scope
