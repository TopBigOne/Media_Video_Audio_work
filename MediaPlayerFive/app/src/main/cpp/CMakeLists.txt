# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("media_player_five")

file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/util/*.cpp
        ${CMAKE_SOURCE_DIR}/common/*.cpp
        ${CMAKE_SOURCE_DIR}/player/*.cpp
        ${CMAKE_SOURCE_DIR}/player/decoder/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/video/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/audio/*.cpp
        ${CMAKE_SOURCE_DIR}/recorder/SingleVideoRecorder.cpp
        ${CMAKE_SOURCE_DIR}/recorder/SingleAudioRecorder.cpp
        ${CMAKE_SOURCE_DIR}/recorder/MediaRecorderContext.cpp
        ${CMAKE_SOURCE_DIR}/recorder/GLCameraRender.cpp
        ${CMAKE_SOURCE_DIR}/recorder/MediaRecorder.cpp
        )

add_library( # Sets the name of the library.
        media_player_five

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${src-files})


set(ffmpeg_path ${CMAKE_SOURCE_DIR}/lib_ffmpeg/${CMAKE_ANDROID_ARCH_ABI})
message(WARNING "ffmpeg_path : ${ffmpeg_path}")

include_directories(${ffmpeg_path}/include)

add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_path}/lib/libavutil.so)

add_library(swresample
        SHARED
        IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_path}/lib/libswresample.so)

add_library(avcodec
        SHARED
        IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_path}/lib/libavcodec.so)

add_library(avfilter
        SHARED
        IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_path}/lib/libavfilter.so)

add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_path}/lib/libswscale.so)

add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_path}/lib/libavformat.so)


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


set(third-party-libs
        avformat
        avcodec
        avfilter
        swresample
        swscale
        avutil
        )


target_link_libraries( # Specifies the target library.
        media_player_five
        ${third-party-libs}
        ${log-lib})