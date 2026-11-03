# ADR 0002: Use C++17 and Qt 5 / QML Stack

## Status
Accepted

## Context
The application requires high-performance execution of scheduling algorithms, precise memory management, and a fluid, responsive graphical user interface with custom animations and data visualization (Gantt charts, tables).

## Decision
* **C++17**: Chosen as the core programming language for its robust standard library, performance, strict typing, and extensive support for unit testing via GoogleTest.
* **Qt 5 (Core, GUI, QML)**: Selected for cross-platform desktop development (Windows and Linux) and its declarative UI capabilities (QML), allowing smooth bindings between native C++ models (`QAbstractListModel`) and visual components.

## Consequences
* High execution performance and low memory footprint for simulation loops.
* Clean separation between pure C++ core logic and declarative QML user interfaces.