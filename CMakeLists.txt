cmake_minimum_required(VERSION 3.19)
project(car)

set(CMAKE_CXX_STANDARD 14)

add_executable(car main.cpp common.h common_macro.h
        find_lines.cpp img_proc.cpp main.h)