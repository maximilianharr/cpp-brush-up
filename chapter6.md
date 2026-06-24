# Chapter 6: Operators

## 6.1 Operator precedence and associativity

- **Precedence** decides which operator binds first.
- **Associativity** breaks ties when precedence is the same.
- Use **parentheses** to make intent obvious; don't rely on memorizing everything.
- Precedence changes grouping, not evaluation of every subexpression detail.

### Quick reference

| Level | Operators | Associativity |
|---|---|---|
| Highest common | `()` `[]` `.` `->` postfix `++ --` | left-to-right |
| Unary | unary `+ -` `!` prefix `++ --` | right-to-left |
| Multiplicative | `* / %` | left-to-right |
| Additive | `+ -` | left-to-right |
| Relational | `< <= > >=` | left-to-right |
| Equality | `== !=` | left-to-right |
| Logical AND | `&&` | left-to-right |
| Logical OR | `||` | left-to-right |
| Assignment | `= += -= *= /= %=` | right-to-left |
| Lowest common | `,` | left-to-right |

```cpp
int x{ 2 + 3 * 4 };      // 14, not 20
int y{ (2 + 3) * 4 };    // 20

bool ok{ 3 < 4 && 8 > 2 };   // true
int z{ 1 + 2 + 3 };          // (1 + 2) + 3
int a{}, b{}, c{};
a = b = c = 0;               // a = (b = (c = 0))
```

- Prefer explicit grouping when an expression is even slightly non-obvious.

## 6.2 Arithmetic operators

- Unary: `+x`, `-x`
- Binary: `+ - * / %`
- Integer division **truncates toward zero**.
- Division by zero with integers is **undefined behavior**.
- Mixed arithmetic may promote integers to floating point.

```cpp
int a{ 7 / 4 };          // 1
int b{ -7 / 4 };         // -1
double c{ 7 / 4.0 };     // 1.75
double d{ 2.5 * 4 };     // 10.0
```

- `int / int` stays integral even if assigned later to `double`.

```cpp
double x{ 7 / 4 };       // 1.0, division already happened as int
double y{ 7.0 / 4 };     // 1.75
```

- Watch for overflow:
  - signed overflow: avoid; behavior is not safe to rely on
  - unsigned arithmetic wraps modulo `2^N`

## 6.3 Remainder and exponentiation

- Remainder uses `%` and works with **integral** operands.
- Result sign follows the **left operand** (dividend).
- Common uses: odd/even checks, cyclic indexing, unit conversions.

```cpp
int r1{ 7 % 4 };         // 3
int r2{ -7 % 4 };        // -3
bool isOdd{ (13 % 2) != 0 };
```

- C++ has **no exponentiation operator**.
- `^` is **bitwise XOR**, not power.
- Use `std::pow()` from `<cmath>` for powers.

```cpp
#include <cmath>

double a{ std::pow(3.0, 4.0) };   // 81.0
int b{ 3 ^ 4 };                   // 7, XOR -- not 81
```

- `std::pow()` returns floating-point and may not be exact for integer math.
- For exact integer powers, prefer integer-specific logic when practical.

## 6.4 Increment/decrement operators, and side effects

- `++x` / `--x`: modify first, then yield the updated value.
- `x++` / `x--`: yield the old value, then modify.
- Prefer **prefix** unless you specifically need the old value.

```cpp
int x{ 5 };
int a{ ++x };   // x = 6, a = 6

int y{ 5 };
int b{ y++ };   // b = 5, y = 6
```

- A **side effect** changes program state (e.g. modifying a variable).
- Avoid expressions with multiple side effects on the same variable.

```cpp
int i{ 0 };
++i;            // clear
i++;            // clear

// Avoid:
// int n{ i++ + ++i };   // confusing / potentially problematic
```

- Keep side effects isolated in separate statements when readability matters.

```cpp
int i{ 0 };
int n{ i };
++i;
```

## 6.5 The comma operator

- The comma operator evaluates the left operand, discards it, then evaluates and returns the right operand.
- It is **different** from commas used as separators in declarations, argument lists, or initializer lists.
- Rare in modern C++; usually separate statements are clearer.

```cpp
int x{ (1 + 2, 4 + 5) };     // x = 9
```

- A legitimate use is in `for` loop update expressions.

```cpp
for (int i{ 0 }, j{ 9 }; i < j; ++i, --j)
{
    // ...
}
```

- Outside narrow cases like `for`, prefer clarity over cleverness.

## 6.6 Relational operators and floating point comparisons

- Relational operators: `< <= > >=`
- Equality operators: `== !=`
- They produce `bool`.

```cpp
bool a{ 3 < 5 };          // true
bool b{ 10 == 7 };        // false
bool c{ 8 != 8 };         // false
```

- Don't chain comparisons like math:

```cpp
int x{ 5 };
bool wrong{ 0 < x < 10 };         // true, but not for the reason you want
bool right{ 0 < x && x < 10 };    // correct
```

- Floating-point values often cannot be represented exactly.
- Direct `==` is often unsafe for computed results.

```cpp
double a{ 0.1 + 0.1 + 0.1 };
bool equal{ a == 0.3 };   // may be false
```

- Compare using an epsilon / tolerance instead.

```cpp
#include <algorithm>
#include <cmath>

bool approximatelyEqual(double a, double b, double eps = 1e-12)
{
    return std::abs(a - b)
        <= eps * std::max(1.0, std::max(std::abs(a), std::abs(b)));
}
```

- Use approximate comparison for computed floats; direct `==` is okay mainly for exact sentinel-style values.

## 6.7 Logical operators

- Logical NOT: `!`
- Logical AND: `&&`
- Logical OR: `||`
- Operands are contextually converted to `bool`.
- Precedence: `!` before `&&`, `&&` before `||`

```cpp
bool a{ true };
bool b{ false };

bool x{ !a };         // false
bool y{ a && b };     // false
bool z{ a || b };     // true
```

- `&&` and `||` **short-circuit**:
  - `lhs && rhs`: if `lhs` is false, `rhs` is skipped
  - `lhs || rhs`: if `lhs` is true, `rhs` is skipped

```cpp
if (ptr && ptr->isValid())
{
    // safe: second operand runs only if ptr is non-null
}
```

- This is useful for guards and efficient condition checks.
- Don't confuse logical `&& ||` with bitwise `& |`.

## 6.x Chapter summary

- Precedence and associativity explain grouping; parentheses improve clarity.
- Arithmetic operators are straightforward, but integer division truncates.
- `%` is remainder, not percent; `^` is XOR, not exponentiation.
- C++ has no power operator; use `std::pow()` when appropriate.
- Prefer prefix `++/--` unless postfix behavior is needed.
- Avoid expressions with too many side effects.
- The comma operator exists, but is rarely the clearest choice.
- Chained comparisons need `&&`, not mathematical shorthand.
- Floating-point equality usually needs tolerance-based comparison.
- Logical operators are central to control flow and short-circuiting.

```cpp
int n{ 7 / 4 };                          // 1
bool inRange{ 0 < n && n < 10 };        // true
bool ok{ ptr && ptr->ready() };         // short-circuit guard
```

_Quiz content intentionally omitted._
