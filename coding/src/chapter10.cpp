#include <iostream>
#include <vector>

using std::cout;
using std::endl;

using Poses = std::vector<int>;

template <typename T>
using Vec = std::vector<T>;

int main()
{
    // 10.2
    char ch{'F'};
    int ch_code{ch};

    // 10.5
    double a{7.7};
    int b{10};
    auto c{a + b};
    cout << "c: " << c << endl;

    // 10.6
    int total{5};
    int count{3};
    double average{static_cast<double>(total) / count};
    cout << "average: " << average << endl;

    // 10.7, 10.8
    const Poses positions{1, 2, 3};

    for (auto i : positions)
    {
        cout << i << endl;
    }

    Vec<double> vals{1., 2., 3.};

    for (const auto &val : vals)
    {
        cout << val << endl;
    }
}
