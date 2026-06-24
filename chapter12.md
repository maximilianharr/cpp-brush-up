# Chapter 12: Compound Types: References and Pointers

Concise refresher for the LearnCpp Chapter 12 lesson pages covering references, pointers, passing/returning by reference or address, and related type deduction. Quiz content omitted.

## 12.1 Introduction to compound types: References and pointers

- A **compound type** is built from another type.
- In this chapter, the main compound types are:
  - **references**: aliases for existing objects
  - **pointers**: objects that store addresses
- Both let you access an existing object indirectly.
- **References** are usually simpler and safer when “must refer to a valid object” is the rule.
- **Pointers** are more flexible when “may be absent”, “may change target”, or “array traversal” matters.

```cpp
int value{42};

int& ref{value};   // ref aliases value
int* ptr{&value};  // ptr stores value's address

ref = 10;          // changes value
*ptr = 20;         // also changes value
```

**Mental model**
- Reference = “another name for the same object”.
- Pointer = “an address you can follow”.

## 12.2 Value categories (lvalues and rvalues)

- An **lvalue** is an expression with identity: it has a persistent object behind it.
- An **rvalue** is a temporary value or a value without stable identity.
- Variables are usually lvalues.
- Literals and most temporary results are rvalues.
- Non-const lvalue references bind only to lvalues.
- `const T&` can bind to both lvalues and rvalues.

```cpp
int x{5};          // x is an lvalue
int y{x + 1};      // x + 1 is an rvalue

int& rx{x};        // ok
// int& bad{5};    // error: non-const lvalue ref can't bind to rvalue

const int& cr1{x}; // ok
const int& cr2{5}; // ok
```

**Why this matters**
- Binding rules depend on value category.
- Function parameter choice (`T`, `T&`, `const T&`, `T*`) affects what arguments are allowed.

## 12.3 Lvalue references

- An **lvalue reference** uses `&` in a declaration: `T&`.
- It must be initialized.
- It cannot be reseated to refer to another object later.
- Reading/writing through the reference reads/writes the original object.
- A reference is not a separate object with separate identity in normal use.

```cpp
int a{10};
int& ref{a};

ref = 25;     // modifies a
std::cout << a << '\n'; // 25

int b{30};
// ref = b;    // assigns b's value into a; does NOT reseat ref
```

**Key points**
- Prefer references when null is not meaningful.
- A reference can dangle if the referred object dies.

## 12.4 Lvalue references to const

- `const T&` is a **reference to const**.
- You can read through it, but not modify through it.
- It can bind to:
  - non-const lvalues
  - const lvalues
  - rvalues / temporaries
- Binding a temporary to `const T&` extends the temporary’s lifetime to the reference’s lifetime.

```cpp
int x{7};
const int& a{x};   // binds to non-const lvalue
const int& b{8};   // binds to rvalue

// a = 9;          // error: can't modify through const ref
std::cout << b << '\n';
```

**Use cases**
- Read-only aliasing
- Efficient parameter passing for expensive-to-copy objects
- Accepting temporaries naturally

## 12.5 Pass by lvalue reference

- Use `T&` when the function must modify the caller’s object.
- The argument must be a modifiable lvalue.
- This avoids a copy and preserves the caller’s original object.
- Because mutation is implicit at the call site, use this only when modification is intended and useful.

```cpp
void increment(int& x)
{
    ++x;
}

int main()
{
    int value{1};
    increment(value);
    std::cout << value << '\n'; // 2
}
```

**Good uses**
- modifying a parameter
- returning extra data via an output/in-out parameter when necessary
- expensive types that must be mutated

**Avoid when**
- the function is input-only
- the type is cheap to copy and mutation is unnecessary

## 12.6 Pass by const lvalue reference

- Use `const T&` for **read-only** parameters when copying would be unnecessary or expensive.
- It accepts non-const lvalues, const lvalues, and rvalues.
- This is common for large class types (`std::string`, `std::vector`, user-defined objects).
- For small cheap types (`int`, `double`, pointers, enums), pass-by-value is often simpler and just as good or better.

```cpp
#include <iostream>
#include <string>

void printName(const std::string& name)
{
    std::cout << name << '\n';
}

int main()
{
    std::string s{"Ada"};
    printName(s);
    printName("Grace"); // temporary binds to const ref
}
```

**Gotcha**
- If the argument needs implicit conversion, a temporary may be created anyway.
- `const T&` prevents modification through that parameter, not through other aliases.

## 12.7 Introduction to pointers

- A **pointer** stores the address of an object.
- Pointer declaration syntax uses `*`: `T* ptr`.
- Use `&obj` to get an address.
- Use `*ptr` to dereference and access the pointed-to object.
- Pointers have their own object identity and can be reassigned.

```cpp
int value{42};
int* ptr{&value};

std::cout << *ptr << '\n'; // 42
*ptr = 99;
std::cout << value << '\n'; // 99
```

**Reference vs pointer**
- **Reference**
  - must bind immediately
  - cannot be null
  - cannot be reseated
  - syntax is cleaner
- **Pointer**
  - can be null
  - can be reseated
  - needs explicit dereference
  - better for optional targets and array traversal

## 12.8 Null pointers

- A **null pointer** points to no object.
- Use `nullptr` in modern C++.
- Do not dereference a null pointer.
- Prefer `nullptr` over `0` or `NULL`.
- Distinguish:
  - **null**: intentionally points nowhere
  - **wild**: uninitialized pointer
  - **dangling**: points to an object that no longer exists

