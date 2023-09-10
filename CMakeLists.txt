cmake_minimum_required(VERSION 3.25)
project(yamm C)

set(CMAKE_C_STANDARD 11)

add_executable(yamm main.c node.h buf.h buf.c yamm.h yamm.c)
