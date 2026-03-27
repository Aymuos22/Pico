// main.cpp
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "run.h"

static constexpr const char* kVersion = "0.4.0";
static constexpr const char* kDefaultSource = "test.ds";

/// True if the last path segment already contains a dot (e.g. `foo.ds`, `a.b`).

static bool basename_has_extension(const std::string& path) {
    size_t slash = path.find_last_of("/\\");
    std::string base = (slash == std::string::npos) ? path : path.substr(slash + 1);
    if (base.empty() || base == "." || base == "..") {
        return false;
    }
    return base.find('.') != std::string::npos;  
}

/// Opens Pico source: uses the path as given, or if that fails and the basename has no extension, tries `path + ".ds"`.
static bool load_source_file(std::string path, std::string& out_code, std::string& out_path_used) {
    std::ifstream file(path);
    if (file.is_open()) {
        out_code.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        out_path_used = std::move(path);
        return true;
    }
    if (!basename_has_extension(path)) {
        std::string with_ds = path + ".ds";
        std::ifstream file_ds(with_ds);
        if (file_ds.is_open()) {
            out_code.assign((std::istreambuf_iterator<char>(file_ds)), std::istreambuf_iterator<char>());
            file_ds.close();
            out_path_used = std::move(with_ds);
            return true;
        }
    }
    out_path_used = std::move(path);
    return false;
}

static void print_usage(const char* argv0) {
    std::cout << "Pico interpreter " << kVersion << "\n\n"
              << "Usage:\n"
              << "  " << argv0 << " [options] [file]\n"
              << "  " << argv0 << " -e \"code\" [options]\n\n"
              << "Options:\n"
              << "  -h, --help      Show this help\n"
              << "  -v, --version   Show version\n"
              << "  -e, --eval STR  Run source string STR\n"
              << "  --tokens        Print lexer tokens (debug)\n\n"
              << "Source files normally use the .ds extension. If you pass a path with no extension\n"
              << "(e.g. myapp), Pico will try myapp.ds when the first open fails.\n\n"
              << "If no file and no -e are given, reads " << kDefaultSource << " in the current directory.\n";
}

int main(int argc, char* argv[]) {
    std::string filename;
    std::string code;
    RunOptions options;

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& a = args[i];
        if (a == "-h" || a == "--help") {
            print_usage(argv[0]);
            return 0;
        }
        if (a == "-v" || a == "--version") {
            std::cout << "pico " << kVersion << std::endl;
            return 0;
        }
        if (a == "--tokens") {
            options.dump_tokens = true;
            continue;
        }
        if (a == "-e" || a == "--eval") {
            if (i + 1 >= args.size()) {
                std::cerr << "Error: " << a << " requires an argument" << std::endl;
                return 1;
            }
            code = args[i + 1];
            filename = "<eval>";
            ++i;
            continue;
        }
        if (!a.empty() && a[0] == '-') {
            std::cerr << "Error: Unknown option " << a << std::endl;
            print_usage(argv[0]);
            return 1;
        }
        if (filename.empty() && code.empty()) {
            filename = a;
        } else {
            std::cerr << "Error: Unexpected argument " << a << std::endl;
            return 1;
        }
    }

    if (code.empty()) {
        if (filename.empty()) {
            filename = kDefaultSource;
        }
        std::string path_used;
        if (!load_source_file(std::move(filename), code, path_used)) {
            std::cerr << "Error: Could not open file " << path_used;
            if (!basename_has_extension(path_used)) {
                std::cerr << " (also tried " << path_used << ".ds)";
            }
            std::cerr << std::endl;
            return 1;
        }
        filename = std::move(path_used);
    }

    std::pair<Value, Error> result_error = run(filename, code, options);
    Value result = result_error.first;
    Error error = result_error.second;

    if (!error.is_empty()) {
        std::cout << "Error: " << error.as_string() << std::endl;
        return 1;
    }

    if (result.is_defined()) {
        std::cout << "Result: " << result.to_string() << std::endl;
    }

    return 0;
}
