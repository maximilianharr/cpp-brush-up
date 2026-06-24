# Chapter 7: Scope, Duration, and Linkage

## 7.1 Introduction to scope, duration, and linkage
- **Scope**: where an identifier can be used.
- **Duration**: how long an object exists in memory.
- **Linkage**: whether the same name can refer to the same object/function across scopes or files.
- These three ideas explain most variable visibility and lifetime behavior in C++.
- Rough intuition:
  - **local variable**: small scope, automatic duration, no linkage
  - **global variable**: wide scope, static duration, often external linkage
  - **static local**: local scope, static duration, no linkage
  - **file-local global**: namespace scope, static duration, internal linkage

```cpp
int g_value{42};          // namespace scope, static duration

void f()
{
    int local{1};         // block scope, automatic duration
    static int count{0};  // block scope, static duration
    ++count;
}
```

## 7.2 Local variables, scope, and duration
- Local variables live inside functions, parameters, and blocks.
- Their **scope** usually starts at declaration and ends at the closing brace of the block.
- Their **duration** is usually **automatic**: created on block entry, destroyed on block exit.
- Inner blocks can access names from outer blocks unless shadowed.
- Prefer the **smallest practical scope**; it improves clarity and reduces bugs.

```cpp
void print()
{
    int x{5};          // visible from here to end of function

    if (x > 0)
    {
        int y{10};     // visible only inside this block
        std::cout << x + y << '\n';
    }

    // y is gone here
    std::cout << x << '\n';
}
```

## 7.3 Static duration variables
- Objects with **static duration** are created once and live until program termination.
- Globals and static locals have static duration.
- A **static local** keeps its value between function calls.
- Static locals are initialized once, then reused.
- Great for counters, cached state, one-time setup helpers; use carefully to avoid hidden state.

```cpp
int nextId()
{
    static int s_id{0};  // initialized once
    return ++s_id;       // remembers prior value
}

// returns 1, then 2, then 3...
```

## 7.4 Global variables and linkage
- Global variables are declared at **namespace scope** (outside functions).
- They have **static duration**.
- Non-const globals usually have **external linkage** by default.
- They are easy to reach, but that convenience causes problems:
  - hidden dependencies
  - harder testing
  - accidental modification from far away
  - tighter coupling between files
  - initialization-order headaches across translation units
- Prefer passing data explicitly; use globals sparingly.

```cpp
int g_mode{1};  // global variable

void run()
{
    if (g_mode == 1)
        std::cout << "fast\n";
}
```

## 7.5 Internal linkage
- **Internal linkage** means a name is usable only within the current translation unit (`.cpp` file).
- Good for helpers and globals that should not be shared across files.
- Namespace-scope `const` objects have internal linkage by default.
- `static` at namespace scope also gives internal linkage.
- Unnamed namespaces are a common modern alternative for file-local names.

```cpp
static int s_fileCounter{0};   // only this .cpp can see it
const int bufferSize{1024};    // internal linkage by default

static void logMessage()
{
    ++s_fileCounter;
}
```

## 7.6 External linkage
- **External linkage** means the same name can be referred to from other translation units.
- Most non-const namespace-scope variables and normal functions have external linkage by default.
- Use `extern` for a declaration when the definition lives elsewhere.
- A variable with external linkage should have **one definition** program-wide.
- Put declarations in headers; put the definition in one source file.

```cpp
// config.h
extern int g_limit;

// config.cpp
int g_limit{100};

// main.cpp
#include "config.h"
std::cout << g_limit << '\n';
```

## 7.7 The static keyword (in global and local scope)
- `static` has different meanings depending on context.
- On a **local variable**: gives **static duration**.
- On a **namespace-scope variable/function**: gives **internal linkage**.
- Same keyword, different effect; the context matters.
- This is why `static` often feels overloaded in C++.

```cpp
void tick()
{
    static int calls{0};  // persists across calls
    ++calls;
}

static int s_state{0};    // file-local global
```

## 7.8 Variable shadowing (name hiding)
- An inner declaration can **shadow** an outer declaration with the same name.
- Once shadowed, the outer name is hidden inside the inner scope.
- Shadowing can happen with local variables, parameters, and globals.
- It is legal, but often hurts readability.
- Prefer distinct names unless the shadowing is tiny and obvious.

```cpp
int value{1};

void demo()
{
    int value{2};          // hides global value
    std::cout << value;    // 2
    std::cout << ::value;  // 1
}
```

## 7.9 Namespace scope and the scope resolution operator (::)
- Names at namespace scope are accessed with the **scope resolution operator**.
- `std::cout` means `cout` inside namespace `std`.
- `::value` means `value` in the **global namespace**.
- Use qualification to disambiguate hidden names and avoid collisions.
- Namespaces help organize code and reduce accidental name clashes.

```cpp
int value{7};

namespace math
{
    int value{42};
}

std::cout << ::value << '\n';      // global value
std::cout << math::value << '\n';  // namespace value
```

## 7.10 Unnamed namespaces and the inline keyword for variables
- An **unnamed namespace** gives internal linkage to its members.
- It is useful for file-local helpers, variables, functions, and types.
- `inline` variables (C++17) allow a variable definition in a header without violating the one-definition rule.
- This is especially useful for shared constants.
- `inline` variables are safer than putting normal global definitions in headers.

```cpp
namespace
{
    int cacheHits{0};   // visible only in this translation unit
}

// constants.h
inline constexpr int maxUsers{64}; // okay in a header
```

## 7.11 Using declarations and using directives
- A **using declaration** imports one specific name:
  - `using std::cout;`
- A **using directive** imports all names from a namespace:
  - `using namespace std;`
- Prefer **using declarations** in small scopes.
- Avoid `using namespace ...` in headers and usually avoid it at global scope in source files too.
- Directives can cause name collisions and make code harder to reason about.

```cpp
using std::cout;   // one name
using std::endl;

void print()
{
    cout << "hello" << endl;
}

// Avoid in headers:
// using namespace std;
```

## 7.12 Scope, duration, and linkage summary
- **Local variable**: block scope, automatic duration, no linkage.
- **Static local**: block scope, static duration, no linkage.
- **Global/non-const namespace variable**: namespace scope, static duration, usually external linkage.
- **Namespace-scope const**: namespace scope, static duration, usually internal linkage.
- **`static` namespace-scope variable/function**: internal linkage.
- **`extern` declaration**: refers to a definition elsewhere.
- Best practice refresher:
  - keep scope narrow
  - prefer locals over globals
  - prefer `constexpr`/`const` when possible
  - share state explicitly instead of hiding it globally
  - use internal linkage for implementation details
  - use `inline constexpr` for header constants when appropriate

```cpp
// Good defaults:
constexpr int maxRetries{3};   // immutable local/config value

namespace
{
    void helper() {}           // file-local implementation detail
}

int main()
{
    static int runCount{0};    // persists, but only visible here
    ++runCount;
}
```
