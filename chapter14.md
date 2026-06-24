# Chapter 14: Introduction to Classes

Concise refresher for the current LearnCpp Chapter 14 lessons. Quiz content omitted.

## 14.1 Introduction to object-oriented programming

- **OOP** models a program as objects that combine **state** (data) and **behavior** (functions).
- A **class** is a blueprint; an **object** is an instance of that class.
- OOP helps group related operations with the data they operate on.
- Good fits: domain types with identity/invariants (`Date`, `Employee`, `BankAccount`).
- Don’t force everything into classes: plain functions and simple structs are often better for simple data.

```cpp
struct Vec2
{
    double x{};
    double y{};

    double lengthSquared() const
    {
        return x * x + y * y;
    }
};

Vec2 velocity{ 3.0, 4.0 };
```

**Refresher:** think “define a type with rules + operations”, not just “put everything in objects”.

## 14.2 Introduction to classes

- A class can contain:
  - **data members** (state)
  - **member functions** (behavior)
- `class` and `struct` are nearly the same in C++.
- Biggest default difference:
  - `struct` members are **public** by default
  - `class` members are **private** by default
- Each object gets its own copy of non-static data members.

```cpp
class Date
{
    int m_year{};
    int m_month{};
    int m_day{};
};

Date today{};
```

**Refresher:** prefer `class` when you want encapsulation; prefer `struct` for plain data aggregates.

## 14.3 Member functions

- Member functions are functions defined as part of a class.
- They can access the object’s private data directly.
- They implicitly operate on a specific object (`this` arrives later in more detail).
- Functions defined inside the class are usually treated as inline.
- Out-of-class definitions use `ClassName::functionName`.

```cpp
class Counter
{
    int m_value{};

public:
    void add(int amount);
    int get() const { return m_value; }
};

void Counter::add(int amount)
{
    m_value += amount;
}
```

**Refresher:** member functions are the primary way objects expose behavior safely.

## 14.4 Const class objects and const member functions

- A `const` object can only call **const member functions**.
- Add `const` after the function parameter list to promise not to modify object state.
- Read-only operations should usually be marked `const`.
- This improves correctness and lets your type work with const objects/references.

```cpp
class Person
{
    int m_age{};

public:
    void setAge(int age) { m_age = age; }
    int getAge() const { return m_age; }
};

const Person p{};
// p.setAge(42); // error
int age{ p.getAge() };
```

**Refresher:** if a member function conceptually reads only, make it `const`.

## 14.5 Public and private members and access specifiers

- Access specifiers control who can use class members:
  - `public`: part of the interface
  - `private`: implementation details/internal state
- Keep data members **private by default**.
- Let public functions enforce valid usage.
- This separates **what users can do** from **how the class stores it**.

```cpp
class Employee
{
private:
    int m_id{};

public:
    void setId(int id) { m_id = id; }
    int getId() const { return m_id; }
};
```

**Refresher:** public interface small, private details flexible.

## 14.6 Access functions

- **Accessors / getters** expose data in a controlled way.
- **Mutators / setters** change data, often with validation.
- Use access functions when direct member access would break invariants or couple callers to representation.
- Not every member needs a getter/setter; expose only what is useful and safe.

```cpp
class Temperature
{
private:
    double m_celsius{};

public:
    double getCelsius() const { return m_celsius; }

    void setCelsius(double celsius)
    {
        if (celsius >= -273.15)
            m_celsius = celsius;
    }
};
```

**Refresher:** getters are cheap abstraction; setters are where validation usually lives.

## 14.7 Member functions returning references to data members

- Returning by reference avoids copies for expensive members.
- Prefer returning **`const T&`** for read-only access.
- Returning a non-const reference exposes internals and weakens encapsulation.
- The returned reference is valid only as long as the object is alive.
- Be careful with temporaries: references obtained from temporary objects can dangle.

```cpp
class Employee
{
private:
    std::string m_name{};

public:
    const std::string& getName() const { return m_name; }
    std::string& name() { return m_name; } // exposes writable internals
};

Employee e{};
// const std::string& bad{ Employee{}.getName() }; // dangling
```

**Refresher:** return by value for safety, by `const&` for efficiency, by non-const `&` only deliberately.

## 14.8 The benefits of data hiding (encapsulation)

- **Encapsulation** = hide representation, expose a stable interface.
- Benefits:
  - protect invariants
  - centralize validation
  - reduce coupling
  - allow internal changes without breaking callers
- Classes become easier to reason about when only a few functions can modify state.

```cpp
class BankAccount
{
private:
    std::string m_owner{};
    int m_balanceCents{};

public:
    BankAccount(std::string owner, int openingBalanceCents)
        : m_owner{ std::move(owner) }
        , m_balanceCents{ openingBalanceCents >= 0 ? openingBalanceCents : 0 }
    {
    }

    const std::string& owner() const { return m_owner; }
    int balance() const { return m_balanceCents; }

    void deposit(int cents)
    {
        if (cents > 0)
            m_balanceCents += cents;
    }

    bool withdraw(int cents)
    {
        if (cents <= 0 || cents > m_balanceCents)
            return false;

        m_balanceCents -= cents;
        return true;
    }
};
```

**Refresher:** callers can use the account safely without knowing how the balance is stored.

## 14.9 Introduction to constructors

- A constructor initializes an object when it is created.
- Same name as the class, no return type.
- Constructors help guarantee objects start life valid.
- Prefer constructor initialization over “default construct, then call init”.

```cpp
class Fraction
{
private:
    int m_num{};
    int m_den{ 1 };

public:
    Fraction(int num, int den)
        : m_num{ num }, m_den{ den != 0 ? den : 1 }
    {
    }
};

Fraction half{ 1, 2 };
```

