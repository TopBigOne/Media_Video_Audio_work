cmake_minimum_required(VERSION 3.22)
project(1_send_flv C)
set(CMAKE_C_STANDARD 99)

add_subdirectory(librtmp)

add_executable(1_send_flv main.c)

target_link_libraries(
        1_send_flv
        rtmp
)

