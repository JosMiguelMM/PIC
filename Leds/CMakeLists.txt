cmake_minimum_required(VERSION 3.0)
project(LED)

set(CMAKE_C_COMPILER "C:/Program Files (x86)/Microchip/xc8/v2.31/bin/xc8.exe")
set(CMAKE_C_FLAGS "-Ipath/to/18F4550.h -Os -maddrqual=16 -mstack=compiled:auto")
set(SOURCE_FILES main.c)

add_executable(LED ${SOURCE_FILES})
