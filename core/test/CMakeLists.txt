find_package(Qt6 REQUIRED COMPONENTS Test)

macro(add_core_test name)
  qt_add_executable(tst_${name} tst_${name}.cpp)
  add_test(NAME tst_${name} COMMAND tst_${name})
  target_link_libraries(tst_${name} PRIVATE ${PROJECT_NAME}::Core Qt::Test)
endmacro()

add_core_test(tcpsocketfactory)
add_core_test(udpsocketfactory)
add_core_test(serialportfactory)
add_core_test(aps1540magnetometer)
add_core_test(sensorbase)