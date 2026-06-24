# Chapter 4: Fundamental Data Types

## 4.1 Introduction to fundamental data types

- Fundamental types are built into the language: `bool`, `char`, integer types, floating-point types, and `void`.
- They describe what kind of value can be stored, how much memory is used, and what operations make sense.
- Exact sizes are partly implementation-defined, so write code around *guarantees*, not assumptions.
- Good default choices:
  - `int` for ordinary whole numbers
  - `double` for ordinary real numbers
  - `bool` for true/false
  - `char` for a single ASCII-like character
- Use the narrowest *meaningful* type, not just the smallest type.
- Prefer clarity first; optimize size only when it actually matters.

```cpp
int age{ 27 };
double price{ 19.99 };
bool isReady{ true };
char grade{ 'A' };
```

## 4.2 Void

- `void` means “no type”.
- Common use: a function that returns no value.
- You cannot create a normal variable of type `void`.
- A `return;` in a `void` function ends the function without producing a value.
- `void` is about *absence*, not storage.

```cpp
void printDone()
{
    std::cout << "done\n";
}

int main()
{
    printDone();
    return 0;
}
```

## 4.3 Object sizes and the sizeof operator

- `sizeof` returns the size of a type or object in bytes.
- Result type: `std::size_t`.
- Use it when you need portable size information.
- `sizeof(type)` and `sizeof variable` are both valid.
- Sizes can differ by compiler/platform.
- A byte is the smallest addressable unit of memory.

Typical sizes on modern systems (not guaranteed):

| Type | Common size |
| --- | --- |
| `bool` | 1 byte |
| `char` | 1 byte |
| `short` | 2 bytes |
| `int` | 4 bytes |
| `long` | 4 or 8 bytes |
| `long long` | 8 bytes |
| `float` | 4 bytes |
| `double` | 8 bytes |

```cpp
std::cout << "int: " << sizeof(int) << " bytes\n";
std::cout << "double: " << sizeof(double) << " bytes\n";

int x{};
std::cout << "x: " << sizeof(x) << " bytes\n";
```

## 4.4 Signed integers

- Signed integers can hold negative and positive whole numbers.
- Main signed integer types:
  - `short`
  - `int`
  - `long`
  - `long long`
- Minimum standard widths:

| Type | Minimum width | Common width (Win64/Linux64) |
| --- | --- | --- |
| `short` | 16 bits | 16 bits |
| `int` | 16 bits | 32 bits |
| `long` | 32 bits | 32/64 bits |
| `long long` | 64 bits | 64 bits |

- Range grows with width.
- `int` is usually the best default integer type.
- Use larger types only when range requires it.

```cpp
int apples{ 42 };
int debt{ -7 };
long long worldPopulation{ 8100000000LL };
```

## 4.5 Unsigned integers, and why to avoid them

- Unsigned integers store only `0` and positive whole numbers.
- They trade negative range for a larger positive range.
- Problem: arithmetic wraps around modulo `2^n` instead of going negative.
- Problem: mixing signed and unsigned values can produce surprising comparisons.
- Prefer signed integers for normal math.
- Good uses for unsigned:
  - bit flags / bit manipulation
  - values that are truly treated as raw binary
  - some APIs that specifically require unsigned types

```cpp
unsigned int u{ 0 };
std::cout << u - 1 << '\n'; // wraps: large value, not -1

int s{ -1 };
if (s < 1u) // surprising after conversions
{
    std::cout << "be careful\n";
}
```

## 4.6 Fixed-width integers and size_t

- Header: `#include <cstdint>`.
- Fixed-width integers give exact sizes when available:
  - `std::int8_t`, `std::int16_t`, `std::int32_t`, `std::int64_t`
  - unsigned counterparts: `std::uint32_t`, etc.
- Use fixed-width integers when width matters:
  - file formats
  - binary protocols
  - serialization
  - cross-platform data layout
- `std::size_t` is the standard type for object sizes and container sizes.
- `sizeof` returns `std::size_t`.
- `std::size_t` is unsigned, so be careful when counting down.

| Type | Meaning |
| --- | --- |
| `std::int32_t` | exactly 32-bit signed |
| `std::uint64_t` | exactly 64-bit unsigned |
| `std::size_t` | size/index/count type |

```cpp
#include <cstdint>

std::int32_t id{ 123456 };
std::uint64_t mask{ 0xff00ff00ULL };
std::size_t count{ sizeof(id) };
```

