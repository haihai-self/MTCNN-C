cmake_minimum_required(VERSION 3.14)
project(MTCNN-C)

set(CMAKE_CXX_STANDARD 14)



set(SOURCE_FILES main.cpp)

add_executable(MTCNN-C main.cpp approximatemul/bbm.h approximatemul/bm07.h approximatemul/abm2.h approximatemul/pebm.h commom/basicop.h)
