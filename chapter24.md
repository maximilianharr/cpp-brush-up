# Chapter 24: Inheritance

## 24.1 Introduction to inheritance
- Inheritance lets one class reuse data/functions from another.
- **Base class** = existing class. **Derived class** = class built on top of it.
- Best fit for an **is-a** relationship:
  - `Dog` is an `Animal`
  - `Manager` is an `Employee`
- A derived object contains a **base-class subobject** plus its own members.
- Main benefits:
  - less duplicated code
  - shared interface/behavior
  - clearer modeling of related types
- Favor inheritance when the relationship is truly hierarchical; otherwise prefer composition.

```cpp
class Animal
{
public:
    void eat() const { std::cout << "eating\n"; }
};

class Dog : public Animal
{
public:
    void bark() const { std::cout << "woof\n"; }
};

Dog d{};
d.eat();   // inherited from Animal
d.bark();  // Dog-specific
```

## 24.2 Basic inheritance in C++
- Inherit with `class Derived : public Base`.
- `public` inheritance usually means “derived is a kind of base”.
- Public base members stay public in the derived class.
- Protected base members are accessible inside derived members.
- Private base members exist in the object, but derived code cannot access them directly.
- A derived object can use inherited functions as if they were its own.
- Constructors are **not inherited automatically** in the basic sense; the base part still needs construction.

```cpp
class Animal
{
public:
    void speak() const { std::cout << "some sound\n"; }

protected:
    int m_age{};

private:
    int m_id{};
};

class Dog : public Animal
{
public:
    void setAge(int age) { m_age = age; }   // ok: protected
    void bark() const { std::cout << "woof\n"; }
    // void setId(int id) { m_id = id; }    // error: private in base
};
```

## 24.3 Order of construction of derived classes, base classes, and class members
- Construction order is fixed:
  1. base classes
  2. member objects
  3. derived-class constructor body
- Member objects are initialized in **declaration order**, not initializer-list order.
- Destruction happens in the reverse order.
- With multiple inheritance, base classes are built left-to-right as listed in the inheritance list.
- This matters when members depend on earlier initialization.

```cpp
struct Base
{
    Base() { std::cout << "Base\n"; }
    ~Base() { std::cout << "~Base\n"; }
};

struct Member
{
    Member(const char* name) { std::cout << name << '\n'; }
    ~Member() { std::cout << "~Member\n"; }
};

struct Derived : Base
{
    Member m_first{"first"};
    Member m_second{"second"};

    Derived() { std::cout << "Derived\n"; }
    ~Derived() { std::cout << "~Derived\n"; }
};

// Construction: Base, first, second, Derived
// Destruction:  ~Derived, ~Member, ~Member, ~Base
```

## 24.4 Constructors and initialization of derived classes
- The derived constructor initializes the base part in its **member initializer list**.
- If the base has no default constructor, the derived constructor **must** call a base constructor explicitly.
- Initialize base data through the base constructor, not by assigning later in the derived body.
- Base constructors run before derived members/body.
- Write initializer lists in logical order, but remember real initialization order still follows declaration/base order.

```cpp
class Employee
{
private:
    std::string m_name{};
    int m_id{};

public:
    Employee(std::string name, int id)
        : m_name{std::move(name)}, m_id{id}
    {
    }
};

class Manager : public Employee
{
private:
    int m_reports{};

public:
    Manager(std::string name, int id, int reports)
        : Employee{std::move(name), id}, // initialize base first
          m_reports{reports}
    {
    }
};
```

## 24.5 Inheritance and access specifiers
- Access in the **base** and access used for **inheritance** are different ideas.
- Base member access:
  - `public`: accessible everywhere allowed by the object
  - `protected`: accessible in the class and derived classes
  - `private`: accessible only inside the base class
- Inheritance mode transforms inherited access:
  - `public` inheritance: base `public -> public`, `protected -> protected`
  - `protected` inheritance: base `public/protected -> protected`
  - `private` inheritance: base `public/protected -> private`
