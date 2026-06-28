#include <iostream>

using std::cout;
using std::endl;

template <typename T>
struct Vector
{
    T x{};
    T y{};

    T lengthSquated()
    {
        return x * x + y * y;
    }
};

class Counter
{
private:
    int m_value{0};
    std::string m_name{"default"};

public:
    // constructors
    Counter() : Counter{0, "default"} {};
    Counter(int value, std::string name)
        : m_value{value}, m_name{std::move(name)} {
          };
    // copy constructor
    Counter(Counter &c)
    {
        m_value = c.m_value;
        m_name = c.m_name;
    }

    // getters
    int value() const { return m_value; };
    std::string &name() { return m_name; };
    void add(int amount);

    void setValue(int value) { m_value = value; };
};

void Counter::add(int amount)
{
    m_value += amount;
}

int main()
{
    // 14.1
    Vector<double> vector(10., 20.);
    cout << vector.lengthSquated() << endl;

    // 14.2, 14.3
    Counter c_xyz{};
    c_xyz.add(100);
    cout << c_xyz.value() << endl;

    // 14.4, 14.5, 14.6
    c_xyz.setValue(124);
    const Counter c_const{c_xyz};
    // c_const.add(100); // const obj can only call const functions
    cout << c_const.value() << endl;

    // 14.7
    cout << c_xyz.name() << endl;

    // 14.8, 14.9, 14.11, 14.12
    Counter c_a(10, "rob");
    Counter c_b{};

    // 14.14
    Counter c_c{c_b};

    // 14.15
}
