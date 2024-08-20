include(GenerateExportHeader)

find_package(Qt6 REQUIRED COMPONENTS Core SerialPort Network)

qt_add_library(core STATIC)
add_library(${PROJECT_NAME}::Core ALIAS core)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/core)
generate_export_header(core
    PREFIX_NAME WIGGLEWATCHER_
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/core/export.h
)

set(core-sources-public
    include/core/global.h
    include/core/vectormagnetometerdata.h
    include/core/sensorbase.h
    include/core/textfilesink.h
    include/core/magnetometer.h
    include/core/aps1540magnetometer.h
    include/core/filetimer.h
    include/core/settings.h
    include/core/sensorbase.h
    include/core/sensorfactory.h
    include/core/sensorlogger.h
    include/core/magnetometerlogger.h
)

set(core-sources-private
    vectormagnetometerdata.cpp
    sensorbase.cpp
    textfilesink.cpp
    magnetometer.cpp
    aps1540magnetometer.cpp
    filetimer.cpp
    settings.cpp
    sensorfactory.cpp
    sensorlogger.cpp
    magnetometerlogger.cpp
    global.cpp
)

target_sources(core
    PRIVATE
        ${core-sources-public}
        ${core-sources-private}
)

target_include_directories(core
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)


target_link_libraries(core
    PUBLIC Qt::Core Qt::Network Qt::SerialPort libqio::libqio
    PRIVATE erbsland-qt-toml
)

if (BUILD_TESTING)
  add_subdirectory("test")
endif ()
