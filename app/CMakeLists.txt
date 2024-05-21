find_package(Qt6 REQUIRED COMPONENTS Network SerialPort Widgets)

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
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}::Core qcustomplot::qcustomplot Qt::Widgets)
