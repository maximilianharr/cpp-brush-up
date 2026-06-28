#include <iostream>

using std::cout;
using std::endl;

bool divideBy(double a, double b, double &result)
{
    if (b == 0)
    {
        throw std::runtime_error{"negative input"};
    }
    else
    {
        return a / b;
    }
}

int main()
{
    // 9.4
    double result;
    divideBy(10, 5, result);
    cout << "hello world!" << endl;
}
