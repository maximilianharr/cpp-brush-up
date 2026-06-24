# Chapter 25: Virtual Functions

## 25.1 Introduction to virtual functions
[Virtual functions and polymorphism](https://www.learncpp.com/cpp-tutorial/virtual-functions/)

- A **virtual function** enables runtime dispatch through a base **pointer or reference**.
- Mark the function `virtual` in the base class; overrides stay virtual even if `virtual` is omitted later.
- Without `virtual`, the call is chosen from the **static type** (`Base*` / `Base&`).
- With `virtual`, the call is chosen from the **dynamic type** (the actual object).
- This is the foundation of **runtime polymorphism**.
- Prefer polymorphic use through references/pointers, not by-value parameters.
- **Early binding** = compile-time choice. **Late binding** = runtime choice.
- Under the hood, compilers usually use a **vptr** + **vtable** to implement dispatch.

```cpp
#include <iostream>
#include <string_view>

struct Animal {
    virtual std::string_view speak() const { return "???"; }
};

struct Cat : Animal {
    std::string_view speak() const override { return "meow"; }
};

void print(const Animal& a) {
    std::cout << a.speak() << '\n'; // late binding
}

int main() {
    Cat cat{};
    print(cat); // meow
}
```

Quick refresher:

- `virtual` is for behavior that may vary in derived types.
- Constructors cannot be virtual.
- Non-virtual functions are fine when behavior should not vary.
- Avoid passing polymorphic objects by value: that risks **object slicing**.

## 25.2 Virtual functions and polymorphism

- **Polymorphism** = “one interface, many forms”.
- A base interface can represent many derived implementations.
- Store heterogeneous objects behind base pointers/references:
  - `Base&`
  - `Base*`
  - `std::unique_ptr<Base>`
- Containers of `Base` by value slice derived state and behavior.
- Virtual dispatch is most useful when client code should not care which derived type it has.
- A common pattern is a non-virtual public API that forwards to virtual customization points.
- Another common pattern is a virtual `print()` used by `operator<<`.

```cpp
#include <iostream>
#include <memory>
#include <vector>

struct Shape {
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

struct Circle : Shape {
    double r{};
    explicit Circle(double radius) : r{radius} {}
    double area() const override { return 3.14159 * r * r; }
};

struct Square : Shape {
    double s{};
    explicit Square(double side) : s{side} {}
    double area() const override { return s * s; }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes{};
    shapes.push_back(std::make_unique<Circle>(2.0));
    shapes.push_back(std::make_unique<Square>(3.0));

    for (const auto& shape : shapes)
        std::cout << shape->area() << '\n';
}
```

Related refresher notes:

- **Object slicing** happens when a derived object is copied into a base object by value.
- Use references/pointers to preserve polymorphism.
- The implementation mechanism is typically a per-class **vtable** and per-object **vptr**.

## 25.3 The override and final specifiers, and covariant return types
[The override and final specifiers, and covariant return types](https://www.learncpp.com/cpp-tutorial/the-override-and-final-specifiers-and-covariant-return-types/)

- `override` says: “this must override a base virtual function”.
- If the signature does not match, the compiler errors out.
- Always prefer `override` on overrides.
- `final` on a virtual function prevents further overriding.
- `final` on a class prevents inheritance from that class.
- **Covariant return types** allow an override to return a more-derived pointer/reference type.
- Covariance works only for pointers/references to related class types, not values.

```cpp
struct Base {
    virtual Base* clone() const { return new Base(*this); }
    virtual void f() {}
};

struct Derived final : Base {
    Derived* clone() const override { return new Derived(*this); } // covariant
    void f() override final {}
};
```

Why this matters:

- `override` catches subtle bugs such as:
  - wrong parameter type
  - missing `const`
  - wrong ref-qualifier / cv-qualifier
- `final` documents design intent.
- Covariant returns are handy for `clone()`-style interfaces.

Bad vs good:

```cpp
struct B { virtual void g() const {} };

struct D1 : B {
    void g() {}              // NOT an override: missing const
};

struct D2 : B {
    void g() const override {} // correct
};
```

## 25.4 Virtual destructors, virtual assignment, and overriding virtualization
[Virtual destructors, virtual assignment, and overriding virtualization](https://www.learncpp.com/cpp-tutorial/virtual-destructors-virtual-assignment-and-overriding-virtualization/)

- If a class is meant to be used polymorphically, give it a **virtual destructor**.
- Deleting a derived object through a base pointer without a virtual destructor is **undefined behavior**.
- Rule of thumb: if a class has any virtual functions, its destructor should usually be virtual too.
- Once a function is virtual in a base, it stays virtual in derived classes even if `virtual` is omitted.
- Assignment operators are usually **not** where polymorphism shines; prefer references/pointers plus virtual behavior.
- Virtual calls made from constructors/destructors dispatch to the class currently under construction/destruction, not to more-derived overrides.

```cpp
#include <iostream>

struct Base {
    virtual ~Base() { std::cout << "Base dtor\n"; }
};

struct Derived : Base {
    ~Derived() override { std::cout << "Derived dtor\n"; }
};

int main() {
    Base* p{new Derived{}};
    delete p; // Derived dtor, then Base dtor
}
```

Constructor/destructor pitfall:

```cpp
struct A {
    A() { print(); }                 // calls A::print(), not B::print()
    virtual void print() const {}
};

struct B : A {
    void print() const override {}
};
```

Related note: **virtual base classes** are different from virtual functions.

- Use virtual base classes to solve the **diamond inheritance** duplicate-base problem.

```cpp
struct PoweredDevice { int watts{}; };
struct Scanner : virtual PoweredDevice {};
struct Printer : virtual PoweredDevice {};
struct Copier : Scanner, Printer {}; // one PoweredDevice subobject
```

## 25.5 Pure virtual functions, abstract base classes, and interface classes
[Pure virtual functions, abstract base classes, and interface classes](https://www.learncpp.com/cpp-tutorial/pure-virtual-functions-abstract-base-classes-and-interface-classes/)

- A **pure virtual function** uses `= 0`.
- A class with at least one pure virtual function is an **abstract base class**.
- Abstract classes cannot be instantiated.
- Derived classes must override all inherited pure virtuals or remain abstract.
- An **interface** is an abstract class used purely as a contract.
- In modern C++, interfaces typically have:
  - public pure virtual functions
  - a virtual destructor
  - little or no data
- This is often where plugin-style, strategy, and polymorphic printing APIs begin.

```cpp
#include <iostream>
#include <ostream>

struct Printable {
    virtual void print(std::ostream& out) const = 0;
    virtual ~Printable() = default;
};

inline std::ostream& operator<<(std::ostream& out, const Printable& p) {
    p.print(out);
    return out;
}

struct Employee : Printable {
    void print(std::ostream& out) const override { out << "Employee"; }
};
```

Related refresher topics:

- **dynamic_cast** safely downcasts within a polymorphic hierarchy.
  - The base type must be **polymorphic** (have at least one virtual function).
  - Pointer form returns `nullptr` on failure.
  - Reference form throws `std::bad_cast` on failure.
- Use it sparingly; needing many downcasts often suggests a design smell.

```cpp
struct Shape { virtual ~Shape() = default; };
struct Circle : Shape { double r{2.0}; };

Shape* shape{new Circle{}};

if (auto* circle = dynamic_cast<Circle*>(shape)) {
    std::cout << circle->r << '\n';
}

delete shape;
```

- **Object slicing** still matters with abstract hierarchies: avoid by-value base parameters.
- **Printing inherited classes using `operator<<`** is usually done with a virtual `print(std::ostream&) const`.

Mental checklist:

- Need runtime dispatch? -> virtual function.
- Writing an override? -> add `override`.
- Base deleted via base pointer? -> virtual destructor.
- Want “must implement”? -> pure virtual `= 0`.
- Passing/storing polymorphic objects? -> use refs/pointers, not values.
