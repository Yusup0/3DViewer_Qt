cmake_minimum_required(VERSION 3.5)

project(3DV LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -ggdb")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core OpenGLWidgets OpenGL Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core OpenGLWidgets OpenGL Gui REQUIRED)
find_package(3DViewer)
find_package(GifImages)

include_directories(./lib/QtGifImage/src/gifimage/)
file(GLOB_RECURSE QTPROPERTY_BROWSER_SOURCES
./lib/QtGifImage/src/gifimage/qtgifimage.pri)

set(PROJECT_SOURCES
        srcs/main.cpp
        ${QTPROPERTY_BROWSER_SOURCES}
)

set(PROJECT_HEADERS
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3DV
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
else()
    add_executable(3DV
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        )
endif()

add_subdirectory(lib)

target_include_directories(3DV
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/includes
  ${CMAKE_CURRENT_LIST_DIR}/code_c
  ${CMAKE_CURRENT_LIST_DIR}/objects
  ${CMAKE_CURRENT_LIST_DIR}/libBin
)

target_link_libraries(3DV
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::OpenGLWidgets
  Qt${QT_VERSION_MAJOR}::OpenGL
  3DViewer
  GifImages
)
