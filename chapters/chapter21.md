# Chapter 21: Operator Overloading

## 21.1 Introduction to operator overloading
[Introduction to operator overloading](https://www.learncpp.com/cpp-tutorial/introduction-to-operator-overloading/)
- Operator overloading lets user-defined types act more like built-in types.
- You define how an operator behaves when at least one operand is a class or enum type.
- Good overloads make code natural; bad overloads make code surprising.
- Keep semantics intuitive: `+` should add/compose, `==` should compare equality, `[]` should index.
- Overloading does **not** change:
  - precedence
  - associativity
  - number of operands
  - syntax of the operator
- At least one operand must be a user-defined type.
- You cannot create new operators.
- Common non-overloadable operators include:
  - `.` member access
  - `.*`
  - `::`
  - `?:`
  - `sizeof`
- Prefer simple, unsurprising behavior.
- Prefer non-member overloads when the operation is symmetric.
- Prefer member overloads when the left operand must be modified or for operators that must be members.

```cpp
class Cents {
private:
    int m_cents {};
public:
    explicit Cents(int cents) : m_cents{ cents } {}
    int value() const { return m_cents; }
};
```

- Typical goals:
  - `a + b`
  - `a == b`
  - `std::cout << a`
  - `a[i]`
- Prefer clarity over cleverness.

## 21.2 Overloading arithmetic operators using friend functions
[Overloading the arithmetic operators using friend functions](https://www.learncpp.com/cpp-tutorial/overloading-the-arithmetic-operators-using-friend-functions/)
- Friend operator overloads are non-members with access to private data.
- Useful when both operands should be treated symmetrically.
- Common for binary arithmetic like `+`, `-`, `*`, `/`.
- A friend function is declared inside the class, but it is not a member.
- It does **not** have a `this` pointer.
- Often best when private state is needed and public getters would be noise.
- Return a new object for non-mutating arithmetic operators.
- Prefer implementing `+` in terms of `+=` when possible.

```cpp
class Cents {
private:
    int m_cents {};
public:
    explicit Cents(int cents = 0) : m_cents{ cents } {}

    friend Cents operator+(const Cents& a, const Cents& b) {
        return Cents{ a.m_cents + b.m_cents };
    }

    friend Cents operator-(const Cents& a, const Cents& b) {
        return Cents{ a.m_cents - b.m_cents };
    }
};

Cents sum { Cents{4} + Cents{6} };
```

- Friend overloads are also handy if you want implicit conversion on the left operand.
- Example: `5 + cents` can work if the left side cannot be a member call on `int`.
- Use `const&` parameters for read-only operands.
- Return by value for new results.

## 21.3 Overloading operators using normal functions
[Overloading operators using normal functions](https://www.learncpp.com/cpp-tutorial/overloading-operators-using-normal-functions/)
- A “normal function” overload is a non-member, non-friend function.
- Use this when public accessors provide everything the operator needs.
- This keeps encapsulation tighter than making the function a friend.
- Great for symmetric operators when private access is unnecessary.
- If conversions should work on either side, non-member is often better than member.

```cpp
class Cents {
private:
    int m_cents {};
public:
    explicit Cents(int cents = 0) : m_cents{ cents } {}
    int value() const { return m_cents; }
};

Cents operator+(const Cents& a, const Cents& b) {
    return Cents{ a.value() + b.value() };
}

bool operator==(const Cents& a, const Cents& b) {
    return a.value() == b.value();
}
```

- Prefer this style when:
  - no left operand mutation is needed
  - symmetry matters
  - no private access is needed
- This is usually the cleanest overload form for value-like types.
- If the operator needs internals often, member or friend may be cleaner.

## 21.4 Overloading the I/O operators
[Overloading the I/O operators](https://www.learncpp.com/cpp-tutorial/overloading-the-io-operators/)
- Stream operators are usually overloaded as non-members.
- `operator<<` takes `std::ostream&`.
- `operator>>` takes `std::istream&`.
- Return the stream by reference so calls can chain.
- `std::cout << a << b;`
- `std::cin >> a >> b;`
- Usually make `<<` a friend if it needs private data.
- Input operators should leave the stream in a failed state if parsing fails.

```cpp
#include <iostream>

class Point {
private:
    int m_x {};
    int m_y {};
public:
    Point(int x = 0, int y = 0) : m_x{ x }, m_y{ y } {}

    friend std::ostream& operator<<(std::ostream& out, const Point& p) {
        out << '(' << p.m_x << ", " << p.m_y << ')';
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Point& p) {
        return in >> p.m_x >> p.m_y;
    }
};
```

- `<<` usually takes the object as `const&`.
- `>>` usually takes the object as non-const reference because it modifies it.
- Keep formatted output concise and predictable.

## 21.5 Overloading operators using member functions
[Overloading operators using member functions](https://www.learncpp.com/cpp-tutorial/overloading-operators-using-member-functions/)
- Member operator overloads are functions on the class itself.
- The left operand becomes `*this`.
- Best for operators that naturally modify the object:
  - assignment
  - compound assignment
  - subscript
  - function call
  - arrow-like operators
- Some operators **must** be members:
  - `=`
  - `[]`
  - `()`
  - `->`

```cpp
class Cents {
private:
    int m_cents {};
public:
    explicit Cents(int cents = 0) : m_cents{ cents } {}

    Cents& operator+=(const Cents& other) {
        m_cents += other.m_cents;
        return *this;
    }

    Cents operator+(const Cents& other) const {
        Cents temp{ *this };
        temp += other;
        return temp;
    }
};
```

- Member binary operators take one explicit parameter because the left operand is implicit.
- Use `const` on non-mutating member operators.
- Return `*this` by reference for assignment-like operators.

## 21.6 Overloading unary operators
[Overloading unary operators +, -, and !](https://www.learncpp.com/cpp-tutorial/overloading-unary-operators/)
- Unary operators work on one operand.
- Common overloads:
  - unary `-`
  - unary `+`
  - logical not `!`
  - bitwise not `~`
- Often implemented as members for convenience, but non-members also work.
- Unary operators should preserve intuitive meaning.

```cpp
class Point {
private:
    int m_x {};
    int m_y {};
public:
    Point(int x, int y) : m_x{ x }, m_y{ y } {}

    Point operator-() const {
        return Point{ -m_x, -m_y };
    }

    bool operator!() const {
        return m_x == 0 && m_y == 0;
    }
};
```

- `-obj` should usually return a new negated object.
- `!obj` should answer a truth-like question.
- Avoid strange meanings such as using `!` for “print debug info”.

## 21.7 Overloading the comparison operators
[Overloading the comparison operators](https://www.learncpp.com/cpp-tutorial/overloading-the-comparison-operators/)
- Equality and ordering should reflect the type’s logical value.
- Common operators:
  - `==`
  - `!=`
  - `<`
  - `>`
  - `<=`
  - `>=`
- If you define `==`, also provide `!=` unless it is auto-generated or derived.
- In modern C++, `<=>` can synthesize other comparisons, but `==` is still important.
- Prefer non-member overloads for symmetric comparisons.

```cpp
class Cents {
private:
    int m_cents {};
public:
    explicit Cents(int cents = 0) : m_cents{ cents } {}
    int value() const { return m_cents; }
};

bool operator==(const Cents& a, const Cents& b) {
    return a.value() == b.value();
}

bool operator!=(const Cents& a, const Cents& b) {
    return !(a == b);
}
```

- Keep comparison consistent:
  - reflexive
  - symmetric
  - transitive
- If two objects compare equal, other behavior should usually agree with that.

## 21.8 Overloading the increment/decrement operators
[Overloading the increment and decrement operators](https://www.learncpp.com/cpp-tutorial/overloading-the-increment-and-decrement-operators/)
- Prefix forms:
  - `++x`
  - `--x`
- Postfix forms:
  - `x++`
  - `x--`
- Prefix is usually more efficient because it returns the modified object directly.
- Postfix uses a dummy `int` parameter to distinguish it.

```cpp
class Counter {
private:
    int m_value {};
public:
    explicit Counter(int value = 0) : m_value{ value } {}

    Counter& operator++() {      // prefix
        ++m_value;
        return *this;
    }

    Counter operator++(int) {    // postfix
        Counter old{ *this };
        ++(*this);
        return old;
    }
};
```

- Prefix usually returns `T&`.
- Postfix usually returns `T` by value.
- Same pattern applies to `--`.
- Keep behavior aligned with built-in types.

## 21.9 Overloading the subscript operator
[Overloading the subscript operator](https://www.learncpp.com/cpp-tutorial/overloading-the-subscript-operator/)
- `operator[]` lets objects act like arrays or maps.
- This operator must be a member function.
- Usually provide both:
  - non-const version returning mutable reference
  - const version returning const reference
- Returning a reference allows assignment through the operator.

```cpp
#include <cassert>

class IntList {
private:
    int m_data[10] {};
public:
    int& operator[](int index) {
        assert(index >= 0 && index < 10);
        return m_data[index];
    }

    const int& operator[](int index) const {
        assert(index >= 0 && index < 10);
        return m_data[index];
    }
};
```

- `list[3] = 7;` works because a reference is returned.
- Bounds checking is optional for `operator[]`; if you want checked access, name it separately or assert.
- Use the const overload so read-only objects can still be indexed.

## 21.10 Overloading the parenthesis operator
[Overloading the parenthesis operator](https://www.learncpp.com/cpp-tutorial/overloading-the-parenthesis-operator/)
- `operator()` is the function call operator.
- It lets an object behave like a callable function.
- This operator must be a member function.
- Objects with `operator()` are often called function objects or functors.
- Useful for:
  - stateful callables
  - predicates
  - custom function-like behavior

```cpp
class Accumulator {
private:
    int m_total {};
public:
    int operator()(int value) {
        m_total += value;
        return m_total;
    }
};

Accumulator acc {};
acc(5);   // 5
acc(3);   // 8
```

- `operator()` can take any number of parameters.
- It can be overloaded multiple times with different parameter lists.
- Lambdas are often simpler, but functors are still useful when state and type matter.

## 21.11 Overloading typecasts
[Overloading typecasts](https://www.learncpp.com/cpp-tutorial/overloading-typecasts/)
- Conversion operators define how an object converts to another type.
- Syntax:
  - `operator Type()`
- Prefer `explicit` unless implicit conversion is clearly safe and desirable.
- Implicit conversions can cause subtle bugs.
- `operator bool()` is common for “valid/usable” state.

```cpp
class InputState {
private:
    bool m_good {};
public:
    explicit InputState(bool good) : m_good{ good } {}

    explicit operator bool() const {
        return m_good;
    }
};

InputState state{ true };
if (static_cast<bool>(state)) {
    // okay
}
```

- Prefer named accessors when conversion is not obvious.
- Avoid many implicit conversions on one type.
- A type with too many conversions becomes hard to reason about.

## 21.12 The copy assignment operator
[Overloading the assignment operator](https://www.learncpp.com/cpp-tutorial/overloading-the-assignment-operator/)
- Copy assignment runs when an existing object is assigned from another object.
- Typical signature:
  - `T& operator=(const T&)`
- Default copy assignment does member-wise assignment.
- That is often fine for value types made of safe members.
- If the class manages a resource, you may need a custom overload.
- Always return `*this` by reference.
- Guard against self-assignment if your implementation is not naturally safe.

```cpp
class Buffer {
private:
    int m_size {};
    int* m_data {};
public:
    Buffer& operator=(const Buffer& other) {
        if (this == &other) {
            return *this;
        }

        delete[] m_data;
        m_size = other.m_size;
        m_data = new int[m_size];
        for (int i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
        return *this;
    }
};
```

- If you define copy assignment, consider the Rule of Three/Five/Zero.
- Modern alternative: copy-and-swap or use standard containers/smart pointers.

## 21.13 Shallow vs deep copying
[Shallow vs. deep copying](https://www.learncpp.com/cpp-tutorial/shallow-vs-deep-copying/)
- **Shallow copy** copies member values as-is.
- For raw pointers, that copies the pointer, not the pointed-to data.
- This can cause:
  - shared ownership by accident
  - double deletion
  - dangling pointers
  - unexpected aliasing
- **Deep copy** duplicates the owned resource itself.
- Classes that own dynamic memory usually need deep copy behavior.

```cpp
class Buffer {
private:
    int m_size {};
    int* m_data {};
public:
    Buffer(const Buffer& other)
        : m_size{ other.m_size }
        , m_data{ new int[other.m_size] } {
        for (int i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }
};
```

- Prefer avoiding manual ownership entirely:
  - `std::string`
  - `std::vector`
  - `std::array`
  - `std::unique_ptr`
- Rule of Zero is best: let standard members manage copying safely.

## 21.14 Overloading operators and function templates
[Overloading operators and function templates](https://www.learncpp.com/cpp-tutorial/overloading-operators-and-function-templates/)
- Operator overloads can be function templates.
- Useful when the same operator logic works for many types.
- Common examples:
  - generic stream output
  - comparison for templated classes
  - arithmetic on class templates
- Template operator overloads are often non-members.
- Friend template syntax can be tricky; keep it simple when possible.

```cpp
#include <iostream>

template <typename T>
class Pair {
private:
    T m_first {};
    T m_second {};
public:
    Pair(T first, T second) : m_first{ first }, m_second{ second } {}

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Pair<U>& p);
};

template <typename U>
std::ostream& operator<<(std::ostream& out, const Pair<U>& p) {
    return out << '(' << p.m_first << ", " << p.m_second << ')';
}
```

- If the operator needs private members, use a friend declaration.
- If public accessors are enough, prefer a normal function template.
- Keep template overloads readable; deduction errors can get noisy fast.
