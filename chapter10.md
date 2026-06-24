# Chapter 10: Type Conversion, Type Aliases, and Type Deduction

Concise refresher for the current LearnCpp Chapter 10 lesson pages (10.1–10.8). Quiz pages omitted.

## 10.1 Implicit type conversion

- An **implicit conversion** happens automatically when C++ needs a value of a different type.
- Common places: initialization, assignment, function arguments, return values, and expressions.
- Safe conversions preserve meaning; unsafe ones can lose data or change interpretation.
- Converting to `bool` uses the usual rule: `0` is `false`, anything else is `true`.
- Prefer matching types directly when practical; fewer silent conversions means fewer surprises.

```cpp
int i{42};
double d{i};   // int -> double

double pi{3.14};
int whole = pi; // double -> int, fractional part discarded

if (whole)      // int -> bool
{
    // runs because whole is non-zero
}
```

**Rule of thumb:** implicit conversions are convenient, but they are safest when they widen rather than narrow.

## 10.2 Standard conversions

- **Standard conversions** are the built-in conversions the language knows how to perform.
- Important categories here:
  - value transformations
  - promotions
  - numeric conversions
  - qualification / pointer-related conversions
- **Promotions** are special standard conversions that prefer a larger “natural” type.
- Typical promotions:
  - `bool`, `char`, `signed char`, `unsigned char`, `short` -> `int` (or sometimes `unsigned int`)
  - `float` -> `double`
- Promotions are favored in overload resolution because they are considered safer than general conversions.

```cpp
char ch{'A'};
int code{ch};     // char promoted to int

short s{7};
int sum{s + 1};   // short promoted to int before arithmetic

float f{3.5f};
double g{f};      // float promoted to double
```

**Why this matters:** many “small” types do not stay small once expressions are evaluated.

## 10.3 Numeric promotion and conversion

- Arithmetic types can be changed to other arithmetic types automatically or explicitly.
- **Promotion** usually preserves the value exactly.
- **Conversion** may lose:
  - fractional parts
  - range
  - precision
  - signedness meaning
- Converting integral -> floating point is often okay for small values, but large integers may not be represented exactly.
- Converting floating point -> integral truncates toward zero.

```cpp
int count{100};
double exactEnough{count}; // int -> double

double price{9.99};
int dollars{static_cast<int>(price)}; // 9, fractional part lost

unsigned int u{42};
int signedValue{static_cast<int>(u)}; // okay only if value fits
```

**Mental model:** promotions are usually harmless; general numeric conversions need scrutiny.

## 10.4 Usual arithmetic conversions

- When arithmetic operands have different types, C++ converts them to a **common type** first.
- This process is called the **usual arithmetic conversions**.
- Broad idea:
  1. floating-point types win over integral types
  2. smaller integral types are promoted
  3. signed/unsigned rank rules decide the final integral type
- The result type of an expression may therefore differ from either original operand type.
- Mixed signed/unsigned expressions are a common source of bugs.

```cpp
double d{4.5};
int i{2};
auto r1{d + i};   // int converted to double, result is double

short a{1};
short b{2};
auto r2{a + b};   // both promoted to int, result is int

int x{-1};
unsigned int y{1};
bool less = (x < y); // x converts to unsigned, often surprising
```

**Takeaway:** if mixed-type arithmetic matters, make the target type explicit instead of relying on implicit balancing.

## 10.5 Narrowing conversions

- A **narrowing conversion** is a conversion that may lose information.
- Common narrowing cases:
  - `double` -> `int`
  - large integer -> smaller integer type
  - signed -> unsigned when negatives are possible
  - large floating point -> smaller floating point type
- **List initialization** (`{}`) helps protect you: it rejects narrowing at compile time.
- Exception: a `constexpr` initializer is allowed when the compiler can prove the value fits exactly.

```cpp
int a = 3.7;     // allowed, becomes 3
// int b{3.7};   // error: narrowing blocked by list initialization

constexpr int n{65};
char c{n};       // okay if 65 fits in char on this platform

int big{1000};
// std::int8_t small{big}; // error: not a constant expression proving fit
```

**Best practice:**
- prefer brace initialization
- check ranges before shrinking a type
- use an explicit cast only when loss is intentional and understood

## 10.6 Explicit type conversion (casting) and static_cast

- Use an **explicit cast** when you want a conversion to be visible and intentional.
- For ordinary well-defined conversions, prefer **`static_cast`**.
- `static_cast` is useful for:
  - arithmetic conversions
  - enum/integer conversions
  - making a conversion obvious to readers
- It does **not** make a dangerous conversion safe; it only makes it explicit.
- Avoid C-style casts when possible; `static_cast` is easier to search for and review.

```cpp
int total{5};
int count{2};
double average{static_cast<double>(total) / count}; // 2.5

char ch{'A'};
int ascii{static_cast<int>(ch)};

double pi{3.14159};
int whole{static_cast<int>(pi)}; // explicit truncation
```

**Use `static_cast` to say “yes, I really mean this conversion”.**

## 10.7 Typedefs and type aliases

- A **type alias** gives an existing type another name.
- Old syntax: `typedef`
- Modern syntax: `using`
- `using` is usually clearer, especially with templates.
- Aliases improve readability for:
  - long template types
  - portability types
  - domain-specific names
- An alias does **not** create a new distinct type; it is only another name.

```cpp
typedef unsigned long ulong;   // old style
using index_t = std::size_t;   // preferred

using Scores = std::vector<int>;
Scores scores{90, 80, 100};

template <typename T>
using Vec = std::vector<T>;

Vec<double> values{1.0, 2.0, 3.0};
```

**Good use:** make complex or semantic types easier to read.  
**Bad use:** hiding simple built-in types for no real benefit.

## 10.8 Type deduction (auto)

- `auto` tells the compiler to deduce the type from the initializer.
- An initializer is required because deduction needs a source type.
- `auto` is great when:
  - the type is obvious from the right-hand side
  - the exact type is verbose
  - you want the compiler to track a changed return type automatically
- Plain `auto` drops top-level `const` and references.
- Preserve them with `const auto`, `auto&`, or `const auto&`.

```cpp
auto i{42};              // int
auto d{3.14};            // double

const int x{5};
auto a{x};               // int
const auto b{x};         // const int
auto& ref{x};            // const int&

std::vector<int> nums{1, 2, 3};
for (const auto& n : nums)
{
    // read-only access without copying
}
```

- Brace-init caveat:
  - `auto x{5};` -> `int`
  - `auto y = {5};` -> `std::initializer_list<int>`
- For functions, `auto` can also deduce a return type:

```cpp
auto square(int v)
{
    return v * v; // deduced as int
}

auto ratio(int a, int b)
{
    return static_cast<double>(a) / b; // deduced as double
}
```

**Practical rule:** use `auto` when it improves clarity, not when it hides important type information.
