cmake_minimum_required(VERSION 3.22)
project(2_rtmp_receive C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(librtmp)

add_executable(2_rtmp_receive main.c)

target_link_libraries(2_rtmp_receive
        rtmp)
