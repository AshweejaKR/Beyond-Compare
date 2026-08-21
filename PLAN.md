# BCClone Development Plan

## Phase 0 --- Project Foundation

Status: ✅ Complete

Goals - Repository - CMake - Qt6 - Logging - Settings - MainWindow - CI
skeleton

Acceptance - Configure succeeds - Build succeeds - Application
launches - Manual verification completed

## Phase 1 --- Text Compare Engine

Status: ✅ Complete

-   File loading
-   Encoding detection
-   Text normalization
-   LCS diff
-   Diff model

Acceptance - Configure succeeds - Build succeeds - Application
launches - Manual verification completed

## Phase 2 --- Qt UI

Status: ✅ Complete

-   Main window
-   Menus
-   Toolbar
-   Editors
-   Scroll sync
-   Theme

Acceptance - Configure succeeds - Build succeeds - Application
launches - Manual verification completed

## Phase 3 --- Folder Comparison

Status: ⬜ Not Started

-   Recursive scan
-   Filters
-   Size/date/hash comparison
-   Tree view

Acceptance - Configure succeeds - Build succeeds - Application
launches - Manual verification completed

## Phase 4 --- Binary Comparison

Status: ⬜ Not Started

-   Hex viewer
-   ASCII view
-   Diff highlighting

Acceptance - Configure succeeds - Build succeeds - Application
launches - Manual verification completed

## Future

5.  Merge Tool
6.  Performance & Multithreading
7.  Advanced Comparisons
8.  Plugins
9.  Packaging & Release
10. Production Polish

## Progress Log

Append after every completed task: - Completed - Decisions - Issues -
Next Task

### 2026-08-01 --- Phase 0 Complete

Completed - Verified the existing Phase 0 scaffold (CMake, Qt6,
MainWindow, Logger, SettingsManager, CI) configures, builds, and
launches cleanly on Windows/MSVC via vcpkg-provided Qt6.

Issues found and fixed - `qt_add_executable(BCClone WIN32_EXECUTABLE
main.cpp)` failed to configure: this vcpkg Qt6 build's
`qt_add_executable` does not parse `WIN32_EXECUTABLE` as a keyword and
forwards it to `add_executable` as a literal source file. Fixed by
setting `WIN32_EXECUTABLE` as a target property instead. - The built
executable aborted at startup with "no Qt platform plugin could be
initialized": the `platforms/qwindowsd.dll` plugin was never deployed
next to the executable. Fixed with a `POST_BUILD` custom command on
the `BCClone` target that copies `Qt6::QWindowsIntegrationPlugin` into
a `platforms/` subdirectory next to the exe (Windows only).

Decisions - Did not reintroduce GoogleTest (removed in the prior
session, see docs/CHANGELOG.md) — CLAUDE.md now mandates manual
verification only. - Did not wire `resources/icons/app_icon.svg` into
a compiled Qt resource (.qrc): the vcpkg Qt6 install here has no
QtSvg module, so a `QIcon` loaded from an embedded SVG would silently
fail to render. MainWindow's existing programmatically-drawn
placeholder icon already satisfies "sample icon" and is verified
working; the SVG remains a source asset for future packaging
(app .ico / .desktop icon).

Next Task - Phase 1: Text Compare Engine (file loading, encoding
detection, text normalization, LCS diff, diff model).

### 2026-08-21 --- Phase 1 Complete

Completed - Added a text comparison core with filesystem-based file
loading, UTF-8/UTF-16 BOM detection, Latin-1 fallback, line-ending and
optional whitespace/case normalization, an LCS diff algorithm, and a
shared side-by-side diff model.

Decisions - Kept all comparison behavior outside the UI. Used Qt Core
value types at the core boundary to avoid unnecessary text conversions
while preserving the documented dependency direction.

Next Task - Phase 2: Qt UI for opening, comparing, and viewing text files.

### 2026-08-21 --- Phase 2 Complete

Completed - Added a working side-by-side text comparison screen with
left/right file pickers, compare options, aligned diff rows, colored
highlights, synchronized scrolling, menus, toolbar actions, status
messages, and light/dark themes.

Decisions - Kept file workflow and diff rendering in a focused
TextCompareWidget while MainWindow remains responsible for application
chrome and themes.

Next Task - Phase 3: Recursive folder comparison and tree view.
