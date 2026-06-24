# Chapter 15: More on Classes

## 15.1 The hidden “this” pointer and member function chaining
[The hidden "this" pointer and member function chaining](https://www.learncpp.com/cpp-tutorial/the-hidden-this-pointer-and-member-function-chaining/)
- Every non-static member function gets an implicit `this` pointer.
- `this` points to the current object, so `value = x;` is really `this->value = x;`.
- In a `const` member function, `this` is effectively a pointer to const object data.
- Static member functions do **not** get a `this` pointer.
- Returning `*this` by reference enables fluent/member-function chaining.
- Return `*this` when you want later calls to keep working on the same object.

```cpp
class Calc
{
private:
    int m_value{};

public:
    Calc& add(int x)
    {
        m_value += x;
        return *this;
    }

    Calc& sub(int x)
    {
        this->m_value -= x;
        return *this;
    }

    int get() const { return m_value; }
};

Calc c{};
int result{ c.add(5).sub(2).get() }; // 3
```

**Refresher:** use chaining for setters/builders; return `*this` by reference, not by value.

## 15.2 Classes and header files
[Classes and header files](https://www.learncpp.com/cpp-tutorial/classes-and-header-files/)
- Put the class declaration in a header (`.h` / `.hpp`).
- Put non-trivial member function definitions in a source file (`.cpp`).
- Use include guards or `#pragma once`.
- Define out-of-class member functions with `ClassName::functionName`.
- Small trivial functions may stay inline inside the class definition.
- Keep interface in headers, implementation in source files.

```cpp
// Point.h
#pragma once

class Point
{
private:
    int m_x{};
    int m_y{};

public:
    Point(int x, int y);
    void print() const;
};
```

```cpp
// Point.cpp
#include <iostream>
#include "Point.h"

Point::Point(int x, int y) : m_x{x}, m_y{y} {}

void Point::print() const
{
    std::cout << '(' << m_x << ", " << m_y << ")\n";
}
```

**Refresher:** class templates are a major exception: their full definitions usually live in headers.

## 15.3 Nested types (member types)
[Nested types (member types)](https://www.learncpp.com/cpp-tutorial/nested-types-member-types/)
- A class can define types inside itself: `enum`, `enum class`, `struct`, `class`, aliases.
- Nested types help group related concepts with the owning class.
- Access them with scope resolution: `ClassName::TypeName`.
- Nested types obey access control (`public` / `private`).
- A nested type does **not** automatically have access to a specific outer object.

```cpp
class TrafficLight
{
public:
    enum class State
    {
        red,
        yellow,
        green
    };

private:
    State m_state{ State::red };

public:
    void set(State state) { m_state = state; }
    State get() const { return m_state; }
};

TrafficLight light{};
light.set(TrafficLight::State::green);
```

**Refresher:** nest a type when it only makes sense in the context of that class.

## 15.4 Introduction to destructors
[Introduction to destructors](https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/)
- A destructor runs automatically when an object is destroyed.
- Name: `~ClassName()`.
- No parameters, no return type, only one destructor per class.
- Destructors are ideal for cleanup: memory, files, mutexes, handles, etc.
- This is the core of **RAII**: acquire in constructor, release in destructor.
- Local objects are destroyed at end of scope, in reverse construction order.

```cpp
class IntArray
{
private:
    int* m_data{};
    int m_length{};

public:
    IntArray(int length)
        : m_data{ new int[length]{} }, m_length{ length }
    {
    }

    ~IntArray()
    {
        delete[] m_data;
    }
};

void useArray()
{
    IntArray arr{ 10 }; // resource acquired
} // arr destroyed here, resource released automatically
```

**Refresher:** prefer owning standard-library types (`std::vector`, `std::string`, smart pointers) when possible — same RAII idea, less manual cleanup.

## 15.5 Class templates with member functions
[Class templates with member functions](https://www.learncpp.com/cpp-tutorial/class-templates-with-member-functions/)
- Class templates let one class work with many element types.
- Member functions can be defined inside the class or outside it.
- For out-of-class definitions, repeat the template parameter list.
- Most class-template definitions belong in headers so all translation units can instantiate them.

```cpp
template <typename T>
class Pair
{
private:
    T m_first{};
    T m_second{};

public:
    Pair(T first, T second);
    T max() const;
};

template <typename T>
Pair<T>::Pair(T first, T second) : m_first{ first }, m_second{ second } {}

template <typename T>
T Pair<T>::max() const
{
    return (m_first < m_second) ? m_second : m_first;
}

Pair<int> p{ 3, 7 };
```

**Refresher:** out-of-class template member definitions need both `template <typename T>` and `Class<T>::member`.

## 15.6 Static member variables
[Static member variables](https://www.learncpp.com/cpp-tutorial/static-member-variables/)
- A static data member belongs to the class, not to any one object.
- All objects share the same static member.
- Useful for counters, IDs, shared config, and class-wide state.
- Access with `ClassName::member`; prefer this over object syntax.
- Traditional form: declare in class, define once in a source file.
- Modern C++ also allows `inline static` members in the class definition.

```cpp
class User
{
private:
    inline static int s_nextId{ 1 };
    int m_id{};

public:
    User() : m_id{ s_nextId++ } {}
    int id() const { return m_id; }
    static int nextId() { return s_nextId; }
};

User a{};
User b{};
// a.id() == 1, b.id() == 2
```

**Refresher:** static members exist even if no objects of the class exist.

## 15.7 Static member functions
[Static member functions](https://www.learncpp.com/cpp-tutorial/static-member-functions/)
- Static member functions are class-level functions.
- They have no `this` pointer.
- They can access only static members directly.
- Call them with `ClassName::function()`.
- Good for utilities, counters, factories, and helpers tightly related to a class.

```cpp
class Math
{
public:
    static int square(int x)
    {
        return x * x;
    }
};

int value{ Math::square(6) }; // 36
```

```cpp
class VisitorCounter
{
private:
    inline static int s_count{};

public:
    VisitorCounter() { ++s_count; }
    static int count() { return s_count; }
};
```

**Refresher:** if a function does not need object state, consider whether it should be `static`.

## 15.8 Friend non-member functions
[Friend non-member functions](https://www.learncpp.com/cpp-tutorial/friend-non-member-functions/)
- A friend function is not a member, but it may access private/protected members.
- Declare friendship inside the class with the `friend` keyword.
- Friendship is explicit and selective.
- Common use cases: symmetric operators (`operator<<`, `operator==`) and tightly coupled helpers.
- Use sparingly: friendship weakens encapsulation.

```cpp
class Vec2
{
private:
    int m_x{};
    int m_y{};

public:
    Vec2(int x, int y) : m_x{x}, m_y{y} {}

    friend Vec2 operator+(const Vec2& a, const Vec2& b);
};

Vec2 operator+(const Vec2& a, const Vec2& b)
{
    return Vec2{ a.m_x + b.m_x, a.m_y + b.m_y };
}
```

**Refresher:** friend functions are still normal non-members for overload resolution and interface design.

## 15.9 Friend classes and friend member functions
[Friend classes and friend member functions](https://www.learncpp.com/cpp-tutorial/friend-classes-and-friend-member-functions/)
- A friend class gets access to private/protected members of another class.
- You can also friend one specific member function instead of the whole class.
- Friendship is **not** mutual, inherited, or transitive.
- Forward declarations are often needed.
- Prefer the narrowest friendship that solves the design problem.

```cpp
class Storage;

class Display
{
public:
    void print(const Storage& storage) const;
};

class Storage
{
private:
    int m_value{ 42 };

    friend void Display::print(const Storage& storage) const;
};

void Display::print(const Storage& storage) const
{
    std::cout << storage.m_value << '\n';
}
```

```cpp
class Inspector;

class Secret
{
    friend class Inspector;
private:
    int m_code{ 1234 };
};
```

**Refresher:** friend one function when possible; friend a whole class only when broad access is truly intended.

## 15.10 Ref qualifiers
[Ref qualifiers](https://www.learncpp.com/cpp-tutorial/ref-qualifiers/)
- Ref qualifiers restrict member functions based on whether the object is an lvalue or rvalue.
- `&` means “callable on lvalues”.
- `&&` means “callable on rvalues”.
- Often combined with `const` for precise overloads: `const&`, `&`, `&&`.
- Useful for performance and API safety, especially with returned references.

```cpp
class Buffer
{
private:
    std::string m_data{};

public:
    Buffer(std::string data) : m_data{ std::move(data) } {}

    const std::string& data() const & { return m_data; } // stable lvalue object
    std::string data() && { return std::move(m_data); }  // temporary object
};

Buffer buf{ "hello" };
const std::string& a{ buf.data() };          // lvalue overload
std::string b{ Buffer{"tmp"}.data() };       // rvalue overload
```

**Refresher:** ref qualifiers let your API treat temporaries differently from named objects without extra function names.
