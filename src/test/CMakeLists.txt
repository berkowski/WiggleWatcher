find_package(Qt6 REQUIRED COMPONENTS Test)

# qt_add_executable(test_qdslfilesink test_qdslfilesink.cpp)
# add_test(NAME test_qdslfilesink COMMAND test_qdslfilesink)
# target_link_libraries(test_qdslfilesink PRIVATE maggui-core Qt::Test)
# target_include_directories(test_qdslfilesink PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
# target_include_directories(test_qdslfilesink PRIVATE $<TARGET_PROPERTY:maggui-core,INCLUDE_DIRECTORIES>)

qt_add_executable(tst_dslmagnetometersink tst_dslmagnetometersink.cpp)
add_test(NAME tst_dslmagnetometersink COMMAND tst_dslmagnetometersink)
target_link_libraries(tst_dslmagnetometersink PRIVATE maggui-core Qt::Test)
target_include_directories(tst_dslmagnetometersink PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
