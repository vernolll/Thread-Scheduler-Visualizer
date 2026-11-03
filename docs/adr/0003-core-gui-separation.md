# ADR 0003: Strict Separation of Core Engine and GUI Layer

## Status
Accepted

## Context
Unit testing scheduling algorithms (FCFS, RR, MLFQ, etc.) should not depend on graphical components or window frameworks. Furthermore, the UI needs to be fully reactive to simulation updates.

## Decision
* The project architecture is strictly divided into two layers:
  1. **Core Engine (`include/core/`, `src/core/`)**: Pure C++ logic, completely decoupled from Qt widgets or QML. Contains scheduling policies, thread states, and metrics calculators.
  2. **GUI Layer (`include/gui/`, `src/gui/`, `src/qml/`)**: Qt/QML viewmodels and data models extending `QAbstractListModel` that bridge core events to the user interface.

## Consequences
* Core logic can be comprehensively tested via automated unit tests (CTest/GoogleTest) without initializing a GUI context.
* UI modifications do not affect scheduling mathematics or data structures.