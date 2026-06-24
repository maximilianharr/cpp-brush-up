# Chapter 19: Dynamic Memory

Refresher based on the current LearnCpp Chapter 19 lesson list found via web search (quizzes omitted).  
Older LearnCpp revisions split out some related topics (destructors, pointers and `const`, `void*`, move semantics); those are folded into the most relevant sections below.

**Default modern rule**
- Prefer automatic storage when possible.
- Prefer `std::vector` / `std::string` for dynamic sequences.
- Prefer `std::unique_ptr` for exclusive heap ownership.
- Reach for raw `new` / `delete` only when teaching, interoperating, or implementing low-level ownership.

## 19.1 Dynamic memory allocation with new and delete

- Dynamic allocation gives an object a lifetime independent of the current scope.
- Use it when:
  - size or lifetime is only known at runtime
  - an object must outlive the creating scope
  - ownership must be transferred
- `new` allocates memory and returns a pointer.
- `delete` destroys the object and frees the memory.
- `new T` may leave fundamental types uninitialized.
- `new T{}` value-initializes.
- Allocation failure normally throws `std::bad_alloc`.
- Every raw owning pointer needs a clear owner and one matching `delete`.
- After `delete`, the pointer becomes **dangling**.
- Setting it to `nullptr` helps avoid accidental reuse.
- `delete nullptr;` is safe.
- Raw `new` / `delete` is easy to get wrong; modern C++ prefers RAII wrappers.

```cpp
#include <iostream>

int main()
{
    int* value{ new int{42} }; // heap object
    std::cout << *value << '\n';

    delete value;              // destroy + free
    value = nullptr;           // no dangling reuse
}
```

**Mental model**
- raw pointer can mean either:
  - **owner** (must eventually free)
  - **observer** (just points at something owned elsewhere)
- Avoid mixing those roles.

## 19.2 The heap, stack, and free store

- **Stack / automatic storage**
  - locals and parameters
  - fast allocation/deallocation
  - lifetime usually ends at scope exit
  - limited size
- **Heap / free store**
  - dynamic storage managed with allocation functions / `new`
  - flexible lifetime and runtime sizing
  - slower, more fragmented, easier to misuse
- “Heap” and “free store” are often used interchangeably in practice.
- Stack is usually best by default because lifetime is simple and automatic.
- Heap allocation is a tool, not a performance feature.
- Recursion and huge local objects can exhaust stack space.
- Dynamic memory is helpful when data is large, variable-sized, or must escape scope.

```cpp
#include <memory>

int main()
{
    int local{7};                           // stack
    auto heapValue{ std::make_unique<int>(9) }; // heap, RAII-managed
}
```

**Rule of thumb**
- start with a normal local variable
- if size varies, try `std::vector`
- if one heap object needs ownership, try `std::unique_ptr`
- only then consider raw `new`

## 19.3 Dynamic arrays

- Use `new[]` when the array length is known only at runtime.
- Destroy with `delete[]`, not `delete`.
- Raw dynamic arrays are low-level:
  - pointer does **not** know the element count
  - no bounds checking
  - manual cleanup required
- Store the length separately.
- `new int[count]` leaves built-in elements uninitialized.
- `new int[count]{}` value-initializes all elements to zero.
- Prefer `std::size_t` for sizes and indices.
- For read-only access, `const T*` means “pointer to const data”.
- Passing raw arrays usually means passing both pointer and count.

```cpp
#include <cstddef>
#include <iostream>

int sum(const int* data, std::size_t count)
{
    int total{};
    for (std::size_t i{0}; i < count; ++i)
        total += data[i];
    return total;
}

int main()
{
    std::size_t count{5};
    int* data{ new int[count]{1, 2, 3, 4, 5} };

    std::cout << sum(data, count) << '\n';

    delete[] data;
}
```

**Pointers and `const` refresher**
- `const int* p` -> cannot modify the `int` through `p`
- `int* const p` -> `p` itself cannot point elsewhere
- `const int* const p` -> neither can change through `p`

## 19.4 std::vector and dynamic arrays

- `std::vector<T>` is the standard dynamic array type.
- It manages heap allocation automatically.
- It remembers its size.
- It grows/shrinks as needed.
- It cleans up in its destructor.
- It is almost always better than raw `new[]`.
- Elements are contiguous, so interop with C APIs is still possible via `.data()`.
- Use:
  - `.size()` for current element count
  - `.push_back()` / `.emplace_back()` to append
  - `.resize()` to change logical size
  - `.reserve()` to pre-allocate capacity
  - `.at()` when you want bounds checking
