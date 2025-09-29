# Pai Gow Poker Engine

A lightweight C++17 Pai Gow Poker engine with joker-aware hand evaluation, a demo `main`, and a test suite wired up to CTest. Portable via CMake and optional Docker.

## Project layout
```
engine/
├─ CMakeLists.txt
├─ include/
│  └─ engine.hpp
└─ src/
   ├─ engine.cpp
   ├─ main.cpp        # demo/driver
   └─ tests.cpp       # unit tests (CTests)
```

## Dependencies

### Native build
- **CMake** ≥ 3.15  
- **C++ compiler** with C++17 (any of):
  - Windows: **Visual Studio 2022** (MSVC) _or_ MinGW-w64
  - macOS: **Xcode/Clang**
  - Linux: **GCC** or **Clang**
- (Optional) **Ninja** for faster builds (`-G Ninja`)

## Build & run (native)

### Windows (Visual Studio generator)
From the repo root:
```powershell
cmake -S engine -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
# run demo and tests (executables are placed in build/bin/)
.uildin\pai_gow_main.exe
.uildin\pai_gow_tests.exe
```

Run tests with CTest (via the `check` target):
```powershell
cmake --build build --config Debug --target check
# Or directly:
ctest --test-dir build --output-on-failure -V -C Debug
```

### macOS / Linux (Makefiles or Ninja)
```bash
# Makefiles
cmake -S engine -B build
cmake --build build
./build/bin/pai_gow_main
ctest --test-dir build --output-on-failure -V

# Ninja (optional, faster)
cmake -S engine -B build -G Ninja
cmake --build build
./build/bin/pai_gow_main
ctest --test-dir build --output-on-failure -V
```

---

## CMake options

- `-DBUILD_TESTS=ON|OFF` (default **ON**): build the `pai_gow_tests` target and register with CTest.

Example:
```bash
cmake -S engine -B build -DBUILD_TESTS=OFF
```

---
## Running just one test (via CTest)

Tests are grouped inside `pai_gow_tests`. You can run all or filter:

```bash
ctest --test-dir build -V --output-on-failure            # all
ctest --test-dir build -V -R straight --output-on-failure  # only tests with "straight" in the name
```

If you added named subtests via CLI args (e.g., `pai_gow_tests straight`), you can also register them individually in CMake using `add_test(NAME straight COMMAND $<TARGET_FILE:pai_gow_tests> straight)`.

---

## Troubleshooting

- **`ctest: Test not available without configuration` (MSVC)**  
  Use `-C Debug` (or add the provided `check` target), or register tests with `$<TARGET_FILE:...>` (already done).

- **Executables not in `Debug/Release` subfolders**  
  CMake is configured to place all exes in `build/bin/` for convenience.

- **UTF-8/emoji prints look garbled on Windows**  
  Use ASCII only, or set the console to UTF-8 (`chcp 65001`) and build with `/utf-8`.

- **Missing CMake / compiler**  
  - Windows: install **Visual Studio 2022** with “Desktop development with C++” and **CMake** (winget: `winget install Kitware.CMake`).  
  - macOS: `xcode-select --install` (for Clang), `brew install cmake`.  
  - Linux: `sudo apt-get install build-essential cmake` (Debian/Ubuntu).

---

## License
MIT
