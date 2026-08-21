# BCClone

A modern, native, production-quality alternative to [Beyond Compare](https://www.scootersoftware.com/),
built with C++20, Qt6 Widgets, and CMake.

> **Status:** Phases 0-4 complete. Text, folder, and binary comparison are available.
> See [PLAN.md](PLAN.md) for the roadmap and [docs/SPECIFICATION.md](docs/SPECIFICATION.md) for scope.

## Technology

- C++20
- Qt6 (Core, Gui, Widgets)
- CMake 3.28+

## Building

### Prerequisites

- CMake 3.28 or newer
- A C++20 compiler (MSVC 2022, GCC 12+, or Clang 15+)
- Qt6 (Core, Gui, Widgets), available either as a system install or via [vcpkg](https://github.com/microsoft/vcpkg)

### Using vcpkg

```sh
vcpkg install qtbase[core,gui,widgets] qttools

cmake -B build -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

### Using a system/official Qt6 install

```sh
cmake -B build -DCMAKE_PREFIX_PATH=<path-to-Qt6>/lib/cmake
cmake --build build
```

This project uses no automated testing framework. Every change is verified
by configuring, building, launching the application, and manually exercising
the feature. GitHub Actions builds both Windows and Linux targets.

## Running the app

Linux/macOS (single-config generator):

```sh
./build/src/app/BCClone
```

Windows with Visual Studio (multi-config generator):

```powershell
.\build\src\app\Debug\BCClone.exe
```

Use the tabs in the main window:

1. **Text Compare** — choose left and right files. Differences are aligned
   and highlighted; whitespace and case can be ignored. Use **Refresh** or
   press **F5** after either source file changes on disk. The view can show
   all lines or differences only, with optional original line numbers.
2. **Folder Compare** — choose two folders, optionally enter filters such as
   `*.cpp;*.h`, and compare. SHA-256 content verification is enabled by default.
3. **Binary Compare** — choose two files up to 2 MiB each. Differing bytes are
   highlighted in synchronized hex and ASCII views.

## Project layout

```
src/
  app/       Application entry point
  ui/        Presentation (Qt widgets)
  core/      Shared comparison business logic
  compare/   Text loading, normalization, and LCS comparison
  folder/    Recursive folder scanning and content comparison
  binary/    Binary loading and byte comparison
  merge/     Merge tooling (future)
  models/    Shared comparison result models
  utils/     Logging, settings, and other utilities
docs/        Architecture and specification documents
```

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for the layering rules that govern this layout.

## License

[MIT](LICENSE)
