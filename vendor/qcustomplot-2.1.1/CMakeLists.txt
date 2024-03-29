find_package(Qt6 REQUIRED COMPONENTS Core PrintSupport)
# QCustomPlot doesn't ship with a CMakeLists.txt
qt_add_library(qcustomplot STATIC)
add_library(qcustomplot::qcustomplot ALIAS qcustomplot)
target_sources(qcustomplot
        PUBLIC qcustomplot.h
        PRIVATE qcustomplot.cpp
)

target_include_directories(qcustomplot
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOUR_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

target_link_libraries(qcustomplot PUBLIC Qt6::Core Qt6::PrintSupport)