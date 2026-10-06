cmake_minimum_required(VERSION 3.16)

project(ThreadSchedulerVisualizer
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Quick
)

add_library(core_lib
    src/core/Thread.cpp
    src/core/PCB.cpp
)

target_include_directories(core_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(thread_scheduler_visualizer
    src/main.cpp
    src/core/PCB.cpp
    src/core/Thread.cpp
    src/core/FCFSScheduler.cpp
    src/core/RRScheduler.cpp
)

target_link_libraries(thread_scheduler_visualizer PRIVATE core_lib)

option(BUILD_TESTING "Build the testing suite" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()