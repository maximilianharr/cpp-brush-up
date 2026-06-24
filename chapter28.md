# Chapter 28: Input and Output (I/O)

## 28.1 Input and output (I/O) streams

- C++ I/O lives in the standard library, mainly `<iostream>`, not the core language.
- A **stream** is a flow of bytes between your program and some device/source.
- Main direction types:
  - `std::istream`: input
  - `std::ostream`: output
  - `std::iostream`: input + output
- Common standard streams:
  - `std::cin`: standard input
  - `std::cout`: standard output
  - `std::cerr`: unbuffered error output
  - `std::clog`: buffered diagnostic output
- Streams hide device details: keyboard, console, file, string buffer, etc.
- I/O uses operators heavily:
  - `>>` extraction (read from stream)
  - `<<` insertion (write to stream)
- Most stream operations return the stream, enabling chaining.
- Stream families you’ll use often:
  - console: `cin`, `cout`
  - files: `ifstream`, `ofstream`, `fstream`
  - strings: `istringstream`, `ostringstream`, `stringstream`

```cpp
#include <iostream>

int main()
{
    int age{};
    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "Next year: " << (age + 1) << '\n';
}
```

**Refresher:** think “same stream interface, different backing source”.

## 28.2 Input with istream

- `operator>>` reads formatted input into typed variables.
- Extraction usually **skips leading whitespace**.
- Reading stops when the next character no longer fits the requested type.
- Common issue: leftover newline remains in the input buffer after `>>`.
- Use `std::getline()` when you want a whole line, including spaces.
- Use `std::ws` before `getline()` to eat leading whitespace/newlines.
- Character-level input:
  - `get(ch)` reads one char, including whitespace
  - `peek()` looks without consuming
  - `ignore()` discards characters
- Chained extraction works left-to-right and short-circuits on failure.
- For C-style buffers, width-limiting matters; `std::string` is usually safer.

```cpp
#include <iostream>
#include <string>

int main()
{
    int id{};
    std::string fullName{};

    std::cin >> id;
    std::getline(std::cin >> std::ws, fullName);

    std::cout << id << ": " << fullName << '\n';
}
```

```cpp
char ch{};
std::cin.get(ch);   // reads spaces/newlines too
std::cin.peek();    // inspect next character
std::cin.ignore(1000, '\n'); // discard up to newline
```

**Rule of thumb:** `>>` for tokenized input, `getline()` for line-oriented input.

## 28.3 Output with ostream and ios

- `operator<<` performs formatted output.
- Prefer `'\n'` over `std::endl` unless you specifically need a flush.
- `std::ios` / `std::ios_base` stores formatting flags and stream settings.
- Useful manipulators from `<iomanip>`:
  - `std::setw(n)` width for next field only
  - `std::setfill(ch)` fill character
  - `std::left`, `std::right`
  - `std::fixed`, `std::scientific`
  - `std::setprecision(n)`
  - `std::boolalpha`
- Useful base/flag formatting:
  - `std::hex`, `std::dec`, `std::oct`
  - `std::showbase`, `std::showpos`, `std::uppercase`
- Formatting state often persists until changed back.

```cpp
#include <iomanip>
#include <iostream>

int main()
{
    double value{ 123.456789 };

    std::cout << std::fixed << std::setprecision(2) << value << '\n';   // 123.46
    std::cout << std::boolalpha << true << '\n';                        // true
    std::cout << std::showbase << std::hex << 255 << '\n';              // 0xff
    std::cout << std::left << std::setw(10) << "item" << 42 << '\n';
}
```

```cpp
std::cout << "line 1\n";      // usually preferred
std::cout << "line 2" << std::endl; // newline + flush
```

**Refresher:** output formatting is mostly “set stream state, then print”.

## 28.4 Stream classes for strings

- `<sstream>` gives stream interfaces backed by strings.
- Main types:
  - `std::istringstream`: read from a string
  - `std::ostringstream`: write to a string
  - `std::stringstream`: both
- Great for:
  - parsing mixed text/numbers
  - building formatted strings
  - simple conversions
  - processing one line at a time after `getline()`
