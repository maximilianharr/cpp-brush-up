#include <iostream>
#include <vector>
#include <array>

using std::cout;
using std::endl;

template <typename T>
void print(const std::vector<T> &v)
{
    for (const T &value : v)
    {
        cout << value << ", ";
    }
    cout << endl;
}

int main()
{
    // 16.1
    std::array<int, 3> stdarr{1, 2, 3};
    std::vector<int> stdvec{1, 2, 3};

    for (auto i : stdvec)
    {
        cout << i << endl;
    }

    // 16.2
    std::vector<double> stdvec_a(5, 2.0); // 10 elements at 2.0
    stdvec_a.push_back(3.0);
    for (auto i : stdvec_a)
    {
        cout << i << "|";
    };
    std::vector<std::vector<int>> matrix{{1, 2, 3}, {4, 5, 6}};

    int x = matrix[1][2];
    cout << endl
         << "x: " << x << endl;

    // 16.3
    for (std::size_t i{0}; i < stdvec_a.size(); ++i)
    {
        std::cout << stdvec_a[i] << ' ';
    }

    // 16.4
    print(stdvec_a);

    // 16.5
    int count = 5;
    std::vector<int> stdvec_b{};
    stdvec_b.reserve(count);
    for (std::size_t i{0}; i <= count; i++)
    {
        stdvec_b[i] = i * i;
    }

    // 17.1, 16.7, 16.10
    std::array<int, 4> stdarr_b{1, 2, 3, 4};
    cout << stdarr_b.size() << endl;
    cout << "size: " << std::ssize(stdarr_b) << endl;

    // 16.11
    std::vector<int> stack{};
    stack.push_back(10);
    stack.push_back(20);
    stack.push_back(30);
    stack.pop_back();
    stack.resize(4);
    stack.reserve(10);
    stack.push_back(40);
    print(stack);
}
