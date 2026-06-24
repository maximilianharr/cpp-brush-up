# Chapter 22: Move Semantics and Smart Pointers

## 22.1 Introduction to smart pointers and move semantics
- Raw owning pointers are error-prone: easy to leak, double-delete, or forget ownership rules.
- Smart pointers wrap raw pointers in classes that manage lifetime automatically.
- Move semantics lets resources be transferred instead of copied.
- This matters for types that own expensive or unique resources:
  - dynamic memory
  - file handles
  - sockets
  - mutexes
- Core ownership models:
  - `std::unique_ptr`: one owner
  - `std::shared_ptr`: shared owners via reference count
  - `std::weak_ptr`: non-owning observer of a `shared_ptr` object
- Copying duplicates state; moving transfers ownership/state from one object to another.
- Moved-from objects must remain valid, but their specific value is usually unspecified.

```cpp
#include <memory>

auto p1 = std::make_unique<int>(42);   // exclusive owner
auto p2 = std::make_shared<int>(99);   // shared owner
```

- Before move semantics, returning resource-owning objects by value was awkward/expensive.
- With move support, temporary objects can hand off resources cheaply.
- Move semantics is typically implemented with:
  - rvalue references (`T&&`)
  - move constructor
  - move assignment operator
  - `std::move`
- Think of move as **resource pilfering**:
  - source gives up ownership
  - destination takes ownership

```cpp
class Buffer {
    int* data{};
    std::size_t size{};
public:
    Buffer(std::size_t n) : data{new int[n]}, size{n} {}
    ~Buffer() { delete[] data; }
};
```

- A class like `Buffer` is a classic candidate for move support.
- Without move semantics, copying such a type may require deep copy.
- With move semantics, a temporary `Buffer` can transfer its pointer instead.

## 22.2 R-value references
- An **lvalue** has identity/persistence; an **rvalue** is typically a temporary.
- `T&` binds to non-const lvalues.
- `const T&` can bind to lvalues and rvalues.
- `T&&` binds to modifiable rvalues.
- Rvalue references exist mainly to enable moving.

```cpp
int x{5};
int& lref{x};         // ok
const int& cref{5};   // ok
int&& rref{5};        // ok
// int&& bad{x};      // error: x is an lvalue
```

- Named rvalue reference variables are themselves lvalues when used by name.

```cpp
int&& ref{5};
// ref is an lvalue expression here
int& again{ref};      // ok
```

- Overload resolution can distinguish lvalues from rvalues.

```cpp
void process(const std::string& s) { /* read only */ }
void process(std::string&& s)      { /* may move from s */ }

std::string name{"Alex"};
process(name);                 // lvalue overload
process(std::string{"tmp"});   // rvalue overload
```

- Use `T&&` parameters when you may steal resources from temporaries.
- Use `const T&` when you only need to read.
- Rvalue references do **not** mean “always faster”; they are a tool for transfer of ownership/state.
- Perfect forwarding is a later, related use case, but Chapter 22 focuses on moving.

## 22.3 Move constructors and move assignment
- A **move constructor** initializes an object by taking resources from an rvalue.
- A **move assignment operator** replaces an existing object's resources by taking from an rvalue.
- Typical move steps:
  1. release destination's current resource (assignment only)
  2. steal source resource
  3. null/reset source

```cpp
class Buffer {
    int* data{};
    std::size_t size{};
public:
    Buffer(std::size_t n) : data{new int[n]}, size{n} {}
    ~Buffer() { delete[] data; }

    Buffer(const Buffer& other) : data{new int[other.size]}, size{other.size} {
        std::copy(other.data, other.data + size, data);
    }

    Buffer(Buffer&& other) noexcept
        : data{other.data}, size{other.size} {
        other.data = nullptr;
        other.size = 0;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
        return *this;
    }
};
```

- Mark moves `noexcept` when possible; standard containers can then prefer moving during reallocation.
- Self-assignment check matters for move assignment too.
- If your class manually manages a resource, think **Rule of Five**:
  - destructor
  - copy constructor
  - copy assignment
  - move constructor
  - move assignment
- If a type should not be copied, delete copy operations.

```cpp
Buffer makeBuffer() {
    Buffer b{1024};
    return b; // NRVO or move
}
```

- In modern C++, copy elision often removes copies/moves entirely, but move support still matters.

## 22.4 std::move
- `std::move` does **not** move by itself.
- It casts its argument to an rvalue expression so move-enabled operations can be selected.
- After moving from an object, only rely on it being valid for destruction/assignment/reset.

```cpp
#include <utility>
#include <string>

std::string a{"hello"};
std::string b{std::move(a)}; // b takes a's contents
// a is valid but unspecified
```

