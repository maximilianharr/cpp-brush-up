# Chapter 3: Debugging C++ Programs

## 3.1 Syntax and semantic errors

- **Syntax errors**: code breaks C++ grammar rules; compiler stops.
- **Semantic errors**: code is valid C++, but meaning/behavior is wrong.
- Fix syntax/compiler errors first, then debug runtime/logic issues.
- Some semantic issues are caught at compile time; others appear only at runtime.

**Typical syntax errors**
- Missing `;`
- Mismatched `()`, `{}`, `[]`
- Misspelled keywords or identifiers
- Invalid declarations

```cpp
int main() {
    std::cout << "Hello\n"   // missing ;
}
```

**Typical semantic errors**
- Wrong formula or condition
- Using an uninitialized variable
- Division by zero
- Off-by-one loop bounds
- Returning the wrong value

```cpp
int max(int a, int b) {
    return (a < b) ? a : b; // valid syntax, wrong logic
}
```

```cpp
int x {};
int y { 0 };
std::cout << x / y << '\n'; // runtime problem: divide by zero
```

**What to do**
- Read compiler errors top-down; fix the first real error first.
- Compile with warnings enabled and treat warnings as bugs to inspect.
- If code compiles but behaves wrong, suspect semantic/logic issues.
- Re-test after each fix; one syntax fix can reveal another hidden error.

---

## 3.2 The debugging process

Debugging is usually:
1. **Find** the root cause
2. **Understand** why it happens
3. **Plan** a fix
4. **Apply** the fix
5. **Retest** the original problem
6. **Retest** nearby behavior for regressions

**Key idea**
- Fixing the symptom is not enough; find the actual cause.

**Good workflow**
- Start with a clear failing case.
- Note expected vs actual behavior.
- Reduce the problem to a small reproducible example when possible.
- Change one thing at a time.

```cpp
// Expected: sum 1..3 = 6
int sum { 0 };
for (int i{1}; i < 3; ++i) // bug: should be <= 3
    sum += i;
```

**Practical checklist**
- Can I reproduce it consistently?
- Which input/state triggers it?
- Where does observed behavior first differ from expected behavior?
- Did my fix break something else?

**Rule**
- Never stop at “it works now”.
- Stop at “I know why it broke, and the fix is verified”.

---

## 3.3 A strategy for debugging

Most debugging time is spent **finding** the bad code, not fixing it.

**Strategy**
- Reproduce the bug reliably.
- Narrow the search area.
- Observe program state.
- Repeat until the bad line/assumption is isolated.

**Useful mindset**
- Don’t guess randomly.
- Form a hypothesis, test it, keep or discard it.
- Prefer binary search / divide-and-conquer over scanning everything.

**Ways to narrow**
- Check halfway through the code path: correct here or already wrong?
- Split large functions into smaller testable pieces mentally or physically.
- Compare a good run vs bad run when possible.

```cpp
// If result is wrong, inspect stages separately:
auto values = readValues();
auto filtered = filterValues(values);
auto result = computeTotal(filtered);
```

Ask:
- Is `readValues()` already wrong?
- If not, does `filterValues()` corrupt data?
- If not, bug is likely in `computeTotal()`.

**Practical rule**
- Always write down or remember the exact steps to reproduce the bug.
- If you cannot reproduce it, debugging becomes guesswork.

---

## 3.4 Basic debugging tactics

### 1) Comment out code
- Temporarily disable suspicious lines/blocks.
- If the symptom changes, the disabled region matters.
- Restore code after the experiment.

```cpp
// total += bonus; // temporarily disabled
```

### 2) Add print debugging
- Print control flow: “did we get here?”
- Print values: “what is this variable right now?”
- `std::cerr` is useful for debug messages because it is intended for diagnostics.

```cpp
std::cerr << "Reached updateScore()\n";
std::cerr << "score=" << score << ", bonus=" << bonus << '\n';
```

### 3) Validate assumptions
- If you think a function is being called, prove it.
- If you think a variable is positive, print/check it.
- If you think loop bounds are correct, inspect them.

### 4) Work incrementally
- Make one small change.
- Rebuild.
- Re-run.
- Observe.

**Good habit**
- Remove temporary debug prints after fixing the bug.
- If you use version control, review/debug-only edits before committing.

---

## 3.5 More debugging tactics

### Toggleable debug output
Use conditional compilation so debug prints are easy to enable/disable.

```cpp
#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DBG(x) std::cerr << x << '\n'
#else
#define DBG(x)
#endif

DBG("x=" << x << ", y=" << y);
```

### Log instead of cluttering normal output
- Use `std::cerr` or a log file for diagnostic output.
- Keep real program output separate from debug output.

### Reproduce first
- Don’t fix a bug you can’t trigger on demand.
- A reproducible bug gives you a way to verify the fix.

