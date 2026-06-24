# Chapter 18: Iterators and Algorithms

Current LearnCpp Chapter 18 pages surfaced via web search are 18.1–18.4 below. This refresher skips quiz material and focuses on the practical STL takeaways.

## 18.1 Sorting an array using selection sort

- **Selection sort** repeatedly finds the smallest remaining element and swaps it into the next sorted position.
- It is useful as a teaching algorithm because the logic is simple and explicit.
- Cost is **O(n²)** comparisons, so it is fine for learning but usually not for production code.
- Learn the pattern, then prefer `std::sort` for real work.

```cpp
#include <array>
#include <iostream>
#include <utility>

template <typename T, std::size_t N>
void selectionSort(std::array<T, N>& arr)
{
    for (std::size_t start{}; start < N - 1; ++start)
    {
        std::size_t bestIndex{ start };

        for (std::size_t current{ start + 1 }; current < N; ++current)
        {
            if (arr[current] < arr[bestIndex])
                bestIndex = current;
        }

        std::swap(arr[start], arr[bestIndex]);
    }
}
```

```cpp
std::array data{ 30, 50, 20, 10, 40 };
selectionSort(data);

for (int value : data)
    std::cout << value << ' '; // 10 20 30 40 50
```

**Mental model**
- Sorted region grows from the front.
- Unsorted region shrinks from the back.
- Each pass places exactly one element in final position.

**Refresher takeaway:** understand selection sort to understand algorithm structure; use STL algorithms when available.

## 18.2 Introduction to iterators

- An **iterator** is an object that points at elements in a container and lets generic code traverse that container.
- Think of it as a generalized pointer:
  - `*it` dereferences to the current element
  - `++it` advances
  - `begin()` gives the first element
  - `end()` gives a **past-the-end sentinel**, not a real element
- Iterators decouple traversal logic from container type.

```cpp
#include <iostream>
#include <vector>

std::vector numbers{ 10, 20, 30, 40 };

for (auto it{ numbers.begin() }; it != numbers.end(); ++it)
    std::cout << *it << ' ';
```

Range-based `for` is often the cleanest read-only traversal:

```cpp
for (int value : numbers)
    std::cout << value << ' ';
```

Common operations:
- `begin(container)`, `end(container)`
- `cbegin()`, `cend()` for const iteration
- `rbegin()`, `rend()` for reverse iteration

```cpp
for (auto it{ numbers.rbegin() }; it != numbers.rend(); ++it)
    std::cout << *it << ' '; // 40 30 20 10
```

Iterator categories matter because not every algorithm needs the same powers:
- **Input**: read once while moving forward
- **Output**: write while moving forward
- **Forward**: multi-pass forward traversal
- **Bidirectional**: can also move backward
- **Random access**: jump with `+`, `-`, indexing-like movement
- **Contiguous**: elements are laid out consecutively in memory

Examples:
- `std::vector`/`std::array`: random access, contiguous
- `std::list`: bidirectional, not random access
- `std::forward_list`: forward only

**Rules to remember**
- Never dereference `end()`.
- Iterator validity can be broken by container modifications (especially insert/erase/reallocation).
- Prefer iterators when you need position-aware traversal; prefer range-for when you just want each element.

## 18.3 Introduction to standard library algorithms

- `<algorithm>` gives you tested, expressive building blocks for common operations.
- Most algorithms work on an iterator pair: **`[first, last)`**.
- Benefits:
  - less handwritten loop code
  - clearer intent
  - reusable with many containers

```cpp
#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>
```

### Searching

```cpp
std::vector values{ 2, 4, 6, 8, 10 };

auto found{ std::find(values.begin(), values.end(), 6) };
if (found != values.end())
    std::cout << "found at index " << (found - values.begin()) << '\n';

auto found2{ std::ranges::find(values, 8) };
if (found2 != values.end())
    std::cout << "found 8\n";
```

Use predicates when matching by condition:

```cpp
auto evenOverFive{ std::find_if(values.begin(), values.end(),
    [](int x) { return x > 5 && x % 2 == 0; }) };
```

### Sorting

```cpp
std::vector scores{ 42, 17, 99, 58 };

std::sort(scores.begin(), scores.end());
std::ranges::sort(scores, std::greater<>{}); // descending
```

### Counting / applying / transforming

```cpp
int evens{ std::count_if(values.begin(), values.end(),
    [](int x) { return x % 2 == 0; }) };

std::for_each(values.begin(), values.end(),
    [](int x) { std::cout << x << ' '; });

std::vector<int> doubled(values.size());
std::transform(values.begin(), values.end(), doubled.begin(),
    [](int x) { return x * 2; });

std::ranges::transform(values, doubled.begin(),
    [](int x) { return x + 1; });
```

Other high-value algorithms worth remembering:
- `std::all_of`, `std::any_of`, `std::none_of`
- `std::count`, `std::count_if`
- `std::copy`, `std::copy_if`
- `std::min_element`, `std::max_element`
- `std::remove`, `std::unique` (often followed by container `erase`)

```cpp
std::vector nums{ 1, 2, 3, 4, 5, 6 };

bool allPositive{ std::ranges::all_of(nums, [](int x) { return x > 0; }) };
auto maxIt{ std::max_element(nums.begin(), nums.end()) };
```

**Refresher takeaway:** if you are about to write a loop, first ask whether an algorithm already expresses the intent better.

## 18.4 Timing your code

- Use timing when comparing implementations, validating optimizations, or finding obviously slow paths.
- Prefer `<chrono>` over ad hoc timers.
- `std::chrono::steady_clock` is a good default for elapsed-time measurement.
- Time whole operations, not tiny fragments in noisy environments, unless you know how to benchmark carefully.

```cpp
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

class Timer
{
private:
    using clock = std::chrono::steady_clock;
    clock::time_point m_start{ clock::now() };

public:
    void reset() { m_start = clock::now(); }

    double elapsedSeconds() const
    {
        return std::chrono::duration<double>{ clock::now() - m_start }.count();
    }
};
```

```cpp
std::vector data{ 5, 1, 9, 3, 7, 2, 8, 6, 4 };

Timer timer{};
std::ranges::sort(data);

std::cout << "sort took " << timer.elapsedSeconds() << " seconds\n";
```

Practical timing advice:
- Build with optimizations when comparing realistic performance.
- Run several times; one measurement can be noisy.
- Keep inputs comparable across runs.
- Warm-up effects, CPU load, debug builds, and I/O can distort results.
- Prefer algorithmic improvements over micro-optimizations.

Good use case:

```cpp
Timer timer{};
auto count{ std::ranges::count_if(data, [](int x) { return x % 2 == 0; }) };
std::cout << count << " even numbers in " << timer.elapsedSeconds() << " seconds\n";
```

**Refresher takeaway:** measure before claiming code is faster, and use timing to compare real alternatives such as handwritten loops vs STL algorithms.
