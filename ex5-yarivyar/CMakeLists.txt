cmake_minimum_required(VERSION 3.16)
project(ex5_yarivyar)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
add_executable(ex5_yarivyar
        presubmit.cpp
        Movie.cpp
        Movie.h
        RSUser.cpp
        RSUser.h
        RecommenderSystem.cpp
        RecommenderSystem.h
        RecommenderSystemLoader.cpp
        RecommenderSystemLoader.h
        RSUsersLoader.cpp
        RSUsersLoader.h)

#add_executable(ex5_yarivyar
#        Movie.cpp
#        Movie.h
#        RecommenderSystem.cpp
#        RecommenderSystem.h
#        RecommenderSystemLoader.cpp
#        RecommenderSystemLoader.h
#        RSUser.cpp
#        RSUser.h
#        RSUsersLoader.cpp
#        RSUsersLoader.h)
