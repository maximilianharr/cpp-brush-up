# Chapter 27: Exceptions

## 27.1 The need for exceptions
[The need for exceptions](https://www.learncpp.com/cpp-tutorial/the-need-for-exceptions/)
- Exceptions are for errors/failures that interrupt the normal flow of a program.
- They separate:
  - normal result handling
  - error detection
  - error recovery
- This is often cleaner than threading error codes through every caller.
- Exceptions are especially useful when:
  - a constructor cannot produce a valid object
  - an operator/function has no natural error return value
  - many stack frames would otherwise just forward an error upward
- Core model:
  1. detect failure
  2. `throw` an exception object
  3. control transfers to the nearest matching `catch`
- Use exceptions for exceptional situations, not routine branching.
- Good fits:
  - failed file open for required input
  - invalid constructor arguments
  - allocation/resource failures
- Poor fits:
  - loop termination
  - "not found" when that is expected
  - ordinary validation branches in hot paths
- Exceptions work best with RAII:
  - objects clean themselves up in destructors
  - cleanup still happens if control exits early

```cpp
#include <stdexcept>

double reciprocal(double x)
{
    if (x == 0.0)
        throw std::runtime_error{"cannot take reciprocal of zero"};

    return 1.0 / x;
}
```

- Compare the "happy path":
  - without exceptions: every caller may need to inspect a status code
  - with exceptions: normal code stays focused on the normal case
- The tradeoff is hidden control flow; readers must know a call can throw.

## 27.2 Throwing exceptions
[Basic exception handling](https://www.learncpp.com/cpp-tutorial/basic-exception-handling/)
- Throw with `throw expression;`.
- The expression creates/copies an exception object.
- Throw by value, catch by reference.
- Prefer exception types that carry meaning:
  - `std::runtime_error`
  - `std::invalid_argument`
  - `std::out_of_range`
  - custom types for domain-specific failures
- Avoid throwing:
  - raw owning pointers
  - vague integers/enums unless a codebase explicitly standardizes that
  - types that lose context
- A thrown exception aborts the current statement sequence immediately.
- Code after the `throw` in that path does not run.
- If no matching handler exists in the current function, the exception keeps propagating outward.

```cpp
#include <stdexcept>
#include <string_view>
#include <vector>

int getValue(const std::vector<int>& values, std::size_t index)
{
    if (index >= values.size())
        throw std::out_of_range{"index out of range"};

    return values[index];
}

void setUserName(std::string_view name)
{
    if (name.empty())
        throw std::invalid_argument{"name must not be empty"};
}
```

- Throw early when an invariant is broken.
- Add enough context for debugging/logging.
- Standard library exception constructors commonly take a message string.
- Prefer small, descriptive exception types over giant error structs.
- If your API can fail in several distinct ways, choose exception types that let callers react differently.

## 27.3 Catching exceptions
[Exceptions, functions, and stack unwinding](https://www.learncpp.com/cpp-tutorial/exceptions-functions-and-stack-unwinding/)
- Handle exceptions with `try` / `catch`.
- Put potentially-throwing code inside a `try` block.
- Attach one or more `catch` handlers after it.
- Matching chooses the first compatible handler.
- Catch most-derived types before base types.
- Usually catch as `const T&`:
  - avoids copies
  - avoids slicing
  - preserves dynamic type information
- `catch (...)` is a catch-all handler:
  - useful for cleanup/logging boundaries
  - cannot directly inspect the exception object
- If no handler is found, the program calls `std::terminate`.

```cpp
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

struct Guard
{
    std::string name;
    ~Guard() { std::cout << "destroying " << name << '\n'; }
};

void low()
{
    Guard g{"low"};
    throw std::runtime_error{"disk read failed"};
}

void mid()
{
    Guard g{"mid"};

    try
    {
        low();
    }
    catch (...)
    {
        std::cerr << "mid(): logging and rethrowing\n";
        throw; // preserve original exception
    }
}

int main()
{
    Guard g{"main"};

    try
    {
        mid();
    }
    catch (const std::exception& e)
    {
        std::cerr << "handled: " << e.what() << '\n';
    }
}
```

- When `low()` throws, stack unwinding begins:
  - `low` locals are destroyed
  - then `mid` locals are destroyed as the stack unwinds
  - then control reaches the matching `catch`
- This is why RAII is so important under exceptions.
- Rethrow with plain `throw;` inside a handler.
- `throw e;` throws a new copy and may slice/change the dynamic type.
- Keep `try` blocks narrow when practical; it makes intended recovery clearer.

## 27.4 Exception specifications (noexcept)
[Exception specifications and noexcept](https://www.learncpp.com/cpp-tutorial/exception-specifications-and-noexcept/)
- Old dynamic exception specifications (`throw(Type)`) are removed from modern C++ practice.
- The important modern tool is `noexcept`.
- `noexcept` means a function promises that no exception will escape it.
- If an exception does escape a `noexcept` function, the program calls `std::terminate`.
- Use `noexcept` when the guarantee is real and useful.
- Common good candidates:
  - destructors
  - `swap`
  - move constructor / move assignment for resource-owning types
  - small utility functions that cannot fail
- The standard library prefers `noexcept` moves in some optimizations (for example, vector reallocation).
- `noexcept(expr)` is a compile-time check that evaluates to `true` if the expression is known not to throw.

```cpp
#include <stdexcept>
#include <utility>

class Buffer
{
public:
    Buffer() = default;
    Buffer(Buffer&&) noexcept = default;
    Buffer& operator=(Buffer&&) noexcept = default;
};

void cleanup() noexcept
{
    // do not let exceptions escape from here
}

void risky() noexcept
{
    throw std::runtime_error{"boom"}; // calls std::terminate
}

static_assert(noexcept(Buffer{std::declval<Buffer&&>()}));
```

- Do not mark a function `noexcept` just because you hope it will not throw.
- `noexcept` is part of interface/optimization behavior.
- A false promise is worse than no promise.
- Practical rule:
  - use `noexcept` for operations that truly must not fail
  - leave it off if failure propagation is still needed

## 27.5 Exceptions, classes, and inheritance
[Exceptions, classes, and inheritance](https://www.learncpp.com/cpp-tutorial/exceptions-classes-and-inheritance/)
- Exception objects are just objects, so classes work naturally as exception types.
- Inheritance lets you group related failures and catch them polymorphically.
- The standard library base type is `std::exception`.
- Many standard exceptions derive from it:
  - `std::runtime_error`
  - `std::logic_error`
  - `std::invalid_argument`
  - `std::out_of_range`
  - `std::bad_alloc`
- Catch by `const std::exception&` when you want a broad handler.
- Catch a more specific derived type first when recovery differs by failure kind.
- If you catch by value, derived objects can be sliced down to the base type.

```cpp
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

class ConfigError : public std::exception
{
private:
    std::string m_message{};

public:
    explicit ConfigError(std::string message)
        : m_message{std::move(message)}
    {
    }

    const char* what() const noexcept override
    {
        return m_message.c_str();
    }
};

void loadConfig(std::string_view path)
{
    if (path.empty())
        throw ConfigError{"config path was empty"};
}

int main()
{
    try
    {
        loadConfig("");
    }
    catch (const ConfigError& e)
    {
        std::cerr << "config error: " << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "generic std::exception: " << e.what() << '\n';
    }
}
```

- Typical custom-exception pattern:
  - derive from `std::exception`
  - store a stable message
  - override `what() const noexcept`
- Prefer semantic exception types when callers may want selective handling.
- Prefer a broad `std::exception` catch near top-level program boundaries.

## 27.6 Exceptions and constructors/destructors
[Function try blocks](https://www.learncpp.com/cpp-tutorial/function-try-blocks/)
- Constructors can throw if they cannot establish a valid object.
- If a constructor throws:
  - the object is considered not fully constructed
  - its own destructor does not run
  - already-constructed base/member subobjects are destroyed
- This again rewards RAII:
  - members clean up themselves automatically
  - partial construction does not leak if resources live in member objects
- Destructors should generally not throw.
- Throwing during stack unwinding can lead to `std::terminate`.
- In practice, destructors are usually written `noexcept` and handle internal failures another way:
  - swallow/log
  - set an error state
  - expose an explicit `close()`/`commit()` API that can report failure
- Function try blocks are mainly useful on constructors because they can catch exceptions from base/member initialization.

```cpp
#include <exception>
#include <iostream>
#include <stdexcept>

struct FileHandle
{
    FileHandle() { std::cout << "open file\n"; }
    ~FileHandle() noexcept { std::cout << "close file\n"; }
};

struct Parser
{
    Parser() { throw std::runtime_error{"parser init failed"}; }
};

class Widget
{
private:
    FileHandle m_file{};
    Parser m_parser{};

public:
    Widget() try
        : m_file{}, m_parser{}
    {
    }
    catch (const std::exception& e)
    {
        std::cerr << "Widget construction failed: " << e.what() << '\n';
        throw;
    }

    ~Widget() noexcept = default;
};
```

- In the example:
  - `m_file` constructs successfully
  - `m_parser` throws
  - `Widget` is never fully created
  - `m_file` is still destroyed during unwinding
- This is the key mental model for constructor failures.

## 27.7 Exception handling and program performance
[Exception dangers and downsides](https://www.learncpp.com/cpp-tutorial/exception-dangers-and-downsides/)
- Exceptions improve separation of normal code from recovery code, but they are not free.
- Downsides:
  - control flow becomes less explicit
  - throwing is relatively expensive
  - code is harder to reason about/test if exceptions can emerge from many layers
  - interfaces need documented exception guarantees
  - careless use can hide ordinary program logic inside error handling
- Performance rule of thumb:
  - not throwing is often cheap
  - actually throwing/unwinding is expensive
- So exceptions are best for rare failures, not common branches.
- Prefer other tools when failure is expected/ordinary:
  - `bool`
  - error code / enum
  - `std::optional`
  - `std::expected` (where available)
- Good exception habits:
  - keep objects valid under failure
  - use RAII for cleanup
  - catch by `const&`
  - throw meaningful types
  - avoid throwing from destructors
  - do not swallow exceptions silently unless that is a deliberate boundary policy

```cpp
#include <optional>
#include <stdexcept>
#include <string_view>

std::optional<int> findCachedUserId(std::string_view name)
{
    return std::nullopt; // cache miss is normal, so no exception
}

int loadRequiredUserId(std::string_view name)
{
    throw std::runtime_error{"required user missing"}; // exceptional failure
}
```

- Use exceptions when callers usually expect success and need a separate path for rare failure.
- Use non-exception return-based APIs when "failure" is a normal, frequent outcome.
- Final mental checklist:
  - exceptions for exceptional failure
  - RAII for cleanup
  - `noexcept` for true no-fail operations
  - `std::exception` hierarchy for interoperable error types
