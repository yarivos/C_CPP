cmake_minimum_required(VERSION 3.21)
project(ex1_yarivyar C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(ex1_yarivyar
        cipher.c
        cipher.h
        main.c
        tests.c
        tests.h)
