find_package(Qt6 REQUIRED COMPONENTS Core SerialPort Network)
qt_standard_project_setup()

qt_add_library(maggui-core)

target_sources(maggui-core PRIVATE
    types/vectormagnetometerdata.h
    types/vectormagnetometerdata.cpp
    # dslmagnetometersink.h
    # dslmagnetometersink.cpp
    sensorbase.h
    sensorbase.cpp
    textfilesink.h
    textfilesink.cpp
    iofactory.h
    iofactory.cpp
    tcpsocketfactory.h
    tcpsocketfactory.cpp
    udpsocketfactory.h
    udpsocketfactory.cpp
    serialportfactory.cpp
    serialportfactory.h
    magnetometer.h
    magnetometer.cpp
    aps1540magnetometer.h
    aps1540magnetometer.cpp
)

target_link_libraries(maggui-core
    PUBLIC Qt::Core Qt::Network Qt::SerialPort)

if (BUILD_TESTING)
  add_subdirectory("test")
endif ()
