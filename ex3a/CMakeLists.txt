cmake_minimum_required(VERSION 3.21)
project(ex3a_yarivyar C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(ex3a_yarivyar
        linked_list.c
        linked_list.h
        markov_chain.c
        markov_chain.h tweets_generator.c)
