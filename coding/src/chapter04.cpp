#include <iostream>
#include <cstdint>
namespace math
{
    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }
}

int main()
{
    std::cout << math::max(10, 20) << std::endl;

    // 4.2
    std::cout << "int: " << sizeof(int) << " bytest" << std::endl;
    int a{0};

    // 4.6
    int32_t id{12345};
    std::cout << id << std::endl;

    double avogadro{6.022e23};
    std::cout << avogadro << std::endl;

    // 4.9, 4.10
    bool isValid{3 == 3};
    if (isValid)
    {
        std::cout << isValid << "|" << std::boolalpha << isValid << std::endl;
    }

    // 4.11, 4.12
    char c{'X'};
    std::cout << static_cast<int>(c) << '\n';
}
