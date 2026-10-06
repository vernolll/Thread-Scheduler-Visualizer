cmake_minimum_required(VERSION 3.16)
project(thread_scheduler_visualizer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick Widgets)

add_library(core_lib STATIC
    include/gui/GUIController.h
    src/gui/GUIController.cpp
    src/core/PCB.cpp
    src/core/Thread.cpp
    src/core/FCFSScheduler.cpp
    src/core/RRScheduler.cpp
)

target_include_directories(core_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(core_lib PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
)

add_executable(thread_scheduler_visualizer
    src/main.cpp
    src/qml.qrc
)

target_link_libraries(thread_scheduler_visualizer PRIVATE
    core_lib
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
)

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(tsv_core_tests
        tests/unit/test_fcfs_scheduler.cpp
        tests/unit/test_rr_scheduler.cpp
    )

    target_link_libraries(tsv_core_tests PRIVATE
        core_lib
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(tsv_core_tests)
endif()