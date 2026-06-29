#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <functional>

using std::cout;
using std::endl;

template <typename T, std::size_t N>
void print(const std::array<T, N> &v_array)
{
    for (const T &elem : v_array)
    {
        cout << elem << endl;
    }
}

int main()
{
    // 17.1
    std::array<int, 4> stdarr_a{1, 4, 9, 16};

    // 17.1, 17.4, 17.5, 17.3
    std::array<int, 5> a{1, 2, 3};
    std::array<std::string, 5> names{"Kim", "Sam", "Joe", "Max", "Gus"};
    print(a);
    print(names);

    std::array<std::pair<int, std::string>, 2> idname{
        std::pair{1, "joe"}, std::pair{2, "gus"}};

    for (const std::pair<int, std::string> &elem : idname)
    {
        cout << "name: " << elem.second << endl;
    }

    int val_a{1};
    int val_b{2};
    std::array<std::reference_wrapper<int>, 2> refs{val_a, val_b};
    refs[1].get() = 32;

    // 17.2, 17.6
    std::array<int, 4> arr{11, 22, 33, 44};
    cout << "front: " << arr.front() << endl;
    cout << "back: " << arr.back() << endl;
    cout << "size: " << arr.size() << endl;
    cout << "[2]: " << arr[2] << endl;
    cout << "at(2): " << arr.at(2) << endl;

    // 17.7
    constexpr std::size_t length{4};
    int val_c[length]{1, 2, 3, 4};
    int val_d[length]{};

    // 17.8, 17.9, 17.10, 17.11
    int values[]{10, 20, 30};
    int *pvalues{values};
    int x{*(pvalues + 1)};
    cout << "x: " << x << endl;
}
