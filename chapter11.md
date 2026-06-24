# Chapter 11: Function Overloading and Function Templates

Current learncpp.com Chapter 11 covers 11.1–11.9 below. Some older refreshers mention deleted functions or a separate “multiple template types” section; those ideas are folded into the relevant sections here.

## 11.1 Introduction to function overloading

- **Function overloading** lets multiple functions share the same name when their **parameter lists differ**.
- This gives one conceptual operation a single name (`print`, `min`, `swap`) across different argument types.
- The compiler chooses the best matching overload at the call site.
- Overloads must be in the **same scope** to form an overload set.
- **Return type alone cannot distinguish overloads**.

```cpp
#include <iostream>
#include <string_view>

void print(int x)                { std::cout << "int: " << x << '\n'; }
void print(double x)             { std::cout << "double: " << x << '\n'; }
void print(std::string_view sv)  { std::cout << "text: " << sv << '\n'; }

int main()
{
    print(42);
    print(3.14);
    print("hello");
}
```

**Rules of thumb**
- Same name, different parameters: okay.
- Same name, same parameters, different return type: not okay.
- Overloading improves readability, but only when the functions really represent the same action.

## 11.2 Function overload differentiation

- Two overloads are different if they differ in:
  - **number of parameters**
  - **parameter types**
  - **parameter order**
- The function **signature** is driven by the parameter list, not the return type.
- Type aliases do **not** create new overloads if they alias the same type.
- Top-level `const` on by-value parameters does not make a distinct overload (covered more in 11.5).

```cpp
void f(int x);
void f(double x);     // different type
void f(int x, int y); // different count
void f(double x, int y);

// void f(int x);     // same as first: redeclaration, not overload
// int  f(int x);     // error: return type alone can't overload

using Age = int;
// void f(Age x);     // same as f(int): not a new overload
```

**Important**
- Parameter names do not matter for overload differentiation.
- Default arguments do not create a new parameter list.
- References/pointers can produce different overloads when their types truly differ.

## 11.3 Function overload resolution and ambiguous matches

- When you call an overloaded function, the compiler performs **overload resolution**.
- Roughly, it prefers:
  1. **exact matches**
  2. **promotions** (`char -> int`, `float -> double`)
  3. other **standard conversions**
  4. **user-defined conversions**
- The best viable function wins.
- If no single overload is better than all others, the call is **ambiguous**.

```cpp
void print(int);
void print(double);

// print(5);    // exact match -> print(int)
// print(5.0);  // exact match -> print(double)
```

```cpp
void convert(long);
void convert(double);

int main()
{
    // convert(5); // ambiguous: int can convert to long or double
}
```

**How to fix ambiguity**
- pass an argument of the exact intended type
- use a cast when appropriate
- redesign the overload set
- add a **deleted overload** to explicitly ban bad calls

```cpp
void log(int);
void log(bool) = delete; // reject accidental bool conversions
```

## 11.4 Default arguments and function overloading

- **Default arguments** let trailing parameters be omitted.
- They work well with overloads, but can also create ambiguity.
- Defaults are substituted at the call site; they do **not** change the function’s identity.
- If two overloads are both callable because one supplies defaults, the call may be ambiguous.

```cpp
void print(int x);
void print(int x, int base = 10);

int main()
{
    // print(42);   // ambiguous: both overloads match
    // print(42, 2);  // calls second overload
}
```

Better:

```cpp
void printDecimal(int x);
void printBase(int x, int base = 10);
```

**Guidelines**
- Avoid overload sets where default arguments make multiple functions equally good.
- Prefer one clear interface over “clever” combinations of overloads + defaults.
- Put defaults on the **trailing** parameters only.

## 11.5 Function overloads and const parameters

- For **by-value** parameters, top-level `const` is ignored for overloading:

```cpp
void f(int);
// void f(const int); // same signature: error
```

- For **references** and **pointers**, constness can matter:

