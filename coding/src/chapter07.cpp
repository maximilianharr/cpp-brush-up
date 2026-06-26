#include <iostream>

using std::cout;
static int s_fileCounter{0}; // 7.6 only this .cpp can see it

namespace loc
{
    int value{42};
}

constexpr int gmode{1}; // global variable

int nextId()
{
    static int s_id{0}; // initialized once
    return ++s_id;      // remembers prior values
}

namespace
{
    void helper() {} // file-local implementation detail
}

int main()
{
    // 7.2
    cout << loc::value << std::endl;

    // 7.3, 7.4, 7.5
    if (gmode == 1)
    {
        constexpr int y = 10;
        cout << "fast\n";
        int gmode = 2; // bad
    }
    // y is gone, gmode still accessible

    // 7.7
    // use "extern" to Put declarations in headers; definition in source

    // 7.11, 7.13
    cout << "nextId: " << nextId() << "|" << nextId() << std::endl;

    // 7.12
    helper();

    // 7.14

    return 0;
}
