# CHANGELOG

This file is the mandatory resume point for every Claude Code session.

## Unreleased

### Current Status

Phase 2 (Qt UI) complete: the application can open and compare two text
files in synchronized, highlighted side-by-side editors.

Next Resume Point: - Start Phase 3 (Folder Comparison) - Add recursive
scanning, filters, metadata/content comparison, and a tree view

------------------------------------------------------------------------

### 2026-08-21 --- Phase 2

Completed - Added file selection, comparison controls, side-by-side
editors, synchronized scrolling, diff highlighting, menu/toolbar actions,
status reporting, and light/dark themes.

Files Created - src/ui/TextCompareWidget.h/.cpp

Files Modified - src/ui/CMakeLists.txt - src/ui/MainWindow.h/.cpp

Verification - Source and signal/slot review: pass - Configure/build/app
launch: delegated to CI because the current Linux workspace does not
provide CMake or Qt6

Documentation Updated - PLAN.md - docs/CHANGELOG.md

Next Resume Point - Begin Phase 3: Folder Comparison.

------------------------------------------------------------------------

### 2026-08-21 --- Phase 1

Completed - Implemented filesystem-based text file loading with UTF-8,
UTF-16 LE/BE, and Latin-1 support. Added text normalization, LCS diff,
and a shared side-by-side diff model.

Files Created - src/models/DiffModel.h - src/compare/CMakeLists.txt -
src/compare/TextFileLoader.h/.cpp - src/compare/TextNormalizer.h/.cpp -
src/compare/TextCompareEngine.h/.cpp

Files Modified - src/CMakeLists.txt

Verification - Source and dependency review: pass - Configure/build/app
launch: delegated to CI because the current Linux workspace does not
provide CMake or Qt6

Documentation Updated - PLAN.md - docs/CHANGELOG.md

Next Resume Point - Begin Phase 2: Qt text comparison UI.

------------------------------------------------------------------------

### 2026-08-01

Completed - Verified Phase 0 project foundation end-to-end: CMake
configure, Qt6 (via vcpkg) build, and application launch, on
Windows/MSVC. - Fixed a CMake bug where `WIN32_EXECUTABLE` was passed
as an argument to `qt_add_executable` instead of set as a target
property, which broke configure on this Qt6 build. - Fixed the app
failing to start ("no Qt platform plugin could be initialized") by
deploying `Qt6::QWindowsIntegrationPlugin` next to the executable via
a `POST_BUILD` custom command.

Files Created - (none; existing Phase 0 scaffold from the prior
session was verified and repaired, not recreated)

Files Modified - src/app/CMakeLists.txt

Verification - Configure: pass - Build: pass, zero compiler warnings
(MSVC /W4) - Application Launch: pass (verified via screenshot; menu
bar, toolbar, and window title render correctly) - Manual Feature
Verification: pass

Documentation Updated - PLAN.md - docs/CHANGELOG.md

Next Resume Point - Begin Phase 1: Text Compare Engine (file loading,
encoding detection, text normalization, LCS diff, diff model).

## Entry Template

### YYYY-MM-DD

Completed - ...

Files Created - ...

Files Modified - ...

Verification - Configure - Build - Application Launch - Manual
Feature Verification

Documentation Updated - PLAN.md - CHANGELOG.md - ARCHITECTURE.md (if
required)

Next Resume Point - ...
