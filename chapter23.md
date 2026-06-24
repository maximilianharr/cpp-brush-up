# Chapter 23: Object Relationships

## 23.1 Object relationships

- Objects can relate to other objects in several common ways:
  - **Composition**: `whole` owns `part` objects.
  - **Aggregation**: `whole` refers to `part` objects it does **not** own.
  - **Association**: objects know about each other and can interact.
  - **Dependency**: one object/function uses another briefly.
- The main questions:
  - **Who owns what?**
  - **Who controls lifetime?**
  - **How long does the relationship last?**
- Stronger relationships usually mean tighter coupling.
- Prefer the weakest relationship that models the problem correctly.
- Rough strength order:
  - composition > aggregation/association > dependency

```cpp
class Engine {};
class Car
{
    Engine m_engine;          // composition: Car owns Engine
};

class Teacher {};
class Department
{
    Teacher* m_head{};        // aggregation/association candidate
};

void printTeacher(const Teacher& t) // dependency: temporary use
{
    (void)t;
}
```

- Good modeling matters:
  - ownership bugs become lifetime bugs
  - weak relationships reduce coupling
  - correct choice improves design clarity

## 23.2 Composition

- **Composition** is a **part-of** relationship where the whole object:
  - directly owns its parts
  - is responsible for creating/acquiring them
  - destroys them when it dies
- Parts usually do not outlive the whole.
- A part belongs to at most one whole at a time.
- In C++, composition is commonly modeled with **member objects**.
- Composition is ideal when the part is essential to the whole.
- Example ideas:
  - `House` has `Rooms`
  - `Car` has `Engine`
  - `Body` has `Heart`

```cpp
class Engine
{
public:
    void start() const {}
};

class Car
{
private:
    Engine m_engine; // owned directly

public:
    void start()
    {
        m_engine.start();
    }
};
```

- Important implications:
  - member subobjects are constructed before the constructor body
  - they are destroyed automatically in reverse declaration order
  - copying/moving the whole also copies/moves members as defined
- Composition often gives the safest lifetime management because ownership is explicit.

## 23.3 Aggregation

- **Aggregation** is also a **has-a** relationship, but the whole does **not own** the parts.
- The aggregated object can exist independently of the aggregator.
- Multiple aggregators may refer to the same object.
- Lifetime is managed elsewhere.
- In C++, aggregation is often modeled with:
  - pointers
  - references
  - smart pointers when ownership is external/shared by design
- Example ideas:
  - `Department` has `Teacher`s, but teachers exist separately
  - `Team` has `Player`s owned by a league/database

```cpp
class Teacher
{
public:
    explicit Teacher(std::string_view name) : m_name{name} {}
    std::string_view getName() const { return m_name; }

private:
    std::string m_name;
};

class Department
{
private:
    const Teacher& m_teacher; // refers to external object

public:
    explicit Department(const Teacher& teacher) : m_teacher{teacher} {}
};
```

- Main risk: dangling references/pointers if the external object dies first.
- Use aggregation only when independent lifetime is intentional.
- If the whole must guarantee the part exists for its entire life, composition is often a better fit.

## 23.4 Association

- **Association** is a relationship where otherwise unrelated objects know about each other and interact.
- Unlike aggregation, association is broader:
  - it may be **unidirectional** (`A` knows `B`)
  - or **bidirectional** (`A` knows `B`, `B` knows `A`)
- Neither side necessarily owns the other.
- Associations can be long-lived compared to dependencies.
- Example ideas:
  - `Doctor` and `Patient`
  - `Player` and `Game`
  - `User` and `Organization`

```cpp
class Patient;

class Doctor
{
public:
    void assignPatient(Patient& patient) { m_patient = &patient; }

private:
    Patient* m_patient{};
};

class Patient
{
public:
    void assignDoctor(Doctor& doctor) { m_doctor = &doctor; }

private:
    Doctor* m_doctor{};
};
```

- Associations are often implemented with pointers, references, IDs, or lookup keys.
- Prefer simple unidirectional associations unless two-way knowledge is genuinely needed.
- Bidirectional associations increase coupling and make invariants harder to maintain.

## 23.5 Dependencies

- A **dependency** means one object/function uses another object **temporarily**.
- This is typically the weakest relationship.
- Common forms:
  - function parameters
  - local variables
  - temporary helper calls
- The dependent object does not usually store the other object.
- If the supplier’s interface changes, dependents may need to change too.

```cpp
class Printer
{
public:
    void print(std::string_view text) const {}
};

class Report
{
public:
    void output(const Printer& printer) const // dependency only
    {
        printer.print("Quarterly report");
    }
};
```

- Dependencies are usually desirable when persistent ownership/links are unnecessary.
- Prefer dependency over stronger relationships when all you need is a short-lived use.
- This keeps classes less coupled and easier to test, replace, and reuse.

## 23.6 Container classes

- A **container class** stores and organizes multiple objects.
- Standard examples:
  - `std::vector`
  - `std::array`
  - `std::list`
  - `std::map`
- Containers often manage:
  - storage
  - insertion/removal
  - iteration/access
  - size/capacity rules
- Relationship depends on what the container stores:
  - storing values -> usually **composition**
  - storing pointers/references -> often **aggregation/association**
- Value containers own their elements.
- Reference/pointer containers usually do not own the pointed-to objects unless explicit ownership wrappers are used.

```cpp
#include <vector>

std::vector<int> values{1, 2, 3}; // owns ints

int a{1};
int b{2};
std::vector<int*> ptrs{&a, &b};   // does not own a or b
```

- Design question: should your container own elements or just refer to them?
- That choice determines lifetime rules, copy semantics, and cleanup responsibilities.
- Prefer standard containers unless you specifically need custom behavior.

## 23.7 std::initializer_list

- `std::initializer_list<T>` lets objects/functions accept **brace-initialized lists**.
- It is commonly used to make container-like types convenient to construct.
- The compiler creates a temporary array-like backing store.
- The initializer list provides read-only access to that data.
- Elements are treated as `const`, so you can read them but not modify them in place.
- Typical use: constructor overloads for list initialization.

```cpp
#include <initializer_list>
#include <vector>

class IntList
{
public:
    IntList(std::initializer_list<int> init)
        : m_values{init}
    {
    }

private:
    std::vector<int> m_values;
};

IntList primes{2, 3, 5, 7};
```

- Important caveats:
  - do **not** store the `std::initializer_list` object itself for later use
  - copy the elements you need into owned storage
  - beware overload interactions with brace initialization
- Great for ergonomic APIs, especially for small container-like classes and configuration objects.
