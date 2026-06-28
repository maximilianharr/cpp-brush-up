#include <iostream>
#include <string>
#include <string_view>
#include <cstdint>
#include <type_traits>

using std::cout;
using std::endl;

enum class Color : std::uint8_t
{
    red = 11,
    green = 13,
    blue
};

struct Point
{
    int x{};
    int y{};
};

constexpr std::string_view
toString(Color color)
{
    switch (color)
    {
    case Color::red:
        return "red";
    case Color::green:
        return "green";
    case Color::blue:
        return "blue";
    };
    return "unknown";
}

struct Employee
{
    int id{};
    std::string name{"Joe"};
};

std::ostream &
operator<<(std::ostream &out, Color &color)
{
    switch (color)
    {
    case Color::red:
        return out << "red";
    case Color::blue:
        return out << "blue";
    case Color::green:
        return out << "green";
    }
    return out << "unknown";
}

template <typename T>
struct Customer
{
    T wallet{};
    T credit{};
};

int main()
{
    // 13.1, 13.2, 13.3
    Color mc{Color::red};
    Point p{10, 20};

    // 13.4
    cout << toString(mc) << endl;

    // 13.5
    cout << mc << endl;

    // 13.7, 13.9
    Employee employee{12, "John Doe"};
    employee.id = 14;

    // 13.12
    Employee &ref{employee};
    Employee *ptr{&employee};

    cout << ref.name << ptr->name << endl;

    // 13.13
    Customer<double> customer{10.0, 12.0};

    // 13.14
}
