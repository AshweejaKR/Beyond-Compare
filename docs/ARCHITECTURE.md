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

## Testing

Unit test all core modules.

## Architectural Decision Log

Record only architecture changes here.
