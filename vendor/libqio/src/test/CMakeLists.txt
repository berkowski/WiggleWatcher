find_package(Qt6 REQUIRED COMPONENTS Test)

macro(add_qio_test name)
  qt_add_executable(tst_${name} tst_${name}.cpp)
  add_test(NAME tst_${name} COMMAND tst_${name})
  target_link_libraries(tst_${name} PRIVATE ${PROJECT_NAME}::libqio Qt::Test)
endmacro()

add_qio_test(tcpsocketfactory)
add_qio_test(udpsocketfactory)
add_qio_test(serialportfactory)

