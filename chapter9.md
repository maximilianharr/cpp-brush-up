# Chapter 9: Error Detection and Handling

## 9.1 Introduction to testing your code

- **Testing** checks whether code behaves as intended for representative inputs.
- Good tests cover:
  - **Typical cases**: normal expected usage
  - **Boundary cases**: edges like `0`, `1`, empty input, max/min values
  - **Invalid cases**: bad input, impossible states, misuse
- A useful mindset: assume your code is wrong until tested.
- Re-test after changes to catch **regressions**.
- Prefer small functions: they are easier to reason about and test.

**Quick checklist**

- Did I test the “happy path”?
- Did I test both sides of each condition?
- Did I test edge values?
- Did I test failure behavior?

```cpp
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Good manual test set:
// max(2, 1)  -> 2
// max(1, 2)  -> 2
// max(5, 5)  -> 5
// max(-3, 7) -> 7
```

```cpp
bool isEven(int x)
{
    return x % 2 == 0;
}

// Test normal, boundary, and negative values:
// isEven(2)  -> true
// isEven(3)  -> false
// isEven(0)  -> true
// isEven(-4) -> true
```

**Takeaway:** testing is not optional polish; it is part of writing correct code.

## 9.2 Code coverage

- **Code coverage** measures how much of your code is exercised by tests.
- Common intuition: if code never runs during testing, bugs there can survive unnoticed.
- High coverage is useful, but **coverage is not correctness**.
- 100% statement coverage can still miss logic bugs.
- Coverage is best used to find **untested paths**, not as proof the code is bug-free.

**Coverage ideas**

- **Statement coverage**: every statement ran
- **Branch coverage**: each branch (`if/else`, loop path) ran
- **Path coverage**: combinations of branches ran

```cpp
int absValue(int x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

// Testing only absValue(5) misses the negative branch.
// Testing absValue(5), absValue(0), absValue(-5) is better.
```

```cpp
bool isSmallPositive(int x)
{
    return x > 0 && x < 10;
}

// Useful tests:
// -1, 0, 1, 9, 10
// This hits both halves of both comparisons.
```

**Practical rule:** use coverage to discover what you forgot to test, then add meaningful tests.

## 9.3 Common semantic errors in C++

- **Semantic errors** compile successfully but do the wrong thing.
- These are often more dangerous than syntax errors because the program still runs.

**Common examples**

- Using the wrong formula or operator
- Off-by-one mistakes in loops and indexing
- Integer division when floating-point was intended
- Wrong condition (`<` vs `<=`)
- Assignment vs comparison confusion
- Using uninitialized values
- Assuming input/state is valid when it is not

```cpp
int items{ 5 };
int groups{ 2 };

double average1{ items / groups };          // 2.0, not 2.5
double average2{ static_cast<double>(items) / groups }; // 2.5
```

```cpp
int sumTo(int n)
{
    int sum{};
    for (int i{ 1 }; i < n; ++i) // bug: excludes n
        sum += i;
    return sum;
}

// sumTo(3) returns 3, but should return 6
```

```cpp
if (x = 0) // bug: assignment, not comparison
{
    // ...
}
```

**How to catch semantic errors**

- Compile with warnings enabled
- Test normal + edge cases
- Use a debugger when behavior is surprising
- Add assertions for assumptions/invariants
- Break complex logic into smaller functions

**Takeaway:** if the program “works” but gives wrong results, suspect a semantic error first.

## 9.4 Detecting and handling errors

- An **error** is a condition that prevents correct work.
- Typical choices when an error is detected:
  1. **Handle it locally**
  2. **Report it to the caller**
  3. **Stop the program** if recovery is impossible or unsafe

**Common strategies**

- Return a status (`bool`, error code)
- Return a value plus success/failure information
- Validate inputs before using them
- Terminate on impossible states
- Use exceptions for failures that should be handled at a higher level

```cpp
bool divide(int a, int b, int& result)
{
    if (b == 0)
        return false;

    result = a / b;
    return true;
}
```

```cpp
int result{};
if (!divide(10, 0, result))
{
    std::cerr << "Error: divide by zero\n";
}
```

```cpp
double calculateSqrt(double x)
{
    if (x < 0.0)
        throw std::runtime_error{ "negative input" }; // brief exception example

    return std::sqrt(x);
}
```

- Use **assertions** for programmer mistakes / broken assumptions.
- Use **normal error handling** for expected runtime problems like bad files or bad user input.
- **Exceptions** are a structured way to signal failure when local handling is awkward; LearnCpp covers them in depth later (Chapter 27).

**Rule of thumb:** expected problems should be reported cleanly; impossible states should be treated as bugs.

## 9.5 std::cin and handling invalid input

Input with `std::cin` can fail in two main ways:

1. **Extraction failure**: input cannot be converted to the target type
2. **Extraneous input**: valid value is read, but junk remains in the buffer

```cpp
int x{};
std::cin >> x;
```

If the user enters `abc`, extraction fails:

- `std::cin.fail()` becomes `true`
- further input attempts fail until the stream is reset

**Core recovery tools**

- `std::cin.fail()` — check whether extraction failed
- `std::cin.clear()` — reset error flags
- `std::cin.ignore(...)` — discard unwanted buffered input

```cpp
#include <iostream>
#include <limits>

int getInt()
{
    while (true)
    {
        int x{};
        std::cin >> x;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again: ";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return x;
    }
}
```

If the user enters `5*7`, `5` may be extracted successfully and `*7` remains. Clearing the rest of the line avoids contaminating the next read.

**Best practice:** after formatted input, consume leftover input when you require a clean next read.

## 9.6 assert and static_assert

- `assert()` checks a condition at **runtime** in debug builds.
- `static_assert` checks a condition at **compile time**.

### `assert`

- Use for assumptions that should always be true if the program is correct.
- Good for internal invariants, preconditions, and impossible states.
- Not for handling user mistakes or normal runtime failures.
- Often disabled in release builds when `NDEBUG` is defined.

```cpp
#include <cassert>

double divide(double a, double b)
{
    assert(b != 0.0);
    return a / b;
}
```

```cpp
void printElement(const std::vector<int>& v, int index)
{
    assert(index >= 0 && index < static_cast<int>(v.size()));
    std::cout << v[index] << '\n';
}
```

### `static_assert`

- Evaluated by the compiler
- Great for type assumptions, platform requirements, and template constraints
- Produces an error message early, before the program runs

```cpp
static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
```

```cpp
template <typename T>
void serialize(const T& value)
{
    static_assert(std::is_trivially_copyable_v<T>,
                  "serialize() requires a trivially copyable type");

    // ...
}
```

**Choose the right tool**

- **assert**: runtime debug check for programmer errors
- **static_assert**: compile-time rule
- **if/return/error message**: expected runtime problems
- **exception**: failure that should propagate to a higher layer

**Takeaway:** assertions help catch bugs early; they are not a substitute for real runtime error handling.
