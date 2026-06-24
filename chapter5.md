# Chapter 5: Constants and Strings

## 5.1 Constant variables (named constants)
- A **constant** is a value that cannot change after initialization.
- Prefer **named constants** over repeating raw values.
- Use `const` for values that should not be modified after creation.
- `const` variables **must be initialized**.
- A `const` object is not automatically a compile-time constant.

```cpp
const double gravity { 9.8 };
const int maxStudents { 30 };

int age {};
std::cin >> age;
const int userAge { age }; // const, but runtime value
```

- Good uses:
  - fixed configuration values
  - function-local read-only values
  - global/program constants (often `constexpr` is even better)
- Prefer constants over macros:

```cpp
constexpr double pi { 3.14159265358979323846 };
// Avoid: #define PI 3.14159
```

## 5.2 Literals
- A **literal** is a fixed value written directly in code.
- Examples: `42`, `3.14`, `'a'`, `true`, `"hello"`.
- Literals have types too:
  - `42` -> `int`
  - `42u` -> `unsigned int`
  - `42LL` -> `long long`
  - `3.0` -> `double`
  - `3.0f` -> `float`
- String literals are not `std::string`; they are C-style character arrays.

```cpp
int x { 42 };
double d { 3.14 };
char ch { 'A' };
bool ok { true };
const char* msg { "hello" };
```

- Use suffixes when you need an exact type.
- Prefer readable literals; don't force the reader to infer intent.

```cpp
auto big { 1'000'000 };   // digit separators improve readability
auto mask { 0xFFu };
auto bits { 0b1010'0101 };
```

## 5.3 Numeral systems (decimal, binary, hexadecimal, and octal)
- C++ supports multiple integer literal bases:
  - decimal: `42`
  - binary: `0b101010`
  - hexadecimal: `0x2A`
  - octal: `052`
- Binary is useful for bit flags.
- Hex is compact for bit patterns, colors, masks, addresses.
- Octal exists, but is uncommon and easy to misread.

```cpp
int dec { 42 };
int bin { 0b0010'1010 };
int hex { 0x2A };
int oct { 052 };
```

- Same value, different representations:

```cpp
std::cout << 26 << '\n';      // decimal
std::cout << 0b11010 << '\n'; // binary literal
std::cout << 0x1A << '\n';    // hexadecimal literal
```

- Watch for legacy octal form:

```cpp
int a { 10 };  // ten
int b { 010 }; // eight
```

## 5.4 The as-if rule and compile-time optimization
- The compiler may transform code **however it wants** as long as the program's
  **observable behavior** stays the same.
- This is the **as-if rule**.
- One major result: compile-time optimization.
- Constant folding replaces constant computations with final values.

```cpp
int x { 3 + 4 }; // compiler can turn this into int x{7};
```

- Compile-time-known values help the compiler:
  - fold arithmetic
  - remove dead branches
  - precompute sizes
- Do **not** depend on how an expression is optimized.
- Only depend on guaranteed language behavior.

```cpp
constexpr int width { 8 };
constexpr int height { 6 };
constexpr int area { width * height }; // computed at compile time
```

- Optimization is a compiler choice; correctness must not rely on it.

## 5.5 Constant expressions
- A **constant expression** is an expression that can be evaluated at compile time.
- Not all constants are compile-time constants.
- Key distinction:
  - `const` -> read-only after initialization
  - constant expression -> usable where compile-time evaluation is required

```cpp
constexpr int a { 5 };   // compile-time constant
const int b { a + 1 };   // also compile-time constant here

int x {};
std::cin >> x;
const int y { x };       // runtime constant, not a constant expression
```

- Compile-time constants are required in places such as:
  - array bounds (traditional arrays)
  - `case` labels
  - non-type template parameters
  - some `static_assert` conditions

```cpp
constexpr int size { 10 };
int arr[size]{};
static_assert(size > 0);
```

- `constexpr` functions can participate in constant expressions.
- Modern note:
  - `constexpr` function -> **may** run at compile time
  - `consteval` function -> **must** run at compile time

```cpp
constexpr int square(int x) { return x * x; }
consteval int cube(int x) { return x * x * x; }

constexpr int s { square(4) }; // compile time
constexpr int c { cube(3) };   // compile time only
```

