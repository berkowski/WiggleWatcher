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
    #iopool.h
    #iopool.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}::Core qcustomplot::qcustomplot Qt::Widgets Qt::Gui)
target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# if(APPLE)
#     set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
#     get_target_property(qmake_executable Qt::qmake IMPORTED_LOCATION)
#     get_filename_component(qt_bin_directory "${qmake_executable}" DIRECTORY)
#     find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${qt_bin_directory}" REQUIRED)
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#             COMMAND ${MACDEPLOYQT_EXECUTABLE} ${PROJECT_NAME}.app -dmg
#             WORKING_DIRECTORY ${CURRENT_CMAKE_BINARY_DIR}
#             COMMENT "Run macdeployqt to create a macOS bundle"
#     )
# endif()

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
  set(DEPLOY_TOOL_OPTIONS "-dmg")
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
    DEPLOY_TOOL_OPTIONS ${DEPLOY_TOOL_OPTIONS}
    # Fedora installs Qt libraries into /lib64, which is excluded due to QT_DEPLOY_IGNORED_LIB_DIRS
    # POST_INCLUDE_REGEXES here will make sure at at least the Qt libraries are grabbed.
    POST_INCLUDE_REGEXES "libQt6.*"
)


install(SCRIPT ${deploy_script})

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Maples magnetometer logging GUI")
set(CPACK_VENDOR "Woods Hole Oceanographic Institution")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_VERBATIM_VARIABLES ON)
set(CPACK_PACKAGING_INSTALL_PREFIX "/${PROJECT_NAME}")

include(CPack)
