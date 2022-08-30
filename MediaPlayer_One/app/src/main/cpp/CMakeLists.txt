# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("mediaplayer_one")


add_library( # Sets the name of the library.
        mediaplayer_one
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        native-lib.cpp)
find_library(
        log-lib
        log)

# 声明一个CPP源码文件路径
set(CPP_MAIN_PATH ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI})
# 配置头文件
include_directories(${CPP_MAIN_PATH}/include)
# 配置库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CPP_MAIN_PATH}/lib")


#set(${CMAKE_SOURCE_DIR}/lib_ffmpeg/)

#add_library(avutil
#        SHARED
#        IMPORTED)
#set_target_properties(avutil
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavutil.so)
#
#add_library(avresample
#        SHARED
#        IMPORTED)
#set_target_properties(avresample
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavresample.so)
#
#add_library(swresample
#        SHARED
#        IMPORTED)
#set_target_properties(swresample
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libswresample.so)
#
#add_library(swscale
#        SHARED
#        IMPORTED)
#set_target_properties(swscale
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libswscale.so)
#
#add_library(avcodec
#        SHARED
#        IMPORTED)
#set_target_properties(avcodec
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavcodec.so)
#
#add_library(avformat
#        SHARED
#        IMPORTED)
#set_target_properties(avformat
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavformat.so)
#
#add_library(avfilter
#        SHARED
#        IMPORTED)
#set_target_properties(avfilter
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavfilter.so)
#
#add_library(avdevice
#        SHARED
#        IMPORTED)
#
#set_target_properties(avdevice
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavdevice.so)
#
#include_directories(${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/include)

#set_target_properties(avdevice
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavdevice.so)
#
#include_directories(${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/include)

target_link_libraries( # Specifies the target library.
        mediaplayer_one
        avutil
        swresample
        swscale
        avcodec
        avformat
        avfilter
        ${log-lib})