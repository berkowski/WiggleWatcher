cmake_minimum_required(VERSION 3.16)

project(maggui
    VERSION 0.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

include(CTest)
add_subdirectory(core)
add_subdirectory(app)
add_subdirectory(utils)
