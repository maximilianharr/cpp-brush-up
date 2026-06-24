# Chapter 13: Compound Types: Enums and Structs

## 13.1 Introduction to program-defined (user-defined) types
[Introduction to program-defined (user-defined) types](https://www.learncpp.com/cpp-tutorial/introduction-to-program-defined-user-defined-types/)
- C++ has built-in types (`int`, `double`, `char`) and program-defined types you create.
- Chapter 13 centers on two simple custom types:
  - **enumerations**: a fixed set of named values
  - **structs**: a group of related members under one type
- Use program-defined types to improve readability and model intent directly.
- Good types reduce “what does this integer mean?” style bugs.
- Prefer named types when raw primitives stop expressing meaning clearly.

```cpp
enum class Color { red, green, blue };

struct Point
{
    int x {};
    int y {};
};

Color paint { Color::red };
Point playerPos { 10, 20 };
```

## 13.2 Unscoped enumerations
[Unscoped enumerations](https://www.learncpp.com/cpp-tutorial/unscoped-enumerations/)
- Unscoped enums use `enum`.
- Enumerators are named integral constants.
- If you do not assign values, they start at `0` and increment by `1`.
- Unscoped enumerator names enter the surrounding scope.
- They implicitly convert to integers, which is convenient but error-prone.

```cpp
enum Color
{
    red,
    green,
    blue
};

Color shirt { green };
int value { shirt }; // implicit conversion: green -> 1
```

- Main downside: name pollution and weak type safety.

## 13.3 Unscoped enumerator integral conversions
[Unscoped enumerator integral conversions](https://www.learncpp.com/cpp-tutorial/unscoped-enumerator-integral-conversions/)
- Each enumerator has an underlying integral value.
- You can assign explicit values, including gaps.
- Unscoped enums convert implicitly to integral types.
- Converting integers back to an enum requires an explicit cast.
- Not every integer is a meaningful enumerator value, so validate before casting.

```cpp
enum Pet
{
    cat = 1,
    dog = 2,
    pig = 4
};

Pet pet { dog };
int bits { pet }; // okay: Pet -> int

pet = static_cast<Pet>(4); // okay syntactically
// pet = static_cast<Pet>(7); // also allowed, but maybe semantically invalid
```

- Treat enum-to-int as a serialization/detail boundary, not everyday logic.

## 13.4 Converting an enumeration to and from a string
[Converting an enumeration to and from a string](https://www.learncpp.com/cpp-tutorial/converting-an-enumeration-to-and-from-a-string/)
- C++ does not automatically stringify enums.
- The common approach is a `switch` for enum-to-string.
- For string-to-enum, parse known inputs and reject invalid ones.
- Keeping conversion code centralized avoids scattered magic strings.

```cpp
#include <optional>
#include <string_view>

enum class Color { red, green, blue };

constexpr std::string_view toString(Color color)
{
    switch (color)
    {
    case Color::red:   return "red";
    case Color::green: return "green";
    case Color::blue:  return "blue";
    }

    return "unknown";
}

std::optional<Color> parseColor(std::string_view text)
{
    if (text == "red")   return Color::red;
    if (text == "green") return Color::green;
    if (text == "blue")  return Color::blue;
    return std::nullopt;
}
```

## 13.5 Introduction to overloading the I/O operators
[Introduction to overloading the I/O operators](https://www.learncpp.com/cpp-tutorial/introduction-to-overloading-the-i-o-operators/)
- Overload `<<` to print your type naturally.
- Overload `>>` to read your type naturally.
- Return the stream by reference so chaining works.
- For input, parse into a temporary value first, then assign on success.
- On bad input, set `failbit`.

```cpp
#include <iostream>
#include <string>

enum class Color { red, green, blue };

std::ostream& operator<<(std::ostream& out, Color color)
{
    switch (color)
    {
    case Color::red:   return out << "red";
    case Color::green: return out << "green";
    case Color::blue:  return out << "blue";
    }

    return out << "unknown";
}

std::istream& operator>>(std::istream& in, Color& color)
{
    std::string text {};
    in >> text;

    if (text == "red")       color = Color::red;
    else if (text == "green") color = Color::green;
    else if (text == "blue")  color = Color::blue;
    else                      in.setstate(std::ios::failbit);

    return in;
}
```

## 13.6 Scoped enumerations (enum classes)
[Scoped enumerations (enum classes)](https://www.learncpp.com/cpp-tutorial/scoped-enumerations-enum-classes/)
- Scoped enums use `enum class`.
- Enumerators stay inside the enum’s scope (`Color::red`).
- They do **not** implicitly convert to integers.
- This gives much better type safety than plain `enum`.
- Prefer `enum class` unless you specifically need old unscoped behavior.

```cpp
#include <cstdint>

enum class Color : std::uint8_t
{
    red,
    green,
    blue
};

Color paint { Color::green };
// int n { paint }; // error: no implicit conversion
int n { static_cast<int>(paint) }; // explicit and obvious
```

- Big refresher point: **`enum class` avoids accidental mixing with ints**.

## 13.7 Introduction to structs, members, and member selection
[Introduction to structs, members, and member selection](https://www.learncpp.com/cpp-tutorial/introduction-to-structs-members-and-member-selection/)
- A `struct` groups related values into one named type.
- Struct members are `public` by default.
- Use the member access operator `.` to select a member.
- Structs are great for simple data objects.
- Compared to passing separate variables around, they improve cohesion.

```cpp
#include <string>

struct Employee
{
    int id {};
    std::string name {};
    double wage {};
};

Employee joe { 1, "Joe", 32.5 };
joe.wage = 35.0;
```

- `struct` vs `class`: same core capability; default access differs (`public` vs `private`).

## 13.8 Struct aggregate initialization
[Struct aggregate initialization](https://www.learncpp.com/cpp-tutorial/struct-aggregate-initialization/)
- Many structs are **aggregates**, meaning they can be brace-initialized member-by-member.
- Members initialize in **declaration order**, not visual order in the brace list.
- Omitted members are value-initialized or use their default member initializers.
- Brace initialization helps prevent narrowing conversions.

```cpp
struct Point
{
    int x {};
    int y {};
};

struct Rectangle
{
    Point topLeft {};
    Point bottomRight {};
};

Point p1 { 3, 4 };
Point p2 { 5 };              // y becomes 0
Rectangle r { {0, 0}, {640, 480} };
```

- Keep the member order stable; callers depend on it during aggregate initialization.

## 13.9 Default member initialization
[Default member initialization](https://www.learncpp.com/cpp-tutorial/default-member-initialization/)
- Members can be given defaults directly inside the struct.
- Those defaults are used when a member is omitted during initialization.
- Explicit initializer values override defaults.
- This is a simple way to make “reasonable default state” easy.

```cpp
struct Settings
{
    int width { 800 };
    int height { 600 };
    bool fullscreen { false };
};

Settings a {};                  // 800, 600, false
Settings b { 1024, 768, true }; // overrides all defaults
Settings c { 1280 };            // 1280, 600, false
```

- Prefer default member initializers over repeating the same setup everywhere.

## 13.10 Passing and returning structs
[Passing and returning structs](https://www.learncpp.com/cpp-tutorial/passing-and-returning-structs/)
- Pass small structs by value when copying is cheap and intended.
- Pass larger structs by `const&` when you only need to read them.
- Pass by non-const reference when you need to modify the caller’s object.
- Returning structs by value is normal and efficient in modern C++.

```cpp
#include <string>

struct Employee
{
    int id {};
    std::string name {};
};

void print(const Employee& e)
{
    // read-only, avoids copying
}

void rename(Employee& e, const std::string& newName)
{
    e.name = newName;
}

Employee makeEmployee(int id, std::string name)
{
    return { id, name };
}
```

- Refresher rule: default to `const&` for nontrivial read-only structs.

## 13.11 Struct miscellany
[Struct miscellany](https://www.learncpp.com/cpp-tutorial/struct-miscellany/)
- Structs copy and assign memberwise by default.
- That default behavior is often exactly what you want for plain data types.
- A struct can contain other structs, arrays, containers, and enums.
- If a type starts needing invariants and tightly controlled state, consider a `class`.
- Keep structs focused: related data together, not “everything object”.

```cpp
struct Pair
{
    int first {};
    int second {};
};

Pair a { 1, 2 };
Pair b { a };  // memberwise copy

b.second = 42; // changes only b
```

- Plain structs are strongest when they stay simple and unsurprising.

## 13.12 Member selection with pointers and references
[Member selection with pointers and references](https://www.learncpp.com/cpp-tutorial/member-selection-with-pointers-and-references/)
- References use the normal `.` operator.
- Pointers use `->`, which is shorthand for `(*ptr).member`.
- This comes up constantly when passing structs by reference/pointer or using dynamic allocation.

```cpp
#include <string>

struct Employee
{
    int id {};
    std::string name {};
};

Employee joe { 1, "Joe" };

Employee& ref { joe };
Employee* ptr { &joe };

ref.name = "Joseph";  // reference uses .
ptr->id = 2;          // pointer uses ->
(*ptr).name = "Joe";  // equivalent to ptr->name = "Joe"
```

- Quick memory aid: **reference = object alias**, **pointer = address**.

## 13.13 Class templates
[Class templates](https://www.learncpp.com/cpp-tutorial/class-templates/)
- A class template lets one struct/class work with many element types.
- Template parameters stand in for real types until instantiation.
- Each unique template argument list creates a distinct type.
- Great for reusable containers, pairs, wrappers, and small utility types.

```cpp
#include <string>

template <typename T>
struct Pair
{
    T first {};
    T second {};
};

Pair<int> scores { 10, 20 };
Pair<std::string> names { "Ada", "Bjarne" };
```

- Think: “functions can be templated; types can be templated too.”

## 13.14 Class template argument deduction (CTAD) and deduction guides
[Class template argument deduction (CTAD) and deduction guides](https://www.learncpp.com/cpp-tutorial/class-template-argument-deduction-ctad-and-deduction-guides/)
- Since C++17, the compiler can often deduce class template arguments from initialization.
- CTAD removes some repetitive `<T>` spelling.
- When deduction does not do what you want, add a **deduction guide**.
- Deduction guides let you steer template argument selection explicitly.

```cpp
#include <type_traits>

template <typename T>
struct Pair
{
    T first {};
    T second {};

    Pair(T a, T b) : first{a}, second{b} {}
};

template <typename T, typename U>
Pair(T, U) -> Pair<std::common_type_t<T, U>>;

Pair p1 { 1, 2 };     // Pair<int>
Pair p2 { 1, 2.5 };   // Pair<double> via deduction guide
```

- CTAD is convenience, not magic: if the deduced type is unclear, spell it out.
