# Pico

Pico is a small interpreted language for learning and quick numeric scripting. This repository contains a minimal C++ interpreter with a straightforward lexer, recursive-descent parser, and tree-walking evaluator. **Source files use the `.ds` extension** (Pico “dot source”). If you pass a path with no extension (e.g. `myapp`), the runner will try `myapp.ds` when the first path does not exist.

## Features

- **Arithmetic** — `+`, `-`, `*`, `/`, `%` (modulo), unary `-`, parentheses
- **Numbers** — Integer and decimal literals (e.g. `3`, `3.14`)
- **Strings** — Double-quoted string literals for `print`
- **Variables** — `let name = expression` and assignment `name = expression`
- **Built-in** — `print expression` writes to standard output
- **Programs** — Multiple statements separated by newlines or `;`
- **CLI** — Run a file, use `-e` for one-liners, optional `--tokens` for lexer debug output
- **I/O & timing** — **`read`** (one line from stdin → number if the whole line is numeric, otherwise text), **`rand n`** (uniform random integer from **1** through **n**, for dice-style rolls), **`sleep ms`** (pause for a whole number of milliseconds, up to 24 hours)

### Pico-only ideas (what makes it a bit different)

- **`that`** — After any `print`, `let`, `assign`, `trace`, `times`, or bare expression, the last meaningful value is stored. Use **`that`** like a pronoun: `let x = 2` then `print that + 3`. You cannot assign to `that` (it is not a variable).
- **`n times …`** — English-style counted repetition, e.g. `3 times print "hi"` or `(1 + 2) times print that`. The count must be a non-negative whole number.
- **`trace expr`** — Prints a step-by-step evaluation log for `expr` (numbers, strings, variables, operators) and still returns the result like a normal expression.
- **`read` / `rand` / `sleep`** — Good for tiny games and scripts: e.g. `let name = read`, `let d6 = rand 6`, `sleep 500`. See `examples/play.ds`.

## License

This project is open source under the [MIT License](LICENSE).

## Requirements

- A C++17 compiler (GCC, Clang, or MSVC)

## Build

From the `Pico` directory:

```bash
g++ -std=c++17 -O2 -o pico main.cpp lexer.cpp token.cpp parser.cpp node.cpp interpreter.cpp run.cpp error.cpp context.cpp symbol_table.cpp value.cpp strings_with_arrows.cpp utils.cpp
```

**MSVC (Developer Command Prompt):**

```cmd
cl /std:c++17 /EHsc /Fe:pico.exe main.cpp lexer.cpp token.cpp parser.cpp node.cpp interpreter.cpp run.cpp error.cpp context.cpp symbol_table.cpp value.cpp strings_with_arrows.cpp utils.cpp
```

## Usage

```text
pico [options] [file]
pico -e "code" [options]
```

| Option             | Meaning                    |
| ------------------ | -------------------------- |
| `-h`, `--help`     | Show help                  |
| `-v`, `--version`  | Show version               |
| `-e`, `--eval STR` | Run source `STR`           |
| `--tokens`         | Print lexer tokens (debug) |

If neither `-e` nor a file path is given, the interpreter reads `test.ds` in the current working directory.

### Examples

```bash
./pico examples/hello.ds
./pico -e "print 2 + 3 * 4"
./pico --tokens test.ds
```

You can also run `pico myapp` and it will open `myapp.ds` when `myapp` has no file extension and does not exist as a bare path.

### Language sketch

```text
let x = 10
x = x + 1
print x
print "done"
print (x % 3)

trace (x + 1) * 2
3 times print "tick"
print that
2 times print 5
print that + 2
```

See `examples/uniques.ds` for a short demo of `that`, `times`, and `trace`.

## Project layout

| Path                            | Role                         |
| ------------------------------- | ---------------------------- |
| `main.cpp`                      | CLI entry                    |
| `lexer.cpp`, `parser.cpp`       | Front end                    |
| `interpreter.cpp`, `run.cpp`    | Evaluation                   |
| `value.cpp`, `symbol_table.cpp` | Runtime values and variables |

## Contributing

Issues and pull requests are welcome. By contributing, you agree that your contributions are licensed under the MIT License.
