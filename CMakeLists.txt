cmake_minimum_required(VERSION 3.20)
project(bugua)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fexec-charset=GBK -finput-charset=UTF-8 -fwide-exec-charset=GBK") #GBK

add_executable(bugua bugua.cpp)