- Use `std::move` when you are done with an object's current value.
- Do **not** move from objects you still need unchanged.
- Do **not** `std::move` a `const` object expecting an actual move:
  - moves usually need to modify the source
  - const objects often fall back to copy

```cpp
std::vector<std::string> v;
std::string s{"abc"};

v.push_back(s);            // copies
v.push_back(std::move(s)); // moves
```

- `std::move` is common in:
  - move constructors/assignments
  - sink functions that take ownership
  - transferring `unique_ptr`

```cpp
std::unique_ptr<int> p1 = std::make_unique<int>(7);
std::unique_ptr<int> p2 = std::move(p1); // ownership transferred
```

- Treat `std::move` as saying: “it is okay to pilfer this object now”.

## 22.5 std::unique_ptr
- `std::unique_ptr<T>` represents exclusive ownership of a dynamically allocated object.
- It deletes its owned object automatically when the `unique_ptr` dies.
- Cannot be copied; can be moved.
- Usually create with `std::make_unique`.

```cpp
#include <memory>

auto p = std::make_unique<int>(42);
if (p) {
    std::cout << *p << '\n';
}
```

- Ownership transfer requires move:

```cpp
std::unique_ptr<int> makeInt() {
    return std::make_unique<int>(5);
}

auto p1 = makeInt();
auto p2 = std::move(p1); // p1 becomes empty
```

- Common operations:
  - `*p`, `p->member`
  - `p.get()` raw pointer access (non-owning)
  - `p.release()` give up ownership without deleting
  - `p.reset(ptr)` replace owned pointer
  - `p == nullptr` / `if (p)`
- Prefer passing:
  - by reference if not transferring ownership
  - by value / `T&&` if consuming ownership

```cpp
void use(const std::unique_ptr<int>& p) { /* observe */ }
void take(std::unique_ptr<int> p)       { /* owns now */ }
```

- Great default choice for heap allocation.
- Makes ownership explicit and local.
- Use `unique_ptr` unless you truly need shared ownership.

## 22.6 std::shared_ptr
- `std::shared_ptr<T>` supports shared ownership.
- Internally it uses a **reference count** in a control block.
- The managed object is deleted when the last owning `shared_ptr` goes away.
- Usually create with `std::make_shared`.

```cpp
#include <memory>

auto p1 = std::make_shared<int>(42);
auto p2 = p1; // shared ownership

std::cout << p1.use_count() << '\n'; // 2
```

- Copying a `shared_ptr` copies the pointer and increments the count.
- Destroying/resetting one decrements the count.
- Useful when ownership is genuinely shared across multiple objects/components.
- Downsides vs `unique_ptr`:
  - more overhead
  - less obvious ownership
  - risk of cycles

```cpp
class Person {
public:
    std::string name;
    std::shared_ptr<Person> partner{};

    Person(std::string n) : name{std::move(n)} {}
};
```

- If two `shared_ptr` owners point at each other, reference counts may never hit zero.

```cpp
auto lucy = std::make_shared<Person>("Lucy");
auto ricky = std::make_shared<Person>("Ricky");
lucy->partner = ricky;
ricky->partner = lucy; // cycle
```

- That cycle leaks unless the back-reference is made non-owning.
- Use `shared_ptr` for true shared lifetime, not just convenient access.

## 22.7 std::weak_ptr
- `std::weak_ptr<T>` observes an object managed by `shared_ptr` without owning it.
- It does **not** contribute to the reference count.
- Primary use: break `shared_ptr` cycles.
- Also useful for caches, parent links, and optional observers.

```cpp
#include <memory>

class Person {
public:
    std::string name;
    std::weak_ptr<Person> partner{};

    Person(std::string n) : name{std::move(n)} {}
};
```

- A `weak_ptr` cannot be dereferenced directly.
- Convert it to `shared_ptr` with `lock()` when you need temporary access.

```cpp
auto lucy = std::make_shared<Person>("Lucy");
auto ricky = std::make_shared<Person>("Ricky");
lucy->partner = ricky;
ricky->partner = lucy; // fine: weak back-reference, no cycle leak

if (auto p = lucy->partner.lock()) {
    std::cout << p->name << '\n';
}
```

- Common operations:
  - `expired()` — whether the object is already gone
  - `lock()` — get a `shared_ptr` if still alive
  - `use_count()` — owner count of associated `shared_ptr`

```cpp
std::weak_ptr<int> w;
{
    auto s = std::make_shared<int>(10);
    w = s;
}

if (w.expired()) {
    std::cout << "object destroyed\n";
}
```

- Rule of thumb:
  - `unique_ptr` for single ownership
  - `shared_ptr` for shared ownership
  - `weak_ptr` for non-owning links into a shared ownership graph
