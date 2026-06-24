# Chapter 17: Fixed-size Arrays: std::array and C-style Arrays

Current learncpp.com Chapter 17 has 17.1–17.8 below. Some older refreshers split out extra subtopics (array decay, pointer arithmetic, C-style strings, enum indexing, `reference_wrapper`, multidimensional arrays); this summary folds those into the current lesson structure.

**Quick chooser**
- **`std::array<T, N>`**: fixed size known at compile time; safest default for fixed-size arrays.
- **`std::vector<T>`**: size can change at runtime.
- **C-style arrays**: mainly legacy code, low-level interop, string literals / C APIs.

## 17.1 Introduction to std::array
[Introduction to std::array](https://www.learncpp.com/cpp-tutorial/introduction-to-stdarray/)

- `std::array<T, N>` is a fixed-size container in `<array>`.
- The length `N` is part of the type.
- It stores elements contiguously like a built-in array.
- Unlike C-style arrays, it has value semantics:
  - can be copied
  - can be assigned
  - can be returned by value
  - usually does **not** decay to a pointer by accident
- Good when the size is known at compile time and should stay fixed.
- Prefer it over C-style arrays in normal modern C++.

```cpp
#include <array>
#include <iostream>

int main()
{
    std::array<int, 4> scores{ 84, 92, 76, 100 };

    std::cout << scores[0] << '\n';

    for (int score : scores)
    {
        std::cout << score << ' ';
    }
}
```

**Why it is nicer than `int arr[4]`**
- knows its own length
- works better with standard library utilities
- copies as one object
- clearer function interfaces

**Rule of thumb**
- Fixed size -> `std::array`
- Variable size -> `std::vector`
- C interoperability / legacy ABI -> C-style array or pointer

## 17.1, 17.4, 17.5 Aggregate initialization, brace elision, and arrays of references for std::array
[Introduction to std::array](https://www.learncpp.com/cpp-tutorial/introduction-to-stdarray/) (aggregate initialization)
[std::array of class types, and brace elision](https://www.learncpp.com/cpp-tutorial/stdarray-of-class-types-and-brace-elision/) (brace elision)
[Arrays of references via std::reference_wrapper](https://www.learncpp.com/cpp-tutorial/arrays-of-references-via-stdreference_wrapper/) (arrays of references)

- `std::array` is usually initialized with a brace-enclosed list.
- Missing elements are value-initialized.
- Too many initializers is an error.
- Since `std::array` is an aggregate, initialization is straightforward.
- For class-type elements, each element is initialized normally.
- Older examples sometimes show extra braces; modern code is often simpler.

```cpp
#include <array>
#include <string>

std::array<int, 5> a{ 1, 2, 3 };          // -> 1, 2, 3, 0, 0
std::array<double, 3> b{};                // all zero-initialized
std::array<std::string, 3> names{
    "Alex", "Bella", "Casey"
};
```

**Brace elision / nested initialization**
- Nested element types may need their own braces or constructors.
- Readability matters more than clever brace packing.

```cpp
#include <array>
#include <utility>

std::array<std::pair<int, int>, 2> points{
    std::pair{1, 2},
    std::pair{3, 4}
};
```

**Arrays of references**
- Real arrays cannot store references.
- Use `std::reference_wrapper<T>` when you need array elements that alias existing objects.

```cpp
#include <array>
#include <functional>

int a{ 1 };
int b{ 2 };
int c{ 3 };

std::array<std::reference_wrapper<int>, 3> refs{ a, b, c };
refs[1].get() = 42; // modifies b
```

## 17.2, 17.6 std::array length and indexing, and enumerations as indices
[std::array length and indexing](https://www.learncpp.com/cpp-tutorial/stdarray-length-and-indexing/)
[std::array and enumerations](https://www.learncpp.com/cpp-tutorial/stdarray-and-enumerations/) (using enumerations as indices)

- Use `arr[index]` for fast unchecked access.
- Use `arr.at(index)` when you want bounds checking.
- Use `.size()` to get the length.
- `std::array` length is fixed and known at compile time.
- Useful helpers:
  - `.front()`
  - `.back()`
  - `.data()`

```cpp
#include <array>
#include <iostream>

int main()
{
    std::array<int, 4> arr{ 10, 20, 30, 40 };

    std::cout << arr.size() << '\n';   // 4
    std::cout << arr[2] << '\n';       // 30
    std::cout << arr.front() << '\n';  // 10
    std::cout << arr.back() << '\n';   // 40
}
```

**Index type note**
- `.size()` returns an unsigned size type (`std::size_t`).
- Be careful mixing signed loop counters with unsigned lengths.

**Using enumerations as indices**
- Great when indices have names.
- Common pattern: final enum value stores the count.

```cpp
#include <array>
#include <cstddef>

enum class Student : std::size_t
{
    kenny,
    kyle,
    stan,
    butters,
    count
};

int main()
{
    std::array<int, static_cast<std::size_t>(Student::count)> testScores{};
    testScores[static_cast<std::size_t>(Student::stan)] = 78;
}
```

## 17.3 Passing and returning std::array
[Passing and returning std::array](https://www.learncpp.com/cpp-tutorial/passing-and-returning-stdarray/)

- Pass by `const&` to avoid copying.
- Pass by non-const reference to modify.
- Pass by value only when an intentional copy is fine.
- Since the size is part of the type, `std::array<int, 5>` and `std::array<int, 6>` are different types.
- Templates help accept different lengths.
- Returning `std::array` by value is normal and safe.

```cpp
#include <array>
#include <cstddef>
#include <iostream>

template <typename T, std::size_t N>
void print(const std::array<T, N>& arr)
{
    for (const auto& elem : arr)
    {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}

std::array<int, 3> makeDigits()
{
    return { 1, 2, 3 };
}
```

```cpp
int main()
{
    auto digits{ makeDigits() };
    print(digits);
}
```

**Important contrast with C-style arrays**
- `std::array` keeps size information in the type.
- Built-in arrays usually decay to pointers when passed to functions.
- That is one of the main reasons `std::array` is safer.

## 17.7 Introduction to C-style arrays
[Introduction to C-style arrays](https://www.learncpp.com/cpp-tutorial/introduction-to-c-style-arrays/)

- A C-style array uses built-in syntax: `T name[length]`.
- Length must be a constant expression in standard C++.
- Elements are stored contiguously.
- The array size is fixed after declaration.
- But the array object itself has awkward semantics:
  - no whole-array assignment
  - no returning by value
  - easy pointer decay
- Mostly relevant for older code and low-level interfaces.

```cpp
#include <iostream>

int main()
{
    int scores[4]{ 84, 92, 76, 100 };

    std::cout << scores[0] << '\n';

    for (int i{ 0 }; i < 4; ++i)
    {
        std::cout << scores[i] << ' ';
    }
}
```

**Compared to `std::array`**
- same contiguous layout benefit
- much worse ergonomics and safety

**Use C-style arrays when**
- an API requires them
- you are working with C libraries
- you are maintaining existing legacy code

## 17.7 Aggregate initialization for C-style arrays
[Introduction to C-style arrays](https://www.learncpp.com/cpp-tutorial/introduction-to-c-style-arrays/) (aggregate initialization is covered within this same lesson)

- C-style arrays are also aggregates.
- Brace initialization works similarly:
  - exact number of initializers -> fully initialized
  - too few -> remaining elements zero-initialized
  - empty braces -> all zero-initialized
- The compiler can deduce the length from the initializer list.

```cpp
int a[5]{ 1, 2, 3 }; // -> 1, 2, 3, 0, 0
int b[3]{};          // -> 0, 0, 0
int c[]{ 4, 5, 6 };  // length deduced as 3
```

**Use symbolic constants for sizes**
- Avoid magic numbers in array lengths.
- Prefer `constexpr` or an enum/count value.

```cpp
#include <cstddef>

constexpr std::size_t maxStudents{ 5 };
int scores[maxStudents]{};
```

**Character arrays**
- A string literal can initialize a `char` array.
- The null terminator is included automatically.

```cpp
char name[]{ "Alex" }; // 'A' 'l' 'e' 'x' '\0'
```

## 17.8, 17.9, 17.10, 17.11 Accessing and manipulating C-style arrays
[C-style array decay](https://www.learncpp.com/cpp-tutorial/c-style-array-decay/) (decay)
[Pointer arithmetic and subscripting](https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/) (pointer arithmetic)
[C-style strings](https://www.learncpp.com/cpp-tutorial/c-style-strings/) (C-style strings)
[C-style string symbolic constants](https://www.learncpp.com/cpp-tutorial/c-style-string-symbolic-constants/) (string symbolic constants)

- `arr[i]` works for C-style arrays too.
- But built-in arrays often **decay to pointers**:
  - when passed to functions
  - in many expressions
- After decay, length information is gone.
- This is the core built-in-array hazard.

```cpp
#include <cstddef>
#include <iostream>

void print(const int* arr, std::size_t length)
{
    for (std::size_t i{ 0 }; i < length; ++i)
    {
        std::cout << arr[i] << ' ';
    }
}
```

```cpp
int main()
{
    int values[]{ 10, 20, 30, 40 };
    print(values, 4); // values decays to int*
}
```

**Decay refresher**
- `int values[4]` is an array object.
- In many contexts it becomes `int*` pointing at the first element.
- `sizeof(values)` works on the real array, not after decay.

**Pointer arithmetic and subscripting**
- `arr[i]` means `*(arr + i)`.
- Pointer arithmetic moves by elements, not bytes.

```cpp
int values[]{ 10, 20, 30 };
int* ptr{ values };

int x{ *(ptr + 1) }; // 20
int y{ ptr[2] };     // 30
```

**C-style strings**
- A C-style string is a `char` array terminated by `'\0'`.
- Functions reading C strings keep going until they hit the null terminator.
- If the terminator is missing, behavior is undefined.

```cpp
#include <iostream>

int main()
{
    char greeting[]{ "Hello" };
    std::cout << greeting << '\n'; // prints until '\0'
}
```

**String symbolic constants**
- For modern read-only text, prefer `std::string_view`.
- Use `const char*` or `char[]` when a C API specifically needs C strings.

```cpp
#include <string_view>

constexpr std::string_view title{ "Chapter 17" };
constexpr const char* cTitle{ "Chapter 17" };
```

## 17.8, 17.12, 17.13 Best practices for arrays, and multidimensional arrays
[Multidimensional C-style Arrays](https://www.learncpp.com/cpp-tutorial/multidimensional-c-style-arrays/) (multidimensional C-style arrays)
[Multidimensional std::array](https://www.learncpp.com/cpp-tutorial/multidimensional-stdarray/) (multidimensional std::array)
(no single page covers the general "best practices for arrays" bullets below; closest is the Chapter 17 summary/quiz page)

- Prefer `std::array` over C-style arrays for fixed-size collections.
- Prefer `std::vector` when the length can vary.
- Use C-style arrays mostly for:
  - interop with C / old APIs
  - raw string buffers
  - existing legacy code
- Prefer named constants or enum counts for sizes and indices.
- Avoid relying on array decay.
- Pass built-in arrays with an explicit length, or better, use modern wrappers.

**Multidimensional C-style arrays**
- Built-in multidimensional arrays are arrays of arrays.
- Layout is row-major.
- All later dimensions must be known in function parameters.

```cpp
#include <iostream>

int main()
{
    int board[2][3]{
        { 1, 2, 3 },
        { 4, 5, 6 }
    };

    std::cout << board[1][2] << '\n'; // 6
}
```

```cpp
void printBoard(const int board[][3], int rows)
{
    for (int r{ 0 }; r < rows; ++r)
    {
        for (int c{ 0 }; c < 3; ++c)
        {
            std::cout << board[r][c] << ' ';
        }
        std::cout << '\n';
    }
}
```

**Modern preference**
- If you need a fixed 2D structure in modern C++, nested `std::array` is usually clearer:

```cpp
#include <array>

using Board = std::array<std::array<int, 3>, 2>;
Board board{ { {1, 2, 3}, {4, 5, 6} } };
```

**Final refresher**
- fixed compile-time size + modern C++ -> `std::array`
- resizable collection -> `std::vector`
- legacy / C interop / raw buffers -> C-style arrays
