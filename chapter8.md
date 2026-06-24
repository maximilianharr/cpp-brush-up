# Chapter 8: Control Flow

Current LearnCpp Chapter 8 sections found via web search. Note: there isn't a dedicated Chapter 8 `if constexpr` lesson in the current list, but it's included briefly below as a useful refresher.

## 8.1 Control flow introduction
- Control flow = the order statements execute.
- Default flow is sequential: top to bottom.
- Main tools:
  - selection: `if`, `switch`
  - iteration: `while`, `do-while`, `for`
  - jumps: `break`, `continue`, `goto`, `return`
- Boolean conditions decide which path runs.
- Prefer clear, direct conditions over clever ones.
- Keep branches small and readable.

```cpp
#include <iostream>

int main()
{
    int x{ 7 };

    if (x > 0)
        std::cout << "positive\n";

    std::cout << "done\n";
}
```

## 8.2 If statements and blocks
- `if` executes a statement only if the condition is `true`.
- A block is a group of statements inside `{}`.
- Use braces even for single-line branches when clarity matters.
- Conditions convert to `bool`.
- Variables declared in a block have block scope.
- `if constexpr` is compile-time branching:
  - condition must be a constant expression
  - discarded branch is not instantiated in templates
  - usually relevant in generic code, not runtime decision-making

```cpp
if (score >= 90)
{
    std::cout << "A\n";
}

if constexpr (sizeof(int) == 4)
{
    // compile-time selected branch
}
```

## 8.3 Else and else if
- `else` runs when the `if` condition is false.
- `else if` chains multiple conditions.
- Conditions are checked top to bottom; first match wins.
- Order matters: put more specific cases first.
- Long `else if` chains can sometimes be replaced by `switch`.

```cpp
if (temp < 0)
    std::cout << "freezing\n";
else if (temp < 20)
    std::cout << "cool\n";
else
    std::cout << "warm\n";
```

## 8.4 Dangling else problem
- `else` binds to the nearest unmatched `if`.
- Missing braces can make logic misleading.
- Common `if` mistakes:
  - forgetting braces in nested conditionals
  - `=` instead of `==`
  - comparing floating-point values directly when tolerance is needed
  - writing overly complex conditions
- Best fix: use braces consistently.

```cpp
if (x > 0)
{
    if (x < 10)
        std::cout << "single digit positive\n";
    else
        std::cout << "10 or more\n";
}
```

## 8.5 Switch statements
- `switch` selects among integral-like values:
  - integers
  - `char`
  - enums / enum classes
- Good for many equality-based branches on one expression.
- `case` labels must be constant expressions.
- `default` handles unmatched values.
- `switch` does not support ranges directly.

```cpp
char cmd{ 'q' };

switch (cmd)
{
case 'a':
    std::cout << "add\n";
    break;
case 'd':
    std::cout << "delete\n";
    break;
default:
    std::cout << "unknown\n";
    break;
}
```

## 8.6 Switch fallthrough and scoping
- Without `break`, execution falls through into the next case.
- Use intentional fallthrough sparingly and clearly.
- In modern C++, `[[fallthrough]];` documents intent.
- Case labels do not create their own scope.
- If you need local variables in a case, wrap that case in braces.

```cpp
switch (level)
{
case 3:
    [[fallthrough]];
case 2:
    std::cout << "medium+\n";
    break;
case 1:
{
    int bonus{ 5 };
    std::cout << bonus << '\n';
    break;
}
default:
    break;
}
```

## 8.7 Goto statements
- `goto` jumps to a labeled statement in the same function.
- Usually avoid it:
  - hurts readability
  - makes flow harder to reason about
  - can bypass initialization in confusing ways
- Rare exceptions exist in low-level/error-cleanup code, but structured control flow is usually better.

```cpp
if (error)
    goto cleanup;

std::cout << "work\n";

cleanup:
std::cout << "cleanup\n";
```

## 8.8 Halts: std::exit() and std::abort()
- Halts end the program immediately rather than returning normally.
- `std::exit(code)`:
  - ends program with a status code
  - runs static object cleanup and `atexit` handlers
  - does not unwind local automatic variables in the current stack
- `std::abort()`:
  - abnormal termination
  - typically no cleanup guarantees
- Prefer returning from `main()` when practical.

```cpp
#include <cstdlib>

if (!configLoaded)
    std::exit(1);

if (fatalCorruption)
    std::abort();
```

## 8.9 Loops (overview)
- Loops repeat work while a condition holds.
- Choose based on intent:
  - `while`: unknown iteration count, pre-check
  - `do-while`: must run at least once
  - `for`: counting / compact loop control
- Watch loop invariants:
  - initialization
  - condition
  - update
- Infinite loops need an internal exit path.

```cpp
while (running)
{
    process();
}
```

## 8.10 While loops
- Syntax: `while (condition) statement;`
- Condition is checked before each iteration.
- May run zero times.
- Good for input loops, sentinels, and state-driven repetition.
- Avoid forgetting the update step.

```cpp
int count{ 3 };

while (count > 0)
{
    std::cout << count << '\n';
    --count;
}
```

## 8.11 Do-while loops
- `do-while` checks the condition after the body.
- Always runs at least once.
- Useful for menus and retry loops.
- Less common than `while`/`for`; use when that "run once first" behavior is intentional.

```cpp
int value{};

do
{
    std::cout << "Enter 1-5: ";
    std::cin >> value;
}
while (value < 1 || value > 5);
```

## 8.12 For loops and off-by-one errors
- Compact structure: init; condition; update.
- Best for counted loops.
- Off-by-one bugs are common:
  - `i < n` vs `i <= n`
  - starting at wrong index
  - updating in the wrong direction
- Loop indices are often `std::size_t` for container sizes.
- Range-based `for` is ideal when you just want each element.

```cpp
for (int i{ 0 }; i < 5; ++i)
    std::cout << i << ' ';

for (const auto& name : names)
    std::cout << name << '\n';
```

## 8.13 Break and continue
- `break` exits the nearest enclosing `switch` or loop.
- `continue` skips to the next iteration of the nearest loop.
- Useful, but too much use can make loops harder to follow.
- Prefer straightforward loop conditions when possible.

```cpp
for (int i{ 0 }; i < 10; ++i)
{
    if (i == 5)
        break;

    if (i % 2 == 0)
        continue;

    std::cout << i << ' ';
}
```

## 8.14 Random number generation
- Prefer `<random>` over `std::rand()`.
- Typical setup:
  - engine: generates pseudo-random numbers
  - distribution: maps numbers into a useful range
- Seed once, usually with `std::random_device`.
- Reuse the engine; don't reseed every draw.

```cpp
#include <iostream>
#include <random>

int main()
{
    std::random_device rd;
    std::mt19937 gen{ rd() };
    std::uniform_int_distribution<int> dist{ 1, 6 };

    std::cout << dist(gen) << '\n'; // like rolling a die
}
```
