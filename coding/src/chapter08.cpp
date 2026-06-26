#include <iostream>
#include <random>

using std::cout;
using std::endl;

int main()
{
    int val{5};

    // 8.1, 8.2, 8.3
    if (val == 5)
    {
        cout << val << endl;
    }
    else if (val == 6)
    {
        cout << val << endl;
    }
    else
    {
        cout << "val neither 5 nor 6" << endl;
    }

    // 8.5, 8.6
    char argument{'h'};

    switch (argument)
    {
    case 'q':
        cout << "quit" << endl;
        break;
    case 'h':
        cout << "help" << endl;
        break;
    default:
        cout << "unknown" << endl;
        break;
    }

    // 8.7
    // usually avoid goto
    if (argument == 'h')
    {
        goto cleanup;
    }
    cout << "not skipping goto" << endl;

cleanup:
    cout << "reached cleanup" << endl;

    // 8.8
    while (val < 10)
    {
        cout << val;
        ++val;
    }

    // 8.9
    do
    {
        cout << val;
        --val;
    } while (val > 5);

    // 8.14
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> dist{1, 6};
    cout << endl
         << dist(gen) << endl; // like rolling a dice

    return 0;
}
