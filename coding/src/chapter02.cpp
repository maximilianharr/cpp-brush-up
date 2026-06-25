#include <iostream>
#include "greeting.h" // 2.12

// 2.10
#define APP_VERSION "v3.5.91"


// 2.1.
int sum( int a, int b){
    return a + b;
}

void greeting () {
    std::cout << "hello" << std::endl;
}

// 2.6
void printSum(int a, int b){
    std::cout << "Sum: " << sum(a,b) << std::endl;
}

// 2.9
namespace math
{
    int square(int a) {return a*a;};
}

namespace config
{
    int square(int a) {return a;};
}

int main () {
    // 2.1
    int a {2};
    int b {10};
    std::cout << "sum:" << sum(a,b) << std::endl;

    //2.3 
    greeting();

    // 2.6
    printSum(10,20);

    //2.8
    std::cout << greeting_message() << std::endl;

    //2.9
    std::cout << math::square(10) << std::endl;
    std::cout << config::square(10) << std::endl;

    // 2.10
    std::cout << "Version: " << APP_VERSION << std::endl;
    return 0;
}