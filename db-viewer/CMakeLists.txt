cmake_minimum_required(VERSION 3.19)
project(db-viewer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(db-viewer
    WIN32 MACOSX_BUNDLE
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    db.properties
    ckb_mydbutil.h ckb_mydbutil.cpp   ##############
)


######################
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/db.properties
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(db-viewer
    PRIVATE
        Qt::Core
        Qt::Widgets
        mysqlcppconnx       ######################
)

add_definitions(-DMY_QT_FLAG)     ##################

include(GNUInstallDirs)

install(TARGETS db-viewer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET db-viewer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
