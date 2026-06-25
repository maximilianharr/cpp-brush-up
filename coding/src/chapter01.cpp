#include <iostream>

int main() {
    // 1.1 / 1.2
    //comment 1
    /* comment 2 
     * with multiple lines */
    std::cout << "hello" << std::endl;

    // 1.3
    double length = {3.0};
    double width = {2.0};

    // 1.4
    double area = length * width;
    std::cout << "Area: " << area << std::endl;

    // 1.5
    int age {0};
    std::cout << "Enter your age: ";
    //std::cin >> age;
    std::cout << "You are " << age << " years old." << std::endl;

    // 1.6, 1.7, 1.8, 1.9, 1.10
    int total { (3 + 4) * 5 };

    // 1.10


    return 0;
}