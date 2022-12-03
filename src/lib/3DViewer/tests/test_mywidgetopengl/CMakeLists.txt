cmake_minimum_required(VERSION 3.5)

project(TestMyWidgetOpenGL LANGUAGES CXX C)
set(OUT_NAME testMyWidgetOpenGL)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_compile_definitions(FOR_TEST)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror")

set(3DVIEWER_HPP ${CMAKE_CURRENT_SOURCE_DIR}/../../includes)

# add_definitions(FOR_TEST)

set(HEADERS
    qrc/objects_test.qrc
    ../../includes/mywidgetopengl.hpp
    ../../../../code_c/s21_read_object.h
    ../../../../code_c/logging_manager.h
    ../../../../code_c/functional.h
    includes/test_mywidgetopengl.hpp
)

set(SOURCES
    ../../srcs/mywidgetopengl.cpp
    ../../../../code_c/s21_read_object.c
    ../../../../code_c/logging_manager.c
    ../../../../code_c/functional.c
    srcs/test_mywidgetopengl.cpp
)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test OpenGLWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test OpenGLWidgets REQUIRED)
#find_package(3DViewer)

add_executable(${OUT_NAME} ${SOURCES} ${HEADERS})

add_test(NAME ${OUT_NAME} COMMAND ${OUT_NAME})

enable_testing()

set_target_properties(${OUT_NAME}
    PROPERTIES
    CMAKE_CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
)

target_include_directories(${OUT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../includes
    ${CMAKE_CURRENT_LIST_DIR}/../../../../code_c
    ${3DVIEWER_HPP}
)

target_link_libraries(${OUT_NAME}
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)

