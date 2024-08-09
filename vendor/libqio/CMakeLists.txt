cmake_minimum_required(VERSION 3.25)

include(GenerateExportHeader)

project(libqio
    VERSION 0.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

find_package(Qt6 REQUIRED COMPONENTS Core SerialPort Network)
qt_standard_project_setup()

qt_add_library(${PROJECT_NAME} STATIC)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
add_library(libqio::libqio ALIAS ${PROJECT_NAME})

generate_export_header(libqio
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/libqio/export.h
)


set(sources-public
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/libqio/export.h>
    include/libqio/iofactory.h
)

set(sources-private
    src/iofactory.cpp
    src/tcpsocketfactory.h
    src/tcpsocketfactory.cpp
    src/udpsocketfactory.h
    src/udpsocketfactory.cpp
    src/serialportfactory.cpp
    src/serialportfactory.h
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${sources-public}
        ${sources-private}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)


target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt::Core Qt::Network Qt::SerialPort
) 

if (BUILD_TESTING)
  include(CTest)
  add_subdirectory("src/test")
endif ()
