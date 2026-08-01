# BCClone Architecture

Status: Living Document

## Layers

Presentation (Qt) ↓ Application ↓ Core ↓ Utilities

Lower layers never depend on upper layers.

## Responsibilities

### Presentation

UI only.

### Application

Coordinates workflows.

### Core

Comparison engines and business logic.

### Utilities

Logging, settings, helpers.

## Modules

-   app/
-   core/
-   compare/
-   folder/
-   binary/
-   merge/
-   models/
-   ui/
-   utils/

## Design Principles

-   Composition over inheritance
-   RAII
-   Small interfaces
-   No UI logic in Core
-   Value semantics where practical

## Threading

UI thread: - Rendering - Input

Workers: - File loading - Folder scan - Hashing - Diff computation

## Verification

This project uses no automated testing framework. Every change is
verified manually:

-   Successful configuration
-   Successful build
-   Successful application launch
-   Manual verification of the implemented feature

## Architectural Decision Log

Record only architecture changes here.
