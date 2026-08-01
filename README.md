# BCClone

A modern, native, production-quality alternative to [Beyond Compare](https://www.scootersoftware.com/),
built with C++20, Qt6 Widgets, and CMake.

> **Status:** Phase 0 — project foundation. No comparison functionality exists yet.
> See [PLAN.md](PLAN.md) for the roadmap and [docs/SPECIFICATION.md](docs/SPECIFICATION.md) for scope.

## Technology

- C++20
- Qt6 (Core, Gui, Widgets)
- CMake 3.28+
- GoogleTest

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

## Running tests

```sh
ctest --test-dir build --output-on-failure
```

## Running the app

```sh
./build/src/app/BCClone
```

## Project layout

```
src/
  app/       Application entry point
  ui/        Presentation (Qt widgets)
  core/      Comparison engines and business logic (future)
  compare/   Text/code comparison (future)
  folder/    Folder comparison (future)
  binary/    Binary comparison (future)
  merge/     Merge tooling (future)
  models/    Shared data models (future)
  utils/     Logging, settings, and other utilities
tests/       GoogleTest unit tests
docs/        Architecture and specification documents
```

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for the layering rules that govern this layout.

## License

[MIT](LICENSE)
