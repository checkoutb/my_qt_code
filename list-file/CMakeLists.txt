cmake_minimum_required(VERSION 3.19)
project(list-file LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(list-file
    WIN32 MACOSX_BUNDLE
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    ckb_myfileutil.h ckb_myfileutil.cpp
)

target_link_libraries(list-file
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS list-file
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET list-file
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
