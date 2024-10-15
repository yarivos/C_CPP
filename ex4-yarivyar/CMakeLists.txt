cmake_minimum_required(VERSION 3.16)
project(ex4_yarivyar)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)


add_executable(ex4_yarivyar
        Activation.h
        Dense.h
        main.cpp
        Matrix.h
        MlpNetwork.h Matrix.cpp Dense.cpp Activation.cpp MlpNetwork.cpp)
