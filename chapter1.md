# Chapter 1: C++ Basics

## 1.1 Statements and the structure of a program
- A C++ program is built from **statements**: instructions that tell the program what to do.
- Statements usually end with a semicolon `;`.
- Execution starts in `main()`.
- A function body is enclosed in `{}`.
- Statements generally run top to bottom.
- `return 0;` reports successful completion from `main()`.
- `#include` brings in library declarations (such as iostream).
- Think of the basic shape as: includes -> `main()` -> statements.

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello, world!\n";
    return 0;
}
```

## 1.2 Comments
- Comments are for humans; the compiler ignores them.
- Use `//` for single-line comments.
- Use `/* ... */` for block comments.
- Prefer comments that explain **why**, not obvious **what**.
- Keep comments accurate; stale comments are worse than no comments.
- Comments are useful for intent, assumptions, and tricky logic.
- Avoid over-commenting simple code.

```cpp
int main()
{
    int width { 5 }; // starting width in tiles

    /* Temporary test:
       keep this input small while debugging. */
    width = width + 2;

    return 0;
}
```

## 1.3 Introduction to objects and variables
- An **object** is a region of memory that can hold a value.
- A **variable** is the name used to access that object.
- Every variable has a **type** (`int`, `double`, `char`, etc.).
- The type determines what kind of data the variable stores.
- Variables must be declared before use.
- Reading a variable gets its current value.
- Writing to a variable changes its stored value.

```cpp
int main()
{
    int apples { 3 };
    int oranges { 4 };
    int total { apples + oranges };

    return 0;
}
```

## 1.4 Variable assignment and initialization
- **Initialization** gives a variable its first value when it is created.
- **Assignment** gives a value to an existing variable later.
- Prefer initializing variables immediately.
- Brace initialization is a strong default:
  - `int x{5};`
  - helps avoid narrowing conversions.
- Copy initialization (`int x = 5;`) is common and valid.
- Assignment uses `=` with an existing variable on the left.

```cpp
int main()
{
    int width { 5 };   // initialization
    int height = 7;    // copy initialization

    int area {};       // value-initialized to 0
    area = width * height; // assignment

    return 0;
}
```

## 1.5 Introduction to iostream: cout, cin, and endl
- Include `<iostream>` to use standard console I/O.
- `std::cout` writes output.
- `<<` inserts data into the output stream.
- `std::cin` reads input.
- `>>` extracts data from the input stream.
- `std::endl` writes a newline and flushes the output buffer.
- For plain line breaks, `'\n'` is often enough; use `std::endl` when flushing matters.
- Prompt before reading input so the user knows what to enter.

```cpp
#include <iostream>

int main()
{
    int age {};
    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "Next year you will be " << (age + 1) << '\n';
    return 0;
}
```

## 1.6 Uninitialized variables and undefined behavior
- A variable that was declared but not given a value is **uninitialized**.
- Reading an uninitialized local variable causes **undefined behavior**.
- Undefined behavior means anything can happen:
  - garbage output
  - inconsistent results
  - crashes
  - behavior that seems correct until it suddenly is not
- Always initialize variables before using them.
- Debug builds may appear to “work” and still be wrong.

```cpp
#include <iostream>

int main()
{
    int x; // uninitialized
    // std::cout << x << '\n'; // bad: undefined behavior

    int y { 0 }; // good
    std::cout << y << '\n';
    return 0;
}
```

## 1.7 Keywords and naming identifiers
- **Keywords** are reserved words in C++ (`int`, `return`, `if`, etc.).
- You cannot use keywords as names.
- **Identifiers** are names for variables, functions, types, and more.
- Identifiers:
  - can use letters, digits, and underscores
  - cannot start with a digit
  - are case-sensitive (`value` and `Value` differ)
- Prefer descriptive names.
- Avoid names starting with `_` or containing `__`; many are reserved.
- Pick a naming style and use it consistently.

```cpp
int main()
{
    int playerScore { 10 };
    int max_score { 100 };

    // int return { 5 };   // invalid: keyword
    // int 2items { 2 };   // invalid: starts with a digit

    return 0;
}
```

## 1.8 Whitespace and basic formatting
- C++ mostly ignores extra spaces, tabs, and line breaks.
- Whitespace exists mainly to make code readable.
- Good formatting reduces mistakes and speeds up review.
- Common habits:
  - indent code inside blocks
  - put one statement per line
  - use spaces around operators
  - keep brace placement consistent
- The compiler accepts ugly formatting; humans do not.

```cpp
// Readable
int main()
{
    int x { 2 };
    int y { 3 };
    int sum { x + y };
    return 0;
}

// Same meaning, poor style
int main(){int x{2};int y{3};int sum{x+y};return 0;}
```

## 1.9 Introduction to literals and operators
- A **literal** is a fixed value written directly in code.
- Common literal kinds:
  - integer: `42`
  - floating-point: `3.14`
  - character: `'A'`
  - string: `"hello"`
  - boolean: `true`
- **Operators** perform work on values.
- Common operators:
  - arithmetic: `+ - * / %`
  - assignment: `=`
  - comparison: `== != < > <= >=`
- Operators combine literals and variables into useful calculations.

```cpp
#include <iostream>

int main()
{
    int total { 2 + 3 * 4 };
    double price { 19.99 };
    char grade { 'A' };
    bool passed { total > 10 };

    std::cout << total << ' ' << price << ' ' << grade << ' ' << passed << '\n';
    return 0;
}
```

## 1.10 Introduction to expressions
- An **expression** is a combination of literals, variables, and operators that evaluates to a value.
- A single literal like `5` is an expression.
- A variable like `x` is an expression.
- `x + 2` is an expression too.
- Expressions are often used inside statements.
- Parentheses help group subexpressions and make intent clear.
- Precedence affects how expressions are evaluated.

```cpp
int main()
{
    int x { 4 };
    int y { 6 };

    int sum { x + y };         // expression: x + y
    int scaled { (x + y) * 2 }; // grouped expression

    return 0;
}
```

## 1.11 Developing your first program
- Typical beginner workflow:
  1. write a small program
  2. compile it
  3. run it
  4. fix errors
  5. improve it in small steps
- Start with tiny, testable programs.
- Make one change at a time when learning.
- Read compiler errors carefully; fix the first real error first.
- Use output to verify your assumptions.
- A good first program combines variables, input, output, and a calculation.

```cpp
#include <iostream>

int main()
{
    int first {};
    int second {};

    std::cout << "Enter two integers: ";
    std::cin >> first >> second;

    std::cout << first << " + " << second << " = " << (first + second) << '\n';
    return 0;
}
```
