# CLAUDE.md

## Project

**Project:** BCClone

Build a modern Beyond Compare alternative using **C++20**, **Qt6
Widgets**, and **CMake**.

## Technology

-   Language: C++20
-   UI: Qt6 Widgets
-   Build: CMake
-   Verification: Manual (no automated testing framework)
-   Version Control: Git

## Engineering Philosophy (CAVEMAN)

### C --- Correct

Correctness over cleverness.

### A --- Architecture

Keep architecture simple. Prefer composition over inheritance.

### V --- Verify

Every task must: - Configure successfully - Build successfully -
Introduce no compiler warnings - Launch successfully (where applicable) -
Be manually verified against its intended behavior

### E --- Explain

Explain what changed, why, and any future improvements.

### M --- Minimal

Do not over-engineer.

Avoid: - Unnecessary factories - DI frameworks - Service locators - Deep
inheritance - Generic abstractions without clear value

### A --- Atomic

Every change must compile and be independently verifiable.

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

### Step 1 (MANDATORY, before every coding session)

Read in order: 1. CLAUDE.md 2. PLAN.md 3. docs/ARCHITECTURE.md 4.
docs/CHANGELOG.md 5. docs/SPECIFICATION.md

### Step 2

Resume exactly from the latest completed task recorded in
docs/CHANGELOG.md. Verify docs/CHANGELOG.md and PLAN.md are
synchronized. If they differ, explain and synchronize them before
coding.

Never restart completed work. Never skip unfinished work.

### Step 3

Implement only the current planned task. Do not start future phases.

### Step 4 (MANDATORY, after every completed task)

-   Build the project
-   Ensure no compiler warnings are introduced
-   Launch the application (when applicable)
-   Manually verify the implemented feature

### Step 5 (MANDATORY, after every completed task)

Update: - PLAN.md - docs/CHANGELOG.md

Update docs/ARCHITECTURE.md only when architectural decisions change.

### Step 6

Provide: - Files created - Files modified - Verification status -
Documentation updated - Next task (within current phase only)
