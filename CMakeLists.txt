cmake_minimum_required(VERSION 3.16)
project(NetworkDesigner VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Prefer Qt6, fall back to Qt5
find_package(Qt6 QUIET COMPONENTS Core Gui Widgets Xml)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    set(QT_LIBS Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Xml)
    message(STATUS "Using Qt6")
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Xml)
    set(QT_VERSION_MAJOR 5)
    set(QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml)
    message(STATUS "Using Qt5")
endif()

add_subdirectory(src)

# Tests are not built for Android (no GTest port)
if(NOT ANDROID)
    enable_testing()
    add_subdirectory(tests)
endif()

# Android: point androiddeployqt at our custom manifest
if(ANDROID)
    set_target_properties(NetworkDesigner PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android"
    )
endif()

# CPack installer (Windows only)
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_NAME "NetworkDesigner")
    set(CPACK_PACKAGE_VENDOR "NetworkDesigner")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Discrete-state neural network design tool")
    set(CPACK_NSIS_DISPLAY_NAME "NetworkDesigner")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    include(CPack)
endif()
