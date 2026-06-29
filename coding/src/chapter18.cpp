#include <iostream>
#include <utility>
#include <array>
#include <vector>
#include <algorithm>
#include <ranges>
#include <chrono>

using std::cout;
using std::endl;

template <typename T>
void print(std::vector<T> vec)
{
    for (auto elem : vec)
    {
        cout << elem << " | ";
    }
    cout << endl;
}

// O(n2) sorting algo
template <typename T, std::size_t N>
void selectionSort(std::array<T, N> &varr)
{
    for (std::size_t start{}; start < N - 1; ++start)
    {
        std::size_t bestIndex{start};
        for (std::size_t current{start + 1}; current < N; ++current)
        {
            if (varr[current] < varr[bestIndex])
            {
                bestIndex = current;
            }
        }
        std::swap(varr[start], varr[bestIndex]);
    }
}

class Timer
{
private:
    using clock = std::chrono::steady_clock;
    clock::time_point m_start{clock::now()};

public:
    void reset() { m_start = clock::now(); }
    double elapsedSeconds() const
    {
        return std::chrono::duration<double>{clock::now() - m_start}.count();
    }
};

int main()
{
    // 18.1
    std::array varr{30, 10, 20, 40, 50};
    selectionSort(varr);
    for (auto elem : varr)
    {
        cout << elem << " | ";
    }
    cout << endl;

    // 18.2
    std::vector<int> vvec{11, 22, 33, 44, 55};
    for (auto it{vvec.begin()}; it != vvec.end(); it++)
    {
        cout << *it << " | ";
    }
    for (auto it{vvec.rbegin()}; it != vvec.rend(); it++)
    {
        cout << *it << " | ";
    }
    cout << endl;

    // 18.3
    std::vector<int> points{3, 5, 1, 6, 8, 2, 4, 7};
    std::sort(points.begin(), points.end());
    print(points);
    std::ranges::sort(points, std::greater<>{});
    print(points);

    int evens{std::count_if(points.begin(), points.end(), [](int x)
                            { return x % 2 == 0; })};
    print(points);

    // 18.4
    std::vector<int> data{5, 1, 4, 5, 6, 8, 8, 9, 7, 2};
    Timer timer{};
    std::ranges::sort(data, std::greater<>{});
    std::cout << "sort took: " << timer.elapsedSeconds() << " s." << endl;
}