- Base `private` members never become directly accessible in the derived class.
- For `class`, omitted inheritance access defaults to `private`; for `struct`, it defaults to `public`.
- Public inheritance is the normal choice for is-a modeling.

```cpp
class Base
{
public:
    int pub{1};

protected:
    int prot{2};
};

class PublicDerived : public Base
{
public:
    int read() const { return prot; } // ok
};

class PrivateDerived : private Base
{
public:
    int getPub() const { return pub; } // ok inside the class
};

PublicDerived a{};
// a.pub;   // ok

PrivateDerived b{};
// b.pub;   // error: became private through private inheritance
```

## 24.6 Calling base class functions from derived classes
- Inherited functions can be called directly from a derived object.
- If the derived class defines a function with the same name, use `Base::function()` to explicitly call the base version.
- Qualified calls are useful to extend behavior instead of replacing it completely.
- This also avoids accidental recursion.

```cpp
class Base
{
public:
    void identify() const
    {
        std::cout << "Base\n";
    }
};

class Derived : public Base
{
public:
    void identify() const
    {
        Base::identify();           // call inherited version
        std::cout << "Derived\n";
    }
};

Derived d{};
d.identify();
// output:
// Base
// Derived
```

## 24.7 Overriding behaviors in derived classes
- A derived class can provide its own version of an inherited function.
- If the name/signature/qualifiers match, the derived version replaces the base behavior for derived objects.
- If the signature does **not** match, you may get hiding or overloading instead of a true override.
- The base version still exists and can be called with `Base::function()`.
- Runtime polymorphism requires `virtual` (next chapter), but the basic override pattern starts here.

```cpp
class Animal
{
public:
    void speak() const { std::cout << "animal sound\n"; }
};

class Cat : public Animal
{
public:
    void speak() const { std::cout << "meow\n"; } // overrides for Cat objects
};

Animal a{};
Cat c{};

a.speak();        // animal sound
c.speak();        // meow
c.Animal::speak(); // animal sound
```

## 24.8 Hiding inherited members
- A derived declaration with the same name can hide base members of that name.
- For functions, one derived overload can hide the entire base overload set.
- For data members, the derived member hides the base member with the same name.
- Use:
  - `Base::member` to access the hidden base member
  - `using Base::member;` to bring base overloads back into scope
- Hiding is often surprising; keep interfaces clear.

```cpp
class Base
{
public:
    void print(int) const { std::cout << "int\n"; }
    void print(double) const { std::cout << "double\n"; }
    int value{1};
};

class Derived : public Base
{
public:
    using Base::print; // restore base overloads

    void print(std::string_view) const { std::cout << "text\n"; }
    int value{2}; // hides Base::value
};

Derived d{};
d.print(42);        // Base::print(int)
d.print("hello");   // Derived::print(string_view)
std::cout << d.value << '\n';        // 2
std::cout << d.Base::value << '\n';  // 1
```

## 24.9 Multiple inheritance
- A class may inherit from more than one base class.
- Useful when a type naturally combines multiple distinct roles/interfaces.
- Construction order follows the base-class list left-to-right; destruction reverses it.
- Main risk: **ambiguity** when multiple bases provide the same member.
- The **diamond problem** happens when two bases share a common base, causing duplicated base subobjects.
- **Virtual inheritance** can collapse the diamond to one shared base subobject.
- Multiple inheritance is powerful but easier to misuse; composition is often simpler.

```cpp
struct A
{
    void print() const { std::cout << "A\n"; }
};

struct B : A {};
struct C : A {};

struct D : B, C {};

// D d{};
// d.print();    // error: ambiguous (B::A or C::A?)
// d.B::print(); // ok
```

```cpp
struct Powered
{
    int hp{100};
};

struct Car  : virtual Powered {};
struct Boat : virtual Powered {};
struct AmphibiousVehicle : Car, Boat {};

AmphibiousVehicle av{};
av.hp = 250; // one shared Powered subobject because of virtual inheritance
```