### Change one thing at a time
- If you change five things and it starts working, you still don’t know why.
- Isolate the effect of each change.

### Minimize variables
- Reduce inputs, code path, and environment differences.
- Small examples are easier to reason about than full programs.

**Practical rule**
- If the bug disappears after a “cleanup” refactor, you may have hidden it, not solved it.

---

## 3.6 Using an integrated debugger: Stepping

Stepping lets you run code **line by line** without editing source to add prints.

### Step into
- Executes the next statement.
- If it calls a function, enters that function.
- Use when the called function might be wrong.

### Step over
- Executes the next statement.
- If it calls a function, runs the whole function without entering it.
- Use when you trust that function and want to stay at the current level.

### Step out
- Finishes the current function, then pauses in the caller.
- Use when you stepped in too far.

### Run to cursor
- Runs until the selected line is reached.
- Faster than stepping through boring code.

```cpp
int z { add(x, y) }; // step into: inspect add()
std::cout << z << '\n'; // step over: likely enough here
```

**What to watch while stepping**
- Which branch executes?
- When does a variable first become wrong?
- Did control flow go where you expected?

---

## 3.7 Using an integrated debugger: Running and breakpoints

Breakpoints pause execution at the place you care about.

### Start under the debugger
- Run the program in debug mode, not just normal execution.
- Then use breakpoints and continue instead of stepping from `main()` every time.

### Breakpoints
- Set one on a suspicious line.
- Program runs normally until that line is about to execute.
- Then inspect variables, call stack, and control flow.

### Continue
- Resume execution after pausing.
- Program runs until the next breakpoint or termination.

### Run to cursor
- Good for temporary one-off pauses without placing a permanent breakpoint.

### Set next statement
- Moves the execution point to another line.
- Useful for re-running or skipping code during investigation.
- Dangerous if skipped code was needed to initialize state.

```cpp
if (count > maxCount)   // place breakpoint here
    handleOverflow();
```

**Best use**
- Put breakpoints near where wrong behavior first becomes visible.
- Then move earlier or later based on what you learn.

---

## 3.8 Using an integrated debugger: Watching variables

Watching variables lets you inspect values without adding `cout`/`cerr`.

### Common tools
- **Hover tooltip**: quick one-off value check
- **Locals window**: shows local variables in current scope
- **Watch window**: manually track chosen variables/expressions

### What to watch
- Inputs to the suspicious function
- Loop indices and bounds
- Pointers/references
- Flags/booleans controlling branches
- Values that “should never happen”

```cpp
for (int i{0}; i < size; ++i) {
    total += values[i];
}
```

Useful watches:
- `i`
- `size`
- `total`
- `values[i]`

**When to use watches**
- When a value changes over time and you need to see when it goes wrong
- When print debugging would be noisy or intrusive
- When stepping through branching logic

**Practical tip**
- If a variable looks wrong, inspect the statement that last changed it.

---

## 3.9 Using an integrated debugger: The call stack

The **call stack** shows the chain of active function calls.

Example flow:

```cpp
int main() {
    startGame();
}

void startGame() {
    loadLevel();
}

void loadLevel() {
    parseConfig();
}
```

If paused inside `parseConfig()`, the call stack typically shows:
- `parseConfig()`
- `loadLevel()`
- `startGame()`
- `main()`

### Why it matters
- Shows **how execution got here**
- Helps with nested calls and crashes
- Lets you inspect caller context

### Stack frame
- Each active function has a frame.
- Selecting a frame shows that function’s context and location.

### Use cases
- Crash occurred deep inside helper code
- Wrong argument was passed from a caller
- Recursion or repeated nested calls behave unexpectedly

**Practical rule**
- If a function receives bad input, the bug may be in the caller, not the callee.

---

## 3.10 Finding issues before they become problems

The cheapest bug is the one you prevent.

### Enable compiler warnings
- Turn warnings up.
- Read them.
- Fix them early.

### Use static analysis
- Compiler warnings are basic static analysis.
- More advanced tools can catch suspicious patterns before runtime.

### Use `assert()` to verify assumptions
- Great during development for conditions that should always be true.
- If the condition fails, stop immediately and investigate.

```cpp
#include <cassert>

void printElement(const std::vector<int>& v, int index) {
    assert(index >= 0 && index < static_cast<int>(v.size()));
    std::cout << v[index] << '\n';
}
```

### Write simpler code
- Small functions
- Clear names
- Fewer side effects
- Prefer standard library facilities over clever custom code

### Refactor safely
- Separate structural cleanup from behavioral changes.
- Test after each kind of change.

### Defensive habits
- Initialize variables.
- Check assumptions.
- Test edge cases.
- Don’t ignore “harmless” odd behavior.

**Bottom line**
- Warnings, assertions, clean structure, and disciplined habits prevent many debugging sessions entirely.
