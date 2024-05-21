find_package(Qt6 REQUIRED COMPONENTS Gui Network SerialPort Widgets)

configure_file(${PROJECT_SOURCE_DIR}/reference/APS1540_manual.pdf . COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/reference/SmartDigitalMagnetometerHMR2300_ds.pdf . COPYONLY)
configure_file(manuals.qrc.in manuals.qrc COPYONLY)

qt_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    state.h
    state.cpp
    logcontrolwidget.ui
    logcontrolwidget.h
    logcontrolwidget.cpp
    maggieplotwidget.h
    maggieplotwidget.cpp
    centralwidget.h
    centralwidget.cpp
    mainwindow.ui
    mainwindow.h
    mainwindow.cpp
    main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/manuals.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}::Core qcustomplot::qcustomplot Qt::Widgets Qt::Gui)
target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)