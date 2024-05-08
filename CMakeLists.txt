# 20240507 1.0 Fennmai

cmake_minimum_required(VERSION 3.5)

project(PathPlanning_Platform VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# Configure output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/data/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/data/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/data/archive)
set(CMAKE_OBJECTS_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/data/objects)

# Configure cpp & hpp
file(GLOB_RECURSE qt_file_cpp "${PROJECT_SOURCE_DIR}/src/qt/*.cpp")
file(GLOB_RECURSE qt_file_hpp "${PROJECT_SOURCE_DIR}/include/qt/*.h")
file(GLOB_RECURSE algorithm_file_cpp "${PROJECT_SOURCE_DIR}/src/algorithm/*.cpp")
file(GLOB_RECURSE algorithm_file_hpp "${PROJECT_SOURCE_DIR}/include/algorithm/*.h")
file(GLOB_RECURSE utils_file_cpp "${PROJECT_SOURCE_DIR}/src/utils/*.cpp")
file(GLOB_RECURSE utils_file_hpp "${PROJECT_SOURCE_DIR}/include/utils/*.h")

# Setting
set(PROJECT_SOURCES
        src/main.cpp
        src/qt/pathplanning_platform.ui
        ${qt_file_cpp}
        ${qt_file_hpp}
        ${algorithm_file_cpp}
        ${algorithm_file_hpp}
        ${utils_file_cpp}
        ${utils_file_hpp}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PathPlanning_Platform
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PathPlanning_Platform APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PathPlanning_Platform SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PathPlanning_Platform
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# source dir
# hpp
include_directories(${PROJECT_SOURCE_DIR}/include/qt)
include_directories(${PROJECT_SOURCE_DIR}/include/algorithm)
# cpp
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/qt)
include_directories(${PROJECT_SOURCE_DIR}/src/algorithm)
# qcustomplot
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)
target_link_libraries(PathPlanning_Platform PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)


target_link_libraries(PathPlanning_Platform PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(PathPlanning_Platform PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PathPlanning_Platform)
endif()
