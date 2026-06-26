#include <algorithm>
#include <cmath>
#include <iostream>

bool approximatelyEqual(const double &a, const double &b, const double &eps = 1e-12)
{

    return std::abs(a - b) <= eps * std::max(1.0, std::max(std::abs(a), std::abs(b)));
}

int main()
{
    // 6.1, 6.2
    int a{7 / 4};
    double b{7. / 4};

    std::cout << a << " | " << b << std::endl;

    // 6.3
    bool isOdd{(13 % 2) != 0};
    double c{std::pow(2, 10)};
    std::cout << c << std::endl;

    // 6.4
    int x{5};
    int d{++x}; // increases x first, then assigns a. a = 6, x = 6

    // 6.7
    int e{2};
    bool right{e < 10 && e > 1};
    std::cout << right << std::endl;

    double f{0.1 + 0.1 + 0.1};
    bool equalf = (f == 0.3); // might be false due to floating-point
    std::cout << "equalf: " << equalf << std::endl;
    std::cout << "approximatelyEqual: " << approximatelyEqual(f, 0.3) << std::endl;

    // 6.8
    // lhs && rhs: if lhs is false, rhs is skipped
    bool g{true};
    bool h{false};
    bool both{g && h};
    std::cout << both << std::endl;
}
