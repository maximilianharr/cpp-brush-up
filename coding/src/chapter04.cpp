#include <iostream>
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
    std::cout << "helloo" << std::endl;
}