```cpp
#include <iostream>
#include <string>

void inspect(std::string& s)       { std::cout << "mutable\n"; }
void inspect(const std::string& s) { std::cout << "const\n"; }

int main()
{
    std::string name { "Ada" };
    inspect(name);    // mutable overload
    inspect("Grace"); // const overload (temporary binds to const ref)
}
```

**Refresher**
- `T&` and `const T&` are different parameter types.
- `T*` and `const T*` are different parameter types.
- Top-level const on a value parameter (`int`, `double`, etc.) is not useful for overloading.
- Overloading on const-reference is common for read-only vs modifiable access.

## 11.6 Function templates

- A **function template** is a pattern for generating functions.
- `T` is a **template type parameter**.
- The compiler usually **deduces** template arguments from the call.
- Templates remove duplicated code when logic is the same across types.

```cpp
template <typename T>
T max(T a, T b)
{
    return (a < b) ? b : a;
}
```

```cpp
int i { max(1, 2) };
double d { max(3.5, 2.5) };
```

Complete multi-type example with deduced return type:

```cpp
template <typename T, typename U>
auto max_any(T a, U b)
{
    return (a < b) ? b : a; // return type deduced from expression
}

int main()
{
    auto a = max_any(2, 3.5);     // double
    auto b = max_any('a', 100);   // int
}
```

**Notes**
- `typename` and `class` mean the same thing here.
- If template argument deduction can’t find one consistent type, the call fails.
- `auto` return type deduction is often convenient in mixed-type templates.

## 11.7 Function template instantiation

- A template is not a real function by itself; it becomes one through **instantiation**.
- Each unique template argument set generates a distinct function.
- Instantiation usually happens **implicitly** when the template is called.
- You can also provide template arguments explicitly.

```cpp
template <typename T>
T square(T x)
{
    return x * x;
}

int main()
{
    auto a = square(3);     // instantiates square<int>
    auto b = square(2.5);   // instantiates square<double>
    auto c = square<int>(4); // explicit template argument
}
```

**Key idea**
- One template definition can produce many concrete functions:
  - `square<int>`
  - `square<double>`
  - `square<long long>`

**Practical implication**
- Template errors often appear at the **use site**, because that’s when instantiation happens.
- If a template is never used, no concrete function may be generated.

## 11.8 Abbreviated function templates (C++20)

- C++20 allows template parameters to be written directly with `auto`.
- This is shorthand for a function template.
- It is great for simple generic utilities.

```cpp
#include <iostream>

auto square(auto x)
{
    return x * x;
}

void printTwice(const auto& value)
{
    std::cout << value << ' ' << value << '\n';
}
```

Equivalent long form:

```cpp
template <typename T>
auto square(T x)
{
    return x * x;
}
```

**When it helps**
- quick generic helpers
- readable one-parameter templates
- generic lambdas and modern C++ style

**When named template parameters are clearer**
- multiple related template parameters
- cases where parameters must be the same deduced type
- constraints/concepts that need explicit names
- more complex return-type logic

## 11.9 Non-type template parameters

- A **non-type template parameter (NTTP)** is a compile-time value, not a type.
- Common uses:
  - fixed sizes
  - compile-time policies
  - selecting behavior without runtime overhead
- Each distinct value produces a separate instantiation.

```cpp
#include <array>
#include <cstddef>
#include <iostream>

template <std::size_t N>
void print(const std::array<int, N>& arr)
{
    for (std::size_t i = 0; i < N; ++i)
        std::cout << arr[i] << ' ';
    std::cout << '\n';
}

int main()
{
    std::array<int, 3> a { 1, 2, 3 };
    std::array<int, 5> b { 4, 5, 6, 7, 8 };

    print(a); // N = 3
    print(b); // N = 5
}
```

Another simple example:

```cpp
template <int N>
int scale(int value)
{
    return value * N;
}

// scale<2>(10) -> 20
// scale<5>(10) -> 50
```

**Takeaway**
- Type template parameters vary the **kind of data**.
- Non-type template parameters vary a **compile-time value**.