```cpp
int* ptr{nullptr};

if (ptr)
{
    std::cout << *ptr << '\n';
}

int x{5};
ptr = &x;
ptr = nullptr; // now safe "points nowhere"
```

**Dangling example**

```cpp
int* badPtr{};
{
    int temp{10};
    badPtr = &temp;
} // temp dies here

// *badPtr; // dangling: undefined behavior
```

## 12.9 Pointers and const

- With pointers, `const` can apply to:
  - the pointed-to object
  - the pointer itself
  - both
- Read right-to-left:
  - `const int* p` = pointer to const int
  - `int* const p` = const pointer to int
  - `const int* const p` = const pointer to const int

```cpp
int x{1};
int y{2};

const int* ptrToConst{&x}; // can't modify *ptrToConst through this pointer
ptrToConst = &y;           // can reseat

int* const constPtr{&x};   // can't reseat
*constPtr = 5;             // can modify x

const int* const both{&x}; // neither modify pointee nor reseat
```

**Rule of thumb**
- Use `const T*` for read-only access.
- Use `T* const` mostly for local implementation details.

## 12.10 Pass by address

- **Pass by address** means passing a pointer argument.
- Use it when:
  - the argument is optional
  - `nullptr` is a meaningful “no object” state
  - interfacing with C-style APIs
  - working naturally with arrays / pointer ranges
- Because pointers may be null, functions usually need validation.

```cpp
#include <iostream>
#include <string>

void printLength(const std::string* s)
{
    if (!s)
    {
        std::cout << "no string\n";
        return;
    }

    std::cout << s->size() << '\n';
}
```

**Guideline**
- If null is invalid, prefer a reference parameter.
- If null is meaningful, a pointer expresses that clearly.

## 12.11 Pass by address (part 2)

- Pointer parameters are also useful for:
  - optional output parameters
  - array traversal
  - range-style processing with begin/end pointers
- Arrays often decay to pointers when passed to functions, so size information is not preserved automatically.
- Pointer arithmetic moves by element, not byte:
  - `ptr + 1` points to the next element of type `T`
- Only do pointer arithmetic within the same array (or one-past-the-end).

```cpp
int sum(const int* begin, const int* end)
{
    int total{};
    for (const int* p{begin}; p != end; ++p)
    {
        total += *p;
    }
    return total;
}

int data[]{1, 2, 3, 4};
int result{sum(data, data + 4)};
```

**Notes**
- `data + 4` is the one-past-end pointer.
- One-past-end is valid for comparison, not dereference.
- Prefer `std::span`, iterators, or containers in modern code when available.

## 12.12 Return by reference and return by address

- Returning by reference/address avoids a copy, but only works safely if the returned object outlives the function.
- Safe sources:
  - function parameters
  - static objects
  - objects owned by the caller
  - subobjects/elements of longer-lived objects
- Never return a reference or pointer to a local variable.

```cpp
#include <vector>

int& getElement(std::vector<int>& v, std::size_t index)
{
    return v[index];
}

int main()
{
    std::vector<int> values{10, 20, 30};
    getElement(values, 1) = 99;
}
```

**Bad**

```cpp
int& badRef()
{
    int x{5};
    return x; // dangling reference
}
```

Returning by address is useful when “not found” should be representable:

```cpp
const int* find(const int* begin, const int* end, int target)
{
    for (const int* p{begin}; p != end; ++p)
        if (*p == target)
            return p;

    return nullptr;
}
```

**Takeaway**
- Returning a reference/pointer can be efficient and expressive.
- It is dangerous when lifetime is unclear.

## 12.13 In and out parameters

- **In parameter**: input only.
- **Out parameter**: function writes a result into it.
- **In-out parameter**: function reads and then modifies it.
- Prefer **return values** for primary outputs when practical.
- Use reference/pointer parameters only when multiple outputs or optional outputs are needed.

```cpp
#include <string_view>

bool parseDigit(std::string_view text, int& outValue)
{
    if (text.size() != 1 || text[0] < '0' || text[0] > '9')
        return false;

    outValue = text[0] - '0';
    return true;
}
```

**Choosing the interface**
- **By value**: small cheap input, callee gets its own copy
- **`const T&`**: read-only expensive input
- **`T&`**: required mutation / required out parameter
- **`T*`**: optional object / nullable out parameter

**Practical comparison**
- Use a **reference** when the caller must provide a valid object.
- Use a **pointer** when “no object” is a real possibility.
- Avoid hiding surprising mutations in non-const reference parameters.

## 12.14 Type deduction with pointers, references, and const

- `auto` generally drops:
  - top-level `const`
  - references
- `auto&` deduces a reference and preserves constness of the referred object.
- For pointers:
  - low-level const on the pointee is preserved
  - top-level const on the pointer itself is dropped unless explicitly written

```cpp
int x{5};
const int cx{x};
const int& rx{cx};

auto a{cx};    // int
auto b{rx};    // int
auto& c{rx};   // const int&

const int* p{&cx};
auto q{p};     // const int*

int* const cp{&x};
auto r{cp};    // int*   (top-level const on pointer dropped)
auto* s{cp};   // int*
```

**Useful patterns**
- `auto&` when you want aliasing
- `const auto&` when you want read-only aliasing
- `auto*` when you want pointer syntax to be obvious

**Final refresher**
- References are usually the default indirect-access tool when null/reseating are not needed.
- Pointers are the right tool for optionality, reseating, and pointer-range/array-style work.
