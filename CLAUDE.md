# CLAUDE.md

## Project

**Project:** BCClone

Build a modern Beyond Compare alternative using **C++20**, **Qt6
Widgets**, and **CMake**.

## Technology

-   Language: C++20
-   UI: Qt6 Widgets
-   Build: CMake
-   Testing: GoogleTest
-   Version Control: Git

## Engineering Philosophy (CAVEMAN)

### C --- Correct

Correctness over cleverness.

### A --- Architecture

Keep architecture simple. Prefer composition over inheritance.

### V --- Verify

Every task must: - Configure successfully - Build successfully -
Introduce no compiler warnings - Pass tests - Launch successfully (where
applicable)

### E --- Explain

Explain what changed, why, and any future improvements.

### M --- Minimal

Do not over-engineer.

Avoid: - Unnecessary factories - DI frameworks - Service locators - Deep
inheritance - Generic abstractions without clear value

### A --- Atomic

Every change must compile and be independently testable.

### N --- Neat

Readable, consistent code.

------------------------------------------------------------------------

## Rules

Always: - Use RAII - Use smart pointers - Use std::filesystem - Use enum
class - Use constexpr - Prefer const correctness - Keep functions small

Never: - Break existing functionality - Rename files unnecessarily -
Ignore compiler warnings - Leave unexplained TODOs

------------------------------------------------------------------------

## Layer Dependency

UI ↓ Application ↓ Core ↓ Utilities

Never reverse dependency direction.

------------------------------------------------------------------------

## Session Workflow (MANDATORY)

### Step 1

Read in order: 1. CLAUDE.md 2. PLAN.md 3. ARCHITECTURE.md 4.
CHANGELOG.md

### Step 2

Resume from the latest CHANGELOG.md entry. Verify it matches PLAN.md. If
they differ, explain and synchronize them before coding.

### Step 3

Implement only the current planned task. Do not start future phases.

### Step 4

Verify: - Configure - Build - Tests - Application launch

### Step 5 (MANDATORY)

Update: - PLAN.md - CHANGELOG.md

Update ARCHITECTURE.md only when architectural decisions change.

### Step 6

Provide: - Files created - Files modified - Verification status -
Documentation updated - Next task (within current phase only)
