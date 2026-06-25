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

    // 3.1
    std::cout << math::max(10, 20) << std::endl;

    /* using gdb
     * cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && gdb ./build/chapter03
     * commands:
     * break chapter03.cpp:11
     * info breakpoints
     * run
     * step
     * print a
     * set var a = 30
     * continue
     * backtrace
     */

    /* using VS code
     * press F5 > triggers tasks.json / launch.json
     * User VS code interface to set breakpoints and see variables / call
     * see `Ctrl Shift D`
     */

    // 3.10

    return 0;
}
