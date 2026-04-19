cmake_minimum_required(VERSION 3.16)
project(TeachingMonitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.6 REQUIRED COMPONENTS Widgets Sql)

qt_add_executable(TeachingMonitor
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
)

target_link_libraries(TeachingMonitor PRIVATE Qt6::Widgets Qt6::Sql)
