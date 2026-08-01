# CHANGELOG

This file is the mandatory resume point for every Claude Code session.

## Unreleased

### Current Status

Phase 0 (project foundation) complete: CMake + Qt6 configure and
build cleanly, the app launches, and MainWindow, Logger, and
SettingsManager are wired up.

Next Resume Point: - Start Phase 1 (Text Compare Engine) - Read
CLAUDE.md - Verify PLAN.md - Begin file loading / encoding detection

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
