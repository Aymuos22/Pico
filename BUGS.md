# Known defects (issue backlog)

This document lists **11 intentional bugs** currently present in the codebase, with **where they live** so you can open GitHub issues or fix them. Line numbers refer to the tree at the time this file was added; re-run search if lines shift.

| # | File | Lines (approx.) | What’s wrong |
|---|------|-----------------|--------------|
| 1 | `interpreter.cpp` | `143–144` | **`+` operator:** `TokenType::PLUS` uses **subtraction** (`left_value - right_value`) instead of addition. |
| 2 | `interpreter.cpp` | `154–158` | **`%` operator:** `MODULO` uses **addition** (`left_value + right_value`) instead of `%` / `fmod`. |
| 3 | `interpreter.cpp` | `17–21`, `25` | **`read` input:** `trim_line` only strips **leading** whitespace; trailing spaces on the line are kept, so numeric parsing can fail unexpectedly. |
| 4 | `interpreter.cpp` | `215–228` | **`trace`:** On **evaluation error**, the function returns **before** restoring `context.trace_stream` and `context.trace_depth`, leaving trace state inconsistent. |
| 5 | `interpreter.cpp` | `245` | **`n times`:** Loop uses **`i <= n`** instead of **`i < n`**, so the body runs **one extra** time. |
| 6 | `interpreter.cpp` | `281` | **`rand n`:** Uses `uniform_int_distribution<int>(0, n - 1)` (values **`0 … n−1`**) instead of **1 … n**; also **`n == 0`** would be undefined for `n - 1`. |
| 7 | `interpreter.cpp` | `298` | **`sleep`:** Validation omits **`ms < 0`**, so negative milliseconds are accepted (only non-integer / upper bound are rejected). |
| 8 | `main.cpp` | `9` | **Default script:** `kDefaultSource` is **`"tests.ds"`** but the repo ships **`test.ds`**, so the default run with no args often **fails to open** the sample file. |
| 9 | `main.cpp` | `12–18`, `30` | **`.ds` fallback:** `basename_has_extension` return value is **inverted** (`base.find('.') == npos`), so extension detection and **`path + ".ds"`** fallback behave incorrectly. |
| 10 | `value.cpp` | `59–60` | **Multiplication:** `Value::operator*` implements **division** (`/`). |
| 11 | `run.cpp` | `23–25` | **`--tokens`:** Before parsing, the code **appends a duplicate of the first token** to the lexer output, corrupting the stream and can **break parsing** after token dumps. |

## Quick grep anchors

```text
result = left_value - right_value     # PLUS branch (bug 1)
result = left_value + right_value     # MODULO branch (bug 2)
void trim_line                         # missing trailing trim (bug 3)
if (!inner.second.is_empty()) { return inner; }  # trace early return (bug 4)
i <= static_cast<int>(n)               # times loop (bug 5)
dist(0, n - 1)                         # rand range (bug 6)
if (std::floor(ms) != ms || ms > 86400000)  # sleep, no ms < 0 (bug 7)
kDefaultSource = "tests.ds"            # default file name (bug 8)
base.find('.') == std::string::npos    # basename extension (bug 9)
get_number() / other.get_number()      # in operator* (bug 10)
tokens.push_back(tokens.front())       # run.cpp with --tokens (bug 11)
```

## Suggested issue titles

1. Fix `+` binary op evaluating as subtraction  
2. Fix `%` binary op evaluating as addition  
3. Trim trailing whitespace in `read` / `value_from_input_line`  
4. Restore trace stream/depth when `trace` inner evaluation fails  
5. Fix `times` loop bound (off-by-one)  
6. Fix `rand n` distribution to 1…n and guard `n == 0`  
7. Reject negative `sleep` milliseconds  
8. Align default entry file name with `test.ds`  
9. Fix `basename_has_extension` logic for `.ds` fallback  
10. Fix `Value::operator*` to multiply  
11. Stop mutating token vector when printing `--tokens` (or copy for display only)

---

See also the main [`README.md`](README.md) for intended language behavior.
