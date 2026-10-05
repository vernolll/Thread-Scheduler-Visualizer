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

option(BUILD_TESTING "Build the testing suite" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()