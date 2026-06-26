#include <iostream>
#include <string>
#include <string_view>

constexpr int square(int x) { return x * x; };
consteval int cube(int x) { return x * x * x; }; // consteval must run at compile time

int main()
{
    // 5.1
    const double gravity{9.81};

    // 5.2
    auto million{1'000'00};

    // 5.3
    std::cout << 26 << std::endl;
    std::cout << 0b11010 << std::endl;
    std::cout << 0x1A << std::endl;

    // 5.4, 5.5, 5.6
    /* constexpr value -> compile-time constant
     * const value -> maybe compile-time, maybe runtime */
    constexpr int width{2 + 3};
    std::cout << cube(10) << std::endl;

    // 5.7
    std::string greeting{"hello"};
    std::string person{"world!"};
    std::cout << greeting + " " + person << std::endl;
    std::cout << person.size() << " | " << person.length() << " | " << person[1] << std::endl;

    // 5.8, 5.9
    std::string_view person_read_only{"world!"};
    // person_read_only[0] = '1'; //fails as read only string_view
    person[1] = 'a';

    return 0;
}
