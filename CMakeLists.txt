cmake_minimum_required(VERSION 3.25)

project(wigglewatcher
    VERSION 1.0.0
    LANGUAGES CXX)
# Set this when making release candidates (can't use alphanumeric 'tweak' values in VERSION statement above)
set(PROJECT_VERSION_SUFFIX "-rc4")

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
add_subdirectory(vendor/erbsland-qt-toml-1.2.2)
add_subdirectory(vendor/qcustomplot-2.1.1)
add_subdirectory(vendor/libqio)
