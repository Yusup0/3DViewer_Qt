cmake_minimum_required(VERSION 3.5)

project(3DViewer LANGUAGES CXX C)
set(OUT_NAME 3DViewer)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libBin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libBin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core OpenGLWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core OpenGLWidgets REQUIRED)
find_package(GifImages)

cmake_host_system_information(RESULT PRETTY_NAME QUERY OS_NAME)
message(STATUS "${PRETTY_NAME}")


if (${PRETTY_NAME} STREQUAL "Linux") 
    add_compile_definitions(__LINUX__)
else()
    add_compile_definitions(__MACOS__)
endif()


# set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -ggdb")

set(SOURCES
        srcs/mainwindow.cpp
        srcs/mywidgetopengl.cpp
        srcs/mainwindow.ui
        ../../code_c/logging_manager.c
        ../../code_c/functional.c
)

set(HEADERS
        includes/mywidgetopengl.hpp
        includes/mainwindow.hpp
        ../../code_c/logging_manager.h
        ../../code_c/functional.h
)

add_library(${OUT_NAME} SHARED ${SOURCES} ${HEADERS})
# add_subdirectory(tests)

set_target_properties(${OUT_NAME}
    PROPERTIES
    CMAKE_CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
)

target_include_directories(3DViewer
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/includes
  ${CMAKE_CURRENT_LIST_DIR}/../../code_c
  ${CMAKE_CURRENT_LIST_DIR}/objects
)

target_link_libraries(3DViewer
  PRIVATE
  # Qt${QT_VERSION_MAJOR}::Widgets
  # Qt${QT_VERSION_MAJOR}::OpenGL
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::OpenGLWidgets
  GifImages
)
