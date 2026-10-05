# Thread Scheduler Visualizer

A C++ simulator and visualization tool for operating system thread scheduling algorithms. Designed to model, simulate, and analyze CPU scheduling mechanics, context switches, and performance metrics in real time.

---

## Key Features

* **Algorithm Simulation**: Implements core scheduling logic (FCFS, SJF, Round Robin, Priority Scheduling).
* **Metrics & Analytics**: Tracks Turnaround Time, Waiting Time, Response Time, and CPU Utilization.
* **Cross-Platform**: Built with C++17 and Qt, supporting both Windows and Linux target environments.
* **Containerized Development**: Docker environment configured for reproducible Linux builds.

---

## Tech Stack

* **Language**: C++17
* **Framework**: Qt 5 / Qt 6
* **Build System**: CMake (>= 3.20), Ninja
* **CI/CD & Dev Environments**: GitHub Actions, Docker, MSVC / GCC 11+

---

## Project Structure

```text
thread-scheduler-visualizer/
├── .github/
│   └── workflows/
│       ├── ci.yml            # GitHub Actions CI pipeline
│       └── readme-check.yml  # Auto-check README updates on structure changes
├── docker/
│   └── Dockerfile            # Linux build environment container
├── include/
│   └── core/
│       ├── PCB.h             # Process Control Block class interface
│       └── Thread.h          # Thread structure & ThreadState enum
├── src/
│   ├── core/
│   │   ├── PCB.cpp           # PCB implementation
│   │   └── Thread.cpp        # Thread helpers implementation
│   └── main.cpp              # Entry point / test runner
├── CMakeLists.txt            # Root CMake build configuration
├── CMakePresets.json         # Standard CMake build presets
├── LICENSE                   # License file
└── README.md                 # Project documentation
