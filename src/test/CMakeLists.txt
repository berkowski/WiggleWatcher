find_package(Qt6 REQUIRED COMPONENTS Test)

# qt_add_executable(test_qdslfilesink test_qdslfilesink.cpp)
# add_test(NAME test_qdslfilesink COMMAND test_qdslfilesink)
# target_link_libraries(test_qdslfilesink PRIVATE maggui-core Qt::Test)
# target_include_directories(test_qdslfilesink PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
# target_include_directories(test_qdslfilesink PRIVATE $<TARGET_PROPERTY:maggui-core,INCLUDE_DIRECTORIES>)

#qt_add_executable(tst_dslmagnetometersink tst_dslmagnetometersink.cpp)
#add_test(NAME tst_dslmagnetometersink COMMAND tst_dslmagnetometersink)
#target_link_libraries(tst_dslmagnetometersink PRIVATE maggui-core Qt::Test)
#target_include_directories(tst_dslmagnetometersink PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

qt_add_executable(tst_tcpsocketfactory tst_tcpsocketfactory.cpp)
add_test(NAME tst_tcpsocketfactory COMMAND tst_tcpsocketfactory)
target_link_libraries(tst_tcpsocketfactory PRIVATE maggui-core Qt::Test)
target_include_directories(tst_tcpsocketfactory PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

qt_add_executable(tst_udpsocketfactory tst_udpsocketfactory.cpp)
add_test(NAME tst_udpsocketfactory COMMAND tst_udpsocketfactory)
target_link_libraries(tst_udpsocketfactory PRIVATE maggui-core Qt::Test)
target_include_directories(tst_udpsocketfactory PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

qt_add_executable(tst_serialportfactory tst_serialportfactory.cpp)
add_test(NAME tst_serialportfactory COMMAND tst_serialportfactory)
target_link_libraries(tst_serialportfactory PRIVATE maggui-core Qt::Test)
target_include_directories(tst_serialportfactory PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)