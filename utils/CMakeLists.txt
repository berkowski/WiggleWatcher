find_package(Qt6 REQUIRED COMPONENTS Core SerialPort Network)
qt_add_executable(fake-sensor)
target_sources(fake-sensor PRIVATE
    fake-sensor.cpp
)
target_link_libraries(fake-sensor PRIVATE Qt::Core maggui::core)