cmake_minimum_required(VERSION 3.21)
project(ex2_yarivyar C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(ex2_yarivyar
        sort_bus_lines.c
        sort_bus_lines.h
        main.c
        test_bus_lines.c
        test_bus_lines.h)
