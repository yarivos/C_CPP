cmake_minimum_required(VERSION 3.16)
project(ex6_yarivyar)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
add_executable(ex6_yarivyar
        test_ex6.cpp
#        Presubmit.cpp
#        Presubmit.hpp
#        Helpers.h
#        main.cpp
        Dictionary.hpp
        HashMap.hpp)
#
#Presubmit.cpp
#Presubmit.hpp
#Helpers.h