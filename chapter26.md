# Chapter 26: Templates and Classes

## 26.1 Template classes
[Template classes](https://www.learncpp.com/cpp-tutorial/template-classes/)

- A class template is a class blueprint parameterized by one or more template parameters.
- The compiler generates concrete classes when you instantiate the template with real arguments.
- Most class templates live entirely in header files because the compiler usually needs both the declaration and full definition at the point of instantiation.
- Common use cases: generic containers, wrappers, fixed utilities, policy-based types.

```cpp
#include <iostream>

template <typename T>
class Pair
{
private:
    T m_first{};
    T m_second{};

public:
    Pair(T first, T second)
        : m_first{ first }, m_second{ second }
    {
    }

    T first() const { return m_first; }
    T second() const { return m_second; }
    T max() const { return (m_first < m_second) ? m_second : m_first; }
};

int main()
{
    Pair<int> p1{ 3, 7 };
    Pair<double> p2{ 1.5, 0.5 };

    std::cout << p1.max() << '\n'; // 7
    std::cout << p2.max() << '\n'; // 1.5
}
```

Quick refresher:
- `template <typename T>` introduces the template parameter list.
- `Pair<int>` and `Pair<double>` are different types.
- Member functions are also templated as part of the class template.
- If you split declaration/definition into `.h` + `.cpp`, implicit instantiation usually fails unless you use explicit instantiation.

## 26.2 Template non-type parameters
[Template non-type parameters](https://www.learncpp.com/cpp-tutorial/template-non-type-parameters/)

- Non-type template parameters take compile-time values instead of types.
- Good for sizes, flags, indices, capacities, and other constants.
- Traditionally these were integral / enum / pointer-like values; modern C++ expands what is allowed.
- The value becomes part of the type: `Buffer<int, 8>` and `Buffer<int, 16>` are different types.

```cpp
#include <array>
#include <cstddef>
#include <iostream>

template <typename T, std::size_t N>
class Buffer
{
private:
    std::array<T, N> m_data{};

public:
    constexpr std::size_t size() const { return N; }
    T& operator[](std::size_t i) { return m_data[i]; }
    const T& operator[](std::size_t i) const { return m_data[i]; }
};

int main()
{
    Buffer<int, 4> values{};
    values[0] = 10;
    values[1] = 20;

    std::cout << values.size() << '\n'; // 4
    std::cout << values[0] + values[1] << '\n'; // 30
}
```

Key points:
- Prefer `std::size_t` for sizes.
- Since the parameter is compile-time, it enables static storage/layout decisions.
- Great for array wrappers and compile-time configuration.

## 26.3 Function template specialization
[Function template specialization](https://www.learncpp.com/cpp-tutorial/function-template-specialization/)

- A function template specialization replaces the primary template for one exact type (or exact template argument set).
- Use it when the generic implementation is wrong or inefficient for a specific type.
- Important: function templates cannot be partially specialized.
- In many cases, a normal overloaded function is simpler and preferred.

```cpp
#include <cstring>
#include <iostream>

template <typename T>
bool isEqual(T a, T b)
{
    return a == b;
}

template <>
bool isEqual<const char*>(const char* a, const char* b)
{
    return std::strcmp(a, b) == 0;
}

int main()
{
    std::cout << isEqual(3, 3) << '\n';                 // true
    std::cout << isEqual("abc", "abc") << '\n';         // true
}
```

Refresher notes:
- Without the specialization, `"abc" == "abc"` would compare addresses, not string contents.
- Specializations must exactly match the primary template’s signature after substitution.
- If overloading solves the problem cleanly, prefer overloading over specialization.

## 26.4 Class template specialization
[Class template specialization](https://www.learncpp.com/cpp-tutorial/class-template-specialization/)

- Class templates can be fully specialized for a specific type.
- Unlike function specializations, a class specialization can have a completely different implementation.
- This is useful when one type needs unique storage, behavior, or interface details.

```cpp
#include <iostream>

template <typename T>
class Storage
{
private:
    T m_value{};

public:
    explicit Storage(T value) : m_value{ value } {}
    void print() const { std::cout << m_value << '\n'; }
};

template <>
class Storage<bool>
{
private:
    bool m_value{};

public:
    explicit Storage(bool value) : m_value{ value } {}
    void print() const { std::cout << (m_value ? "true" : "false") << '\n'; }
};

int main()
{
    Storage<int> n{ 42 };
    Storage<bool> b{ true };

    n.print(); // 42
    b.print(); // true
}
```

Why this matters:
- `Storage<bool>` is a separate hand-written implementation selected for `bool`.
- The specialization does not need to mirror the primary template internally.
- This is the classic “generic version + custom exact-type version” pattern.

## 26.5 Partial template specialization
[Partial template specialization](https://www.learncpp.com/cpp-tutorial/partial-template-specialization/)

- Partial specialization means “specialize some template arguments, but keep others generic”.
- This is available for class templates (not function templates).
- It lets you write a more specific version when template arguments have a recognizable pattern.

```cpp
#include <iostream>

template <typename T, typename U>
class PairInfo
{
public:
    static void print()
    {
        std::cout << "different types\n";
    }
};

template <typename T>
class PairInfo<T, T>
{
public:
    static void print()
    {
        std::cout << "same types\n";
    }
};

int main()
{
    PairInfo<int, double>::print(); // different types
    PairInfo<int, int>::print();    // same types
}
```

Pattern to remember:
- Primary template handles the general case.
- Partial specialization handles a family of cases, such as `<T, T>`, `<T, int>`, or `<T*>`.
- The most specialized matching template wins.

## 26.6 Partial template specialization for pointers
[Partial template specialization for pointers](https://www.learncpp.com/cpp-tutorial/partial-template-specialization-for-pointers/)

- Pointers are a common specialization target because pointer semantics differ from plain values.
- A primary template that works for values may need different behavior for `T*`.
- Classic example: deep-copy the pointed-to object so the wrapper owns an independent value.

```cpp
#include <iostream>

template <typename T>
class Storage
{
private:
    T m_value{};

public:
    explicit Storage(T value) : m_value{ value } {}
    void print() const { std::cout << m_value << '\n'; }
};

template <typename T>
class Storage<T*>
{
private:
    T* m_value{};

public:
    explicit Storage(T* value)
        : m_value{ value ? new T{ *value } : nullptr }
    {
    }

    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;

    ~Storage()
    {
        delete m_value;
    }

    void print() const
    {
        if (m_value)
            std::cout << *m_value << '\n';
    }
};

int main()
{
    int x{ 7 };
    Storage<int> a{ x };
    Storage<int*> b{ &x };

    a.print(); // 7
    b.print(); // 7
}
```

Takeaways:
- `Storage<T*>` matches any pointer type.
- Pointer specializations often change ownership, copying, printing, or null handling.
- If a specialization owns dynamic memory, define or restrict copy/move behavior deliberately.
- In real code, prefer standard owning types like `std::unique_ptr` when possible.

---

Chapter 26 memory jog:
- Class templates generate types from type/value parameters.
- Non-type parameters are compile-time constants and become part of the type.
- Function templates can be fully specialized, but not partially specialized.
- Class templates can be fully specialized and partially specialized.
- Pointer partial specialization is a common pattern because pointers often need custom behavior.
- Put template definitions where instantiating code can see them, usually in headers.
