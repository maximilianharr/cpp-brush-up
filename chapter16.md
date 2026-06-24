# Chapter 16: Dynamic Arrays: std::vector

Concise refresher based on the current LearnCpp Chapter 16 lesson list (quizzes omitted).

## 16.1 Introduction to containers and arrays
[Introduction to containers and arrays](https://www.learncpp.com/cpp-tutorial/introduction-to-containers-and-arrays/)
- A **container** stores and manages a collection of objects.
- An **array** stores elements of one type, usually in contiguous memory.
- Prefer standard containers over raw arrays in modern C++.
- `std::vector` = dynamic length, heap-backed, contiguous storage.
- `std::array` = fixed length, stack-friendly wrapper around a built-in array.
- Raw C-style arrays are mostly for interop or low-level cases.

```cpp
#include <array>
#include <vector>

std::array<int, 3> fixed{1, 2, 3};   // fixed-size
std::vector<int> dynamic{1, 2, 3};   // resizable
```

**Rule of thumb**
- Size known at compile time -> `std::array`
- Size changes at runtime -> `std::vector`

## 16.2 Introduction to std::vector and list constructors
[Introduction to std::vector and list constructors](https://www.learncpp.com/cpp-tutorial/introduction-to-stdvector-and-list-constructors/)
- Include `<vector>`.
- `std::vector<T>` tracks its own length.
- Parentheses and braces can mean different things.
- **Important:** `std::vector<int>(5)` is not the same as `std::vector<int>{5}`.

```cpp
#include <vector>

std::vector<int> a;          // empty
std::vector<int> b(5);       // 5 value-initialized ints: {0,0,0,0,0}
std::vector<int> c(5, -1);   // {-1,-1,-1,-1,-1}
std::vector<int> d{5};       // one element: {5}
std::vector<int> e{1, 2, 3}; // list constructor
```

Common operations:

```cpp
std::vector<int> v{1, 2, 3};
v.push_back(4);      // {1,2,3,4}
v.pop_back();        // {1,2,3}
auto n = v.size();   // current element count
bool empty = v.empty();
```

### Quick extra: 2D vectors
Use a vector of vectors when row lengths may vary or dimensions are runtime-driven.

```cpp
std::vector<std::vector<int>> matrix{
    {1, 2, 3},
    {4, 5, 6}
};

int x = matrix[1][2]; // 6
```

## 16.3 std::vector and the unsigned length and subscript problem
[std::vector and the unsigned length and subscript problem](https://www.learncpp.com/cpp-tutorial/stdvector-and-the-unsigned-length-and-subscript-problem/)
- `vector::size()` returns an unsigned type (`size_type`, usually `std::size_t`).
- Subscripting also expects an unsigned index.
- Mixing signed ints with unsigned sizes can cause warnings or bugs.
- A negative signed value converted to unsigned becomes a huge positive value.

```cpp
std::vector<int> v{10, 20, 30};
int i{-1};

// v[i]; // bad: -1 converts to a very large unsigned index
if (i >= 0 && static_cast<std::size_t>(i) < v.size())
    std::cout << v[static_cast<std::size_t>(i)] << '\n';
```

Safer habits:
- Validate signed indices before converting.
- Use `std::size_t` when naturally working with sizes/indices.
- Use `std::ssize()` when you want a signed length for loops/comparisons.

```cpp
for (std::size_t i{0}; i < v.size(); ++i)
    std::cout << v[i] << ' ';
```

## 16.4 Passing std::vector
[Passing std::vector](https://www.learncpp.com/cpp-tutorial/passing-stdvector/)
- Pass by **const reference** to read without copying.
- Pass by **non-const reference** to modify in-place.
- Pass by **value** only when you intentionally want a copy.

```cpp
void print(const std::vector<int>& v)
{
    for (int n : v)
        std::cout << n << ' ';
}

void doubleValues(std::vector<int>& v)
{
    for (int& n : v)
        n *= 2;
}
```

Generic version:

```cpp
template <typename T>
void print(const std::vector<T>& v)
{
    for (const auto& elem : v)
        std::cout << elem << ' ';
}
```

## 16.5 Returning std::vector and the elision optimization
[Returning std::vector, and an introduction to move semantics](https://www.learncpp.com/cpp-tutorial/returning-stdvector-and-an-introduction-to-move-semantics/)
- Returning a vector by value is normal and usually efficient.
- Compilers often apply **copy elision / NRVO**.
- If elision does not happen, move semantics usually transfer ownership of the buffer cheaply.
- Do **not** return a reference to a local vector.

```cpp
std::vector<int> makeSquares(int count)
{
    std::vector<int> result{};
    result.reserve(count);

    for (int i{1}; i <= count; ++i)
        result.push_back(i * i);

    return result; // elided or moved
}
```

Bad:

```cpp
// std::vector<int>& makeBad(); // never return ref to local data
```

## 17.1 Introduction to std::array
[Introduction to std::array](https://www.learncpp.com/cpp-tutorial/introduction-to-stdarray/)
- `std::array<T, N>` is a fixed-size container.
- Size is part of the type.
- Unlike raw arrays, it works well with copy, assignment, `.size()`, and range-for.
- Prefer it over built-in arrays when size is compile-time constant.

```cpp
#include <array>

std::array<int, 4> a{1, 2, 3, 4};
std::cout << a.size() << '\n';
std::cout << a[2] << '\n';
```

Compared to `std::vector`:
- `std::array`: fixed size
- `std::vector`: resizable

## 16.7 Arrays, loops, and sign challenge solutions
[Arrays, loops, and sign challenge solutions](https://www.learncpp.com/cpp-tutorial/arrays-loops-and-sign-challenge-solutions/)
- Loops over arrays/vectors often run into signed/unsigned mismatches.
- Forward loops are straightforward; reverse loops need extra care.
- Unsigned reverse loops can underflow if written poorly.

Forward loop:

```cpp
std::array<int, 4> a{10, 20, 30, 40};

for (std::size_t i{0}; i < a.size(); ++i)
    std::cout << a[i] << ' ';
```

Signed loop with `std::ssize()`:

```cpp
for (auto i{0}; i < std::ssize(a); ++i)
    std::cout << a[static_cast<std::size_t>(i)] << ' ';
```

Safe reverse idea:

```cpp
for (auto i{std::ssize(a) - 1}; i >= 0; --i)
    std::cout << a[static_cast<std::size_t>(i)] << ' ';
```

In practice, range-for is often cleaner when you do not need indices.

## 16.8 std::array and std::vector element initialization
- Initialization details matter.
- `()` often means “create N elements”.
- `{}` often means “use exactly these elements”.

```cpp
std::array<int, 5> a{};      // all zero
std::array<int, 5> b{1, 2};  // {1,2,0,0,0}

std::vector<int> v1(5);      // 5 zeros
std::vector<int> v2{5};      // one element: 5
std::vector<int> v3{1, 2, 3};
```

Resize also value-initializes new elements:

```cpp
std::vector<int> v{1, 2};
v.resize(5); // {1,2,0,0,0}
```

## 16.9 Array indexing and length using enumerators
[Array indexing and length using enumerators](https://www.learncpp.com/cpp-tutorial/array-indexing-and-length-using-enumerators/)
- Enumerators make named indices clearer than magic numbers.
- A final enumerator can represent the element count.
- This works especially well when entries have stable named positions.

```cpp
enum StudentNames
{
    kenny,
    kyle,
    stan,
    butters,
    max_students
};

std::array<int, max_students> testScores{};
testScores[stan] = 76;
```

Benefits:
- readable indices
- one source of truth for length
- fewer hard-coded numbers

## 16.10 Using range-based for loops with containers
[Range-based for loops (for-each)](https://www.learncpp.com/cpp-tutorial/range-based-for-loops-for-each/)
- Use range-for when you care about elements more than indices.
- By value -> copies each element.
- By reference -> can modify elements.
- By const reference -> avoids copies and prevents modification.

```cpp
std::vector<int> v{1, 2, 3};

for (int x : v)              // copy
    std::cout << x << ' ';

for (int& x : v)             // modify
    x *= 2;

for (const int& x : v)       // read-only, no copy
    std::cout << x << ' ';
```

Use indices instead when you need:
- element position
- parallel access to multiple containers
- neighbor lookups (`v[i - 1]`, `v[i + 1]`)

## 16.11 std::vector resizing/capacity and stack behavior
[std::vector resizing and capacity](https://www.learncpp.com/cpp-tutorial/stdvector-resizing-and-capacity/)
[std::vector and stack behavior](https://www.learncpp.com/cpp-tutorial/stdvector-and-stack-behavior/)
- `std::vector` is efficient at the **back**.
- `push_back()` / `pop_back()` make it a natural stack-like container.
- Middle/front insertion and erasure are comparatively expensive.

```cpp
std::vector<int> stack{};

stack.push_back(10);
stack.push_back(20);
stack.push_back(30);

std::cout << stack.back() << '\n'; // 30
stack.pop_back();                  // removes 30
```

### size vs capacity
- `size()` = number of active elements
- `capacity()` = currently allocated storage

```cpp
std::vector<int> v{};
v.reserve(10);   // capacity >= 10, size still 0
v.resize(3);     // size becomes 3
```

### resize vs reserve
- `reserve(n)` changes capacity only
- `resize(n)` changes logical length
- Growing via `resize()` value-initializes new elements

```cpp
std::vector<int> v{1, 2, 3};
v.reserve(20); // size 3, capacity at least 20
v.resize(5);   // size 5, now {1,2,3,0,0}
```

Use `reserve()` when many `push_back()` calls are coming.

## 16.12 std::vector<bool>
[std::vector<bool>](https://www.learncpp.com/cpp-tutorial/stdvector-bool/)
- `std::vector<bool>` is a special packed representation, not a normal `vector<T>`.
- It may store bits instead of separate `bool` objects.
- Element access can return proxy objects rather than real `bool&`.
- This can be surprising in generic code.

```cpp
std::vector<bool> flags{true, false, true};
flags[1] = true;
```

Guideline:
- Use `std::vector<bool>` only if bit-packing is actually helpful.
- Otherwise prefer something unsurprising like:
  - `std::vector<char>`
  - `std::vector<std::uint8_t>`
  - `std::bitset<N>` for fixed-size bit sets