- Use `.str()` to get/set the underlying string.
- If reuse fails after extraction, call `.clear()` before reusing the stream.

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::istringstream input{ "42 3.14 hello" };
    int i{};
    double d{};
    std::string s{};

    input >> i >> d >> s;
    std::cout << i << ", " << d << ", " << s << '\n';
}
```

```cpp
std::ostringstream out{};
out << "score=" << 97 << ", avg=" << std::fixed << std::setprecision(1) << 91.5;

std::string message{ out.str() };
```

```cpp
std::stringstream ss{};
ss.str("10 20");
int a{}, b{};
ss >> a >> b;
```

**Refresher:** string streams let you reuse normal stream syntax without actual files/console I/O.

## 28.5 Stream states and input validation

- Streams track error/state flags:
  - `goodbit`: no error
  - `failbit`: recoverable format/input failure
  - `badbit`: serious stream error
  - `eofbit`: end-of-file reached
- Common checks:
  - `if (stream)` / `if (!stream)`
  - `stream.fail()`
  - `stream.bad()`
  - `stream.eof()`
  - `stream.good()`
- After failed extraction:
  1. `clear()` the error state
  2. discard bad input with `ignore()`
  3. try again
- Numeric validation often needs two checks:
  - was the input the right type?
  - is the value in the allowed range?
- If you mix `>>` and `getline()`, leftover buffered characters matter.

```cpp
#include <iostream>
#include <limits>

int main()
{
    int value{};

    while (true)
    {
        std::cout << "Enter an integer from 1 to 5: ";

        if ((std::cin >> value) && value >= 1 && value <= 5)
            break;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, try again.\n";
    }

    std::cout << "Accepted: " << value << '\n';
}
```

```cpp
if (std::cin.eof())
{
    // input source ended
}
```

**Refresher:** failed streams stay failed until you explicitly recover them.

## 28.6 Basic file I/O

- File streams live in `<fstream>`.
- Main types:
  - `std::ifstream`: read from file
  - `std::ofstream`: write to file
  - `std::fstream`: read + write
- Typical open modes:
  - `std::ios::in`
  - `std::ios::out`
  - `std::ios::app`
  - `std::ios::trunc`
  - `std::ios::binary`
- Construction can open immediately, or use `.open()`.
- Always verify the file opened successfully.
- Files close automatically via RAII when the stream object dies.
- Reading patterns:
  - token by token: `file >> x`
  - line by line: `std::getline(file, line)`
- Writing works just like `std::cout`.

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ofstream outFile{ "notes.txt" };
    if (!outFile)
    {
        std::cerr << "Could not open notes.txt for writing\n";
        return 1;
    }

    outFile << "alpha\nbeta\ngamma\n";
}
```

```cpp
std::ifstream inFile{ "notes.txt" };
if (!inFile)
{
    std::cerr << "Could not open notes.txt for reading\n";
    return 1;
}

std::string line{};
while (std::getline(inFile, line))
    std::cout << line << '\n';
```

**Refresher:** file streams behave like `cin`/`cout`, just with a file as the source/sink.

## 28.7 Random file I/O

- Sequential I/O reads/writes in order; random I/O jumps directly to positions.
- Position functions:
  - `seekg()` / `tellg()` for input position
  - `seekp()` / `tellp()` for output position
- Reference points:
  - `std::ios::beg`
  - `std::ios::cur`
  - `std::ios::end`
- Best with **binary files** or fixed-size records.
- Be careful with random access in text mode:
  - encodings and newline translation can make byte positions tricky
  - especially on Windows (`\n` may be stored as CRLF)
- Common use cases:
  - jump to a record
  - patch a file in place
  - compute file size

```cpp
#include <fstream>
#include <iostream>

int main()
{
    std::ifstream file{ "data.bin", std::ios::binary };
    if (!file)
        return 1;

    file.seekg(0, std::ios::end);
    std::streampos size{ file.tellg() };
    file.seekg(0, std::ios::beg);

    std::cout << "File size: " << size << " bytes\n";
}
```

```cpp
struct Record
{
    int id{};
    double score{};
};

std::fstream file{ "records.bin", std::ios::in | std::ios::out | std::ios::binary };
std::size_t index{ 3 };

file.seekg(static_cast<std::streamoff>(index * sizeof(Record)), std::ios::beg);
Record r{};
file.read(reinterpret_cast<char*>(&r), sizeof(r));
```

**Refresher:** random file I/O is mostly “move the cursor, then read/write”.
