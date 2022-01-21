cmake_minimum_required(VERSION 3.21)
project(newlink)

set(CMAKE_CXX_STANDARD 14)

add_executable(newlink main.cpp)

target_include_directories(newlink PRIVATE "D:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include")
target_link_directories(newlink PRIVATE "D:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/lib/x86")
