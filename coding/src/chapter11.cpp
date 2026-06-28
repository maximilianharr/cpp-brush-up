#include <iostream>

using std::cout;
using std::endl;

double square(int x) { return x * x; }
double square(double x) { return x * x; }

void printBase(int x, int base = 10) { cout << x % base; }

void inspect(std::string &s) { cout << "mutable" << endl; }
void inspect(const std::string &s) { cout << "const" << endl; }

template <typename T>
T max(T a, T b)
{
    return (a < b) ? b : a;
}

auto square(auto x) // c++20
{
    return x * x;
}

int main()
{
    // 11.1, 11.2
    int a{5};
    double b{5.1};
    cout << "int:" << square(a) << "|" << square(b) << endl;

    // 11.4
    printBase(421);

    // 11.5
    std::string name{"Athena"};
    inspect(name);
    inspect("Zeus");

    // 11.6, 11.7
    auto i{max(2, 3)};
    auto d{max(2.0, 4.0)};

    // 11.8
    square(i);
    square(d);

    // 11.9
}