- If you think “dynamic array”, think `std::vector` first.

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> values{ 1, 2, 3 };
    values.push_back(4);
    values.resize(6); // adds 0, 0

    for (int v : values)
        std::cout << v << ' ';
}
```

**Why `vector` wins**
- no manual `delete[]`
- exception safe
- copy/move behavior already implemented
- fewer leaks, fewer dangling pointers, clearer ownership

## 19.5 Dynamically allocating structs and classes

- `new T{...}` works for user-defined types too.
- Access members through `->`.
- If a class owns a raw dynamic resource, it usually needs:
  - a destructor
  - copy control (deep copy or deleted copy)
  - possibly move operations
- This is the **Rule of Three/Five** territory.
- Better: design for the **Rule of Zero** by using members like `std::vector`, `std::string`, and `std::unique_ptr`.
- If deleting through a base-class pointer, the base class usually needs a **virtual destructor**.

```cpp
#include <cstddef>

class Buffer
{
    int* data{};
    std::size_t size{};

public:
    explicit Buffer(std::size_t n)
        : data{ new int[n]{} }, size{ n }
    {
    }

    ~Buffer()
    {
        delete[] data;
    }
};
```

**`void*` refresher**
- `void*` can point to any object type.
- It does **not** remember what type it points to.
- You cannot directly dereference it.
- You must cast it back first.
- In modern C++, it is mostly for low-level / C interop.
- Avoid it in normal application code.

```cpp
int value{5};
void* raw{ &value };

int* typed{ static_cast<int*>(raw) };
// std::cout << *typed << '\n';
```

## 19.6 std::unique_ptr

- `std::unique_ptr<T>` is the default smart pointer for exclusive ownership.
- It owns one object and deletes it automatically.
- Prefer `std::make_unique<T>(...)` over manual `new`.
- `unique_ptr` cannot be copied.
- It **can** be moved, which transfers ownership.
- After a move, the source pointer is typically empty (`nullptr`).
- Great for:
  - owning polymorphic objects
  - factory returns
  - optional heap allocation with clear lifetime
- `std::unique_ptr<T[]>` exists for arrays, but `std::vector<T>` is usually a better fit.

```cpp
#include <memory>

std::unique_ptr<int> makeValue()
{
    return std::make_unique<int>(42);
}

int main()
{
    auto p1{ makeValue() };
    auto p2{ std::move(p1) }; // transfer ownership
}
```

**Move-semantics connection**
- moving does not copy the resource
- it transfers ownership/state
- smart pointers are a clean, practical introduction to move semantics

Useful members:
- `*p`, `p->member`
- `p.get()` -> raw non-owning pointer
- `p.release()` -> give up ownership without deleting
- `p.reset()` -> replace or clear owned object

## 19.7 When and when not to use new, delete, and smart pointers

**Prefer these first**
- plain local objects
- `std::array` for fixed-size compile-time arrays
- `std::vector` / `std::string` for runtime-sized sequences
- `std::unique_ptr` for exclusive ownership

**Use raw `new` / `delete` sparingly**
- implementing low-level containers / allocators
- teaching / understanding lifetime mechanics
- interfacing with older APIs that require raw ownership

**Use raw pointers mainly as non-owning observers**
- function parameters that just inspect an object
- optional “maybe points to something” references
- interop boundaries

```cpp
#include <iostream>

void print(const int* p)   // observe only
{
    if (p)
        std::cout << *p << '\n';
}
```

**Choose the tool by ownership**
- exactly one owner -> `std::unique_ptr`
- shared lifetime really required -> `std::shared_ptr`
- no ownership -> raw pointer or reference

**Avoid**
- heap allocation just because “objects should live on the heap”
- `void*` unless an API truly demands typeless storage
- manual `delete` spread across multiple code paths

RAII is the big idea:
- tie resource lifetime to object lifetime
- let constructors acquire and destructors release
- make cleanup automatic even on exceptions / early returns

## 19.8 Common dynamic memory allocation mistakes

- **Memory leak**: allocated memory is never freed.
- **Double delete**: same memory freed twice.
- **Mismatched delete**: `new[]` with `delete`, or `new` with `delete[]`.
- **Dangling pointer**: pointer still used after the object is destroyed.
- **Lost pointer**: overwriting the only owning pointer leaks the old allocation.
- **Shallow copy of owning raw pointer**: two objects think they own the same memory.
- **Deleting non-heap memory**: only `delete` what came from `new`.
- **Deleting through wrong base type**: polymorphic base usually needs virtual destructor.
- **Using `void*` unsafely**: wrong cast means wrong type, wrong behavior, possible UB.

```cpp
int* p{ new int{5} };
int* q{ p };

delete p;
// delete q;   // bad: double delete
// std::cout << *q; // bad: use-after-free
q = nullptr;
```

Better:

```cpp
#include <memory>

auto p{ std::make_unique<int>(5) };
// automatic cleanup, no manual delete
```

**Best defense**
- minimize raw ownership
- prefer containers and smart pointers
- make ownership explicit
- keep lifetimes local and obvious
