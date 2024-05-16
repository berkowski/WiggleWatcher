find_package(Qt6 REQUIRED COMPONENTS Network SerialPort Widgets)

qt_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    logcontrolwidget.ui
    logcontrolwidget.h
    logcontrolwidget.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}::Core Qt::Widgets)
