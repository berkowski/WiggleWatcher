find_package(Qt6 REQUIRED COMPONENTS Gui Network SerialPort Widgets)

qt_add_executable(${PROJECT_NAME})

configure_file(${PROJECT_SOURCE_DIR}/reference/APS1540_manual.pdf ${CMAKE_CURRENT_SOURCE_DIR} COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/reference/SmartDigitalMagnetometerHMR2300_ds.pdf ${CMAKE_CURRENT_SOURCE_DIR} COPYONLY)
qt_add_resources(${PROJECT_NAME} documents
    PREFIX "/documents"
    FILES APS1540_manual.pdf SmartDigitalMagnetometerHMR2300_ds.pdf
)

target_sources(${PROJECT_NAME}
    PRIVATE
    aboutdialog.ui
    aboutdialog.h
    aboutdialog.cpp
    state.h
    state.cpp
    logcontrolwidget.ui
    logcontrolwidget.h
    logcontrolwidget.cpp
    maggieplotwidget.h
    maggieplotwidget.cpp
    maggieplot.h
    maggieplot.cpp
    centralwidget.h
    centralwidget.cpp
    mainwindow.ui
    mainwindow.h
    mainwindow.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}::Core qcustomplot::qcustomplot Qt::Widgets Qt::Gui)
target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
endif()

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()


install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    NO_TRANSLATIONS
    # Fedora installs Qt libraries into /lib64, which is excluded due to QT_DEPLOY_IGNORED_LIB_DIRS
    # POST_INCLUDE_REGEXES here will make sure at at least the Qt libraries are grabbed.
    POST_INCLUDE_REGEXES "libQt6.*"
)


install(SCRIPT ${deploy_script})

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
string(TOLOWER "${CPACK_PACKAGE_NAME}-${CMAKE_PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}" CPACK_PACKAGE_FILE_NAME)
set(CPACK_VERBATIM_VARIABLES ON)
if(NOT APPLE)
    set(CPACK_PACKAGING_INSTALL_PREFIX "/${PROJECT_NAME}")
endif()
cmake_path(RELATIVE_PATH CMAKE_BINARY_DIR BASE_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE RELATIVE_BINARY_DIR)
set(CPACK_SOURCE_IGNORE_FILES /${RELATIVE_BINARY_DIR}/;/\\.cache/;/\\.git/)
include(CPack)
