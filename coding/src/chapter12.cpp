#include <iostream>
#include <string>
#include <vector>
#include <string_view>

using std::cout;
using std::endl;

void increment(int &a) { ++a; };

void printName(const std::string &name) { cout << name << endl; }

int sum(const int *begin, const int *end)
{
    int total{0};
    for (const int *p{begin}; p != end; ++p)
    {
        total += *p;
    }
    return total;
}

int &getElement(std::vector<int> &v, std::size_t index)
{
    return v[index];
}

int main()
{
    // 12.1, 12.3
    int value{42};
    int &ref{value};
    int *ptr{&value};

    ref = 20;
    *ptr = 10;

    cout << "value: " << value << endl;

    // 12.4, 12.5
    const int &a{value};
    increment(value);

    // 12.6
    /* This is common for large class types
     * (std::string, std::vector, user-defined objects).
     * For small cheap types (int, double, pointers, enums),
     * pass-by-value is often simpler and just as good or better.
     */
    printName("max");

    // 12.7
    /* Pointer declaration syntax uses *: T* ptr.
     * Use &obj to get an address.
     * Use *ptr to dereference and access the pointed-to object.
     */

    int *ptr_a{&value};
    *ptr_a = 89;
    cout << *ptr_a << endl;

    // 12.8
    int *ptr_b{nullptr};
    ptr_b = &value;

    // 12.9 (read left to right
    /* const int* p = pointer to const int
     * int* const p = const pointer to int
     * const int* const p = const pointer to const int
     */
    const int *ptrToConstInt{&value};
    value = 42;
    //*ptrToConstInt = 34; // cannot change s point to constant int
    ptrToConstInt = ptr_a; // can change pointer though

    int *const constPointerToInt{&value};
    *constPointerToInt = 34; // can change value
    // constPointerToInt = ptr_a; // but pointer is constant

    const int *const constPointToConstInt(&value);
    //*constPointToConstInt = 32;
    // constPointToConstInt = ptr_a;

    // 12.10, 12.11
    int data[]{0, 1, 2, 3, 4, 5};
    int result{sum(data, data + 4)};
    cout << "data[2]: " << data[2] << endl;
    cout << "data+4: " << data + 4 << endl;

    // 12.12
    std::vector<int> v{1, 2, 3, 4, 5};
    cout << getElement(v, 1) << endl;

    for (auto val : v)
    {
        cout << val << endl;
    }

    for (auto p = v.begin(); p != v.end(); p++)
    {
        cout << *p << endl;
    }

    // 12.14
}