## 4.7 Introduction to scientific notation

- Scientific notation is a compact way to write very large or very small numbers.
- Form: significand × 10^exponent.
- In code, use `e` or `E`:
  - `6.02e23`
  - `1.5e-4`
- Helpful for reading floating-point literals without long runs of zeros.

```cpp
double avogadro{ 6.022e23 };
double small{ 1.0e-8 };
double alsoSmall{ 3.5E-5 };
```

## 4.8 Floating point numbers

- Floating-point types store real numbers with fractional parts.
- Main types:
  - `float`
  - `double`
  - `long double`
- `double` is the usual default.
- Floating-point values are approximate, not exact.
- Many decimal values cannot be represented exactly in binary.
- Never assume `0.1 + 0.2 == 0.3`.
- Compare carefully when precision matters.

Approximate precision:

| Type | Common size | Approx. significant digits |
| --- | --- | --- |
| `float` | 4 bytes | ~6-7 |
| `double` | 8 bytes | ~15-16 |
| `long double` | varies | at least `double`, often more |

```cpp
double x{ 0.1 };
double y{ 0.2 };

std::cout << std::setprecision(17);
std::cout << (x + y) << '\n'; // often 0.30000000000000004-ish
```

## 4.9 Boolean values

- `bool` stores `true` or `false`.
- Used heavily in conditions and decision-making.
- By default, streaming a `bool` prints `1` or `0`.
- Use `std::boolalpha` to print `true` / `false`.
- Booleans often come from comparisons.

```cpp
bool isValid{ true };
bool isEqual{ 3 == 3 };

std::cout << isValid << '\n';              // 1
std::cout << std::boolalpha << isEqual;    // true
```

## 4.10 Introduction to if statements

- `if` executes code only when its condition is true.
- Conditions evaluate to `bool`.
- `else` handles the false case.
- Use braces even for one statement; it prevents mistakes.
- Prefer direct boolean conditions over clever expressions.

```cpp
int x{ 5 };

if (x > 0)
{
    std::cout << "positive\n";
}
else
{
    std::cout << "zero or negative\n";
}
```

## 4.11 Chars

- `char` stores a single character.
- Character literals use single quotes: `'a'`, `'Z'`, `'0'`, `'\n'`.
- `char` is an integral type under the hood.
- Printing a `char` shows the character; casting to `int` shows its code.
- ASCII codes are commonly used for basic English letters/digits.
- Plain `char` may be signed or unsigned depending on the implementation.

```cpp
char c{ 'A' };

std::cout << c << '\n';                    // A
std::cout << static_cast<int>(c) << '\n';  // 65

char next{ static_cast<char>(c + 1) };
std::cout << next << '\n';                 // B
```

## 4.12 Introduction to type conversion and static_cast

- Type conversion changes a value from one type to another.
- Implicit conversions happen automatically.
- Explicit conversions are written by the programmer.
- `static_cast<T>(value)` is the preferred explicit cast for basic type conversions.
- Watch for narrowing conversions:
  - `double` to `int` drops the fractional part
  - larger integer to smaller integer may lose data
- Use casts intentionally, not casually.

```cpp
double pi{ 3.14159 };
int whole{ static_cast<int>(pi) }; // 3

int a{ 5 };
int b{ 2 };
double result{ static_cast<double>(a) / b }; // 2.5
```

## 4.x Chapter summary

- Fundamental types are the base vocabulary of C++ values.
- `void` means “no type”.
- `sizeof` reports object/type size in bytes and returns `std::size_t`.
- Prefer `int` for ordinary integer work.
- Avoid unsigned integers for general arithmetic and comparisons.
- Use `<cstdint>` fixed-width integers when exact bit width matters.
- Use `std::size_t` for sizes/counts returned by `sizeof` and container size APIs.
- Scientific notation makes floating-point literals readable.
- Prefer `double` to `float` unless you have a reason not to.
- Floating-point math is approximate; equality checks need care.
- `bool` represents true/false and drives control flow.
- `if` statements branch based on conditions.
- `char` stores character codes; it is still an integer type.
- Use `static_cast` for clear, explicit type conversions.

Quick defaults:

```cpp
int count{};          // whole numbers
double price{};       // fractional values
bool isOpen{};        // true/false
char letter{};        // one character
std::int32_t id{};    // exact-width integer when needed
std::size_t size{};   // sizes/counts
```
