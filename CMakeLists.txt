cmake_minimum_required(VERSION 3.10)

project(Andeme)

set(THIRDPARTY_PATH ${CMAKE_SOURCE_DIR}/thirdparty)

set(QT_PATH ${THIRDPARTY_PATH}/qt5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED PATHS ${THIRDPARTY_PATH}/qt5)

# set the project name
add_definitions(-std=c++17 -fPIC)

# AndemeQtGui
file(GLOB ANDEME_QT_GUI_SRCS qtgui/*.cpp)

add_executable(AndemeQtGui ${ANDEME_QT_GUI_SRCS})
target_link_libraries(AndemeQtGui Qt5::Widgets)

