find_package(Qt6 REQUIRED COMPONENTS Core SerialPort Network)

qt_add_library(core STATIC)
add_library(${PROJECT_NAME}::Core ALIAS core)

set(core-sources-public
    include/core/vectormagnetometerdata.h
    include/core/sensorbase.h
    include/core/textfilesink.h
    include/core/iofactory.h
    include/core/magnetometer.h
    include/core/aps1540magnetometer.h
    include/core/global.h
    include/core/filetimer.h
)

set(core-sources-private
    vectormagnetometerdata.cpp
    sensorbase.cpp
    textfilesink.cpp
    iofactory.cpp
    tcpsocketfactory.h
    tcpsocketfactory.cpp
    udpsocketfactory.h
    udpsocketfactory.cpp
    serialportfactory.cpp
    serialportfactory.h
    magnetometer.cpp
    aps1540magnetometer.cpp
    filetimer.cpp
)

target_sources(core
    PUBLIC ${core-sources-public}
    PRIVATE ${core-sources-private}
)

target_include_directories(core
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)

target_compile_definitions(core PRIVATE MAGGUI_CORE_LIBRARY)

target_link_libraries(core
    PUBLIC Qt::Core Qt::Network Qt::SerialPort)

if (BUILD_TESTING)
  add_subdirectory("test")
endif ()