## 5.6 Constexpr variables
- `constexpr` declares a variable that must be initialized with a constant expression.
- If initialization is not compile-time evaluable, compilation fails.
- Use `constexpr` when a value is conceptually constant **and** should be compile-time known.
- Prefer `constexpr` over `const` when possible for true constants.

```cpp
constexpr int daysPerWeek { 7 };
constexpr double gravity { 9.8 };
```

- `const` vs `constexpr`:

```cpp
int input {};
std::cin >> input;

const int runtimeConst { input }; // okay
// constexpr int bad { input };   // error: not a constant expression
```

- `constexpr` functions can be used with `constexpr` variables:

```cpp
constexpr int max(int a, int b)
{
    return (a > b) ? a : b;
}

constexpr int bigger { max(5, 8) };
```

- Remember:
  - `constexpr` value -> compile-time constant
  - `const` value -> maybe compile-time, maybe runtime

## 5.7 Introduction to std::string
- `std::string` is the standard owning string type.
- Include `<string>`.
- It manages memory for you.
- It is easier and safer than raw C-style strings for most application code.

```cpp
#include <string>

std::string name { "Alex" };
std::string empty {};
```

- Common operations:

```cpp
std::string first { "Hello" };
std::string second { "world" };

std::string combined { first + " " + second };
combined += "!";
```

- Input:

```cpp
std::string line {};
std::getline(std::cin, line);
```

- Useful members:
  - `.length()` / `.size()`
  - indexing with `[]`
  - concatenation with `+` / `+=`

```cpp
std::cout << name.size() << '\n';
std::cout << name[0] << '\n';
```

## 5.8 Introduction to std::string_view
- `std::string_view` is a **non-owning read-only view** of a string.
- Include `<string_view>`.
- It does not allocate or copy string data.
- Great for function parameters that only need to read text.

```cpp
#include <string_view>

void print(std::string_view text)
{
    std::cout << text << '\n';
}

print("hello");
print(std::string{"world"});
```

- Benefits:
  - cheap to copy
  - works with string literals, `std::string`, and character arrays
  - avoids unnecessary temporary strings
- Important: it does **not own** the characters it points at.

```cpp
std::string_view sv { "hello" }; // view of string literal
```

- Reach for `std::string_view` when:
  - you only need to inspect text
  - you do not need to store/modify/own it

## 5.9 std::string_view (part 2)
- `std::string_view` is fast, but lifetime matters.
- A view becomes dangling if the referenced string dies or is modified in an invalidating way.

```cpp
std::string_view good { "hello" }; // okay: literal lives for program lifetime

std::string makeName();
std::string_view bad { makeName() }; // dangling: temporary string destroyed
```

- Useful operations:

```cpp
std::string_view text { "peach" };
std::cout << text.substr(1, 3) << '\n'; // "eac"

text.remove_prefix(1); // now "each"
text.remove_suffix(2); // now "ea"
```

- Use `std::string` when:
  - you need ownership
  - you need mutation
  - you need to return/store text long-term
- Use `std::string_view` when:
  - you want cheap read-only access
  - the original data outlives the view

```cpp
std::string owner { "banana" };
std::string_view view { owner };
```

## 5.x Chapter 5 summary and quiz
- Summary only below; quiz content intentionally omitted.
- Use **`const`** for values that should not change after initialization.
- Use **`constexpr`** for values that should be compile-time constants.
- Use **`consteval`** for functions that must always produce compile-time results.
- Literals are raw values written directly in code.
- Integer literals may be decimal, binary, hexadecimal, or octal.
- Constant expressions matter because some language features require compile-time values.
- `std::string` is an owning, mutable string type.
- `std::string_view` is a non-owning, read-only view; cheap and useful, but lifetime-sensitive.

```cpp
int input {};
std::cin >> input;
const int runtimeOnly { input };       // read-only, runtime
constexpr int compileTime { 42 };      // compile-time constant

constexpr int twice(int x) { return x * 2; }
consteval int triple(int x) { return x * 3; }

std::string s { "hello" };   // owns characters
std::string_view sv { s };   // borrows characters
```

- Rule of thumb:
  - prefer named constants over magic values
  - prefer `constexpr` when possible
  - prefer `std::string_view` for read-only parameters
  - prefer `std::string` when ownership or mutation is needed