**Refresher:** if an object needs setup, put that setup in a constructor.

## 14.10 Constructor member initializer lists

- The member initializer list comes after `:` and before the constructor body.
- Members are initialized **before** the body executes.
- Required for:
  - `const` members
  - reference members
  - members without a default constructor
- Prefer initializing members directly instead of assigning inside the body.
- Actual initialization order follows **member declaration order**, not list order.

```cpp
class User
{
private:
    const int m_id;
    std::string m_name;

public:
    User(int id, std::string name)
        : m_id{ id }
        , m_name{ std::move(name) }
    {
    }
};
```

**Refresher:** initialize first, assign later only when you must.

## 14.11 Default constructors and default arguments

- A **default constructor** can be called with no arguments.
- `T obj{};` uses the default constructor if one exists.
- If you declare any constructor, the compiler usually won’t generate an implicit default constructor for you.
- One constructor with default arguments can often cover multiple construction cases.

```cpp
class Widget
{
private:
    int m_size{};
    bool m_enabled{};

public:
    Widget(int size = 0, bool enabled = true)
        : m_size{ size }, m_enabled{ enabled }
    {
    }
};

Widget a{};          // default construction
Widget b{ 10, false };
```

**Refresher:** default arguments are a simple way to avoid repetitive overloads.

## 14.12 Delegating constructors

- A delegating constructor calls another constructor in the same class.
- Use this to centralize common initialization logic.
- The target constructor runs first; then the delegating constructor body runs.
- This reduces duplicated code and keeps constructors consistent.

```cpp
class Rectangle
{
private:
    int m_width{};
    int m_height{};

public:
    Rectangle() : Rectangle{ 0, 0 } {}
    Rectangle(int side) : Rectangle{ side, side } {}
    Rectangle(int width, int height)
        : m_width{ width }, m_height{ height }
    {
    }
};
```

**Refresher:** pick one “real” constructor and delegate to it from the convenience ones.

## 14.13 Temporary class objects

- A temporary class object is an unnamed object created in an expression.
- Common uses:
  - passing one-off objects to functions
  - returning objects by value
  - explicit construction in-place
- Temporaries typically live until the end of the full expression.
- Binding a temporary to `const T&` extends the temporary’s lifetime to the reference’s lifetime.

```cpp
class Point
{
public:
    Point(int x, int y) : m_x{ x }, m_y{ y } {}

private:
    int m_x{};
    int m_y{};
};

void print(Point p);

print(Point{ 3, 4 });   // temporary object
const std::string& s{ std::string{ "hello" } }; // lifetime extended
```

**Refresher:** temporary object lifetime is short; don’t keep references/pointers into it unless lifetime extension applies.

## 14.14 Introduction to the copy constructor

- A copy constructor initializes a new object from another object of the same type.
- Typical form: `Class(const Class& other)`.
- It is used for:
  - copy initialization
  - pass-by-value
  - return-by-value (unless elided)
- The compiler-generated copy constructor performs **memberwise copy**.
- If your class manages resources manually, you may need a custom copy constructor.

```cpp
class IntPair
{
private:
    int m_x{};
    int m_y{};

public:
    IntPair(int x, int y) : m_x{ x }, m_y{ y } {}
    IntPair(const IntPair& other) : m_x{ other.m_x }, m_y{ other.m_y } {}
};

IntPair a{ 1, 2 };
IntPair b{ a }; // copy constructor
```

**Refresher:** for value-like classes, compiler-generated copy is often enough; prefer Rule of Zero when possible.

## 14.15 Class initialization and copy elision

- Class objects can be:
  - default-initialized
  - direct-initialized
  - copy-initialized
  - list-initialized
- These forms may choose different constructors.
- **Copy elision** lets the compiler skip creating/copying temporaries.
- Since C++17, some elisions are guaranteed.
- Don’t rely on counting copies to reason about behavior or performance.

```cpp
class Widget
{
public:
    Widget(int) {}
};

Widget makeWidget()
{
    return Widget{ 42 }; // copy/move often elided
}

Widget a{ 1 };          // direct initialization
Widget b = Widget{ 2 }; // copy initialization syntax
Widget c{ makeWidget() };
```

**Refresher:** think in terms of object construction semantics; copies may never materialize.

## 14.16 Converting constructors and the explicit keyword

- A constructor that can be called with one argument can define an implicit conversion.
- This can be convenient, but accidental conversions are a common source of bugs.
- `explicit` disables implicit conversion through that constructor.
- Direct initialization still works with `explicit`; copy initialization does not.
- Default stance: mark single-argument constructors `explicit` unless implicit conversion is truly desired.

```cpp
class Distance
{
private:
    double m_meters{};

public:
    explicit Distance(double meters) : m_meters{ meters } {}
    double meters() const { return m_meters; }
};

Distance a{ 3.5 };   // ok
// Distance b = 3.5; // error: constructor is explicit
```

**Refresher:** implicit conversions should feel obviously correct; otherwise use `explicit`.

## 14.17 Constexpr aggregates and classes

- `constexpr` enables objects and functions to participate in compile-time evaluation.
- A small class can be usable at compile time if its constructor/operations are `constexpr`.
- Aggregates are still handy when a type is just bundled public data.
- For lightweight value types, `constexpr` can make code both safer and faster.

```cpp
struct Color
{
    int r{};
    int g{};
    int b{};
};

class Pair
{
private:
    int m_x{};
    int m_y{};

public:
    constexpr Pair(int x, int y) : m_x{ x }, m_y{ y } {}
    constexpr int sum() const { return m_x + m_y; }
};

constexpr Color white{ 255, 255, 255 };
constexpr Pair p{ 2, 3 };
static_assert(p.sum() == 5);
```

**Refresher:** use `constexpr` for simple value types when the result can be known at compile time.
