cmake_minimum_required(VERSION 3.16)
project(Algorithm_Visualizer VERSION 0.1 LANGUAGES CXX)

# Global settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
# python setup
set(EMBEDDED_PYTHON_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/python")
set(Python3_ROOT_DIR "${EMBEDDED_PYTHON_DIR}")
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
message(STATUS "Python3_VERSION: ${Python3_VERSION}")
message(STATUS "Python3_EXECUTABLE: ${Python3_EXECUTABLE}")

# Include directories
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set UI search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})

# Add subdirectories
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(resource)
add_subdirectory(test)
add_subdirectory(googletest)

add_executable(google_unit_test
    ${TEST_SOURCES}
    ${SRC_FILES}
    ${INCLUDE_FILES}
    mainwindow.ui
    helpFile.qrc
)

target_link_libraries(google_unit_test
    gtest
    gtest_main
    pthread
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
    ${Python3_LIBRARIES}
)

enable_testing()
add_test(NAME google_unit_tests COMMAND google_unit_test)

# Main executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Algorithm_Visualizer
        MANUAL_FINALIZATION
        main.cpp
        mainwindow.ui
        ${SRC_FILES}
        ${INCLUDE_FILES}
        ${RESOURCE_FILES}
        helpFile.qrc
    )
else()
    if(ANDROID)
        add_library(Algorithm_Visualizer SHARED
            main.cpp
            mainwindow.ui
            ${SRC_FILES}
            ${INCLUDE_FILES}
            ${RESOURCE_FILES}
        )
    else()
        add_executable(Algorithm_Visualizer
            main.cpp
            mainwindow.ui
            ${SRC_FILES}
            ${INCLUDE_FILES}
            ${RESOURCE_FILES}
        )
    endif()
endif()

target_link_libraries(Algorithm_Visualizer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Python3_LIBRARIES})
# Bundle properties
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Algorithm_Visualizer)
endif()

set_target_properties(Algorithm_Visualizer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Installation
include(GNUInstallDirs)
install(TARGETS Algorithm_Visualizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#qt_deploy_runtime_libraries(Algorithm_Visualizer)
add_custom_command(TARGET Algorithm_Visualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${EMBEDDED_PYTHON_DIR}"
        "$<TARGET_FILE_DIR:Algorithm_Visualizer>/python"
    COMMENT "Bundling embedded Python..."
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Algorithm_Visualizer)
endif()
