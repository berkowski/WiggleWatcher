find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

qt_add_library(maggui-core)

target_sources(maggui-core PRIVATE
    # qabstractsink.hpp
    # qabstractfilesink.hpp
    # qabstractfilesink.cpp
    # qabstractsensor.hpp
    # qtextfilesink.hpp
    # qtextfilesink.cpp
    # qdslfilesink.hpp
    # qdslfilesink.cpp
    # qrotatingfilesink.hpp
    types/vectormagnetometerdata.h
    metadatainterface.h
    sensorinterface.h
    sinkinterface.h
    dslmagnetometersink.h
    dslmagnetometersink.cpp
)

target_link_libraries(maggui-core
    PUBLIC Qt::Core)

if (BUILD_TESTING)
  add_subdirectory("test")
endif ()
