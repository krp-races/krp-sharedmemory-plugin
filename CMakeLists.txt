cmake_minimum_required(VERSION 3.5.0)
project(krp-sharedmemory-plugin VERSION 0.1.0 LANGUAGES C CXX)

add_library(krp-sharedmemory-plugin SHARED
    src/lib.cpp
    src/lib.h
    src/logger/logger.cpp
    src/logger/logger.h
    src/krp/krp-kart.h
    src/krp/krp-draw.h
    src/krp/krp-track.h
    src/krp/krp-race.h
    src/krp/krp-replay.h)

target_include_directories(krp-sharedmemory-plugin PRIVATE src)

# cpp-ipc
include(FetchContent)

FetchContent_Declare(
    cpp-ipc
    GIT_REPOSITORY https://github.com/mutouyun/cpp-ipc.git
    GIT_TAG "v1.3.0"
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
    SYSTEM
)

FetchContent_MakeAvailable(cpp-ipc)

# Copy dll to dlo file as kart racing pro only uses dlo files
add_custom_command(TARGET krp-sharedmemory-plugin POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_BINARY_DIR}/Debug/krp-sharedmemory-plugin.dll 
                   ${CMAKE_CURRENT_BINARY_DIR}/Debug/krp-sharedmemory-plugin.dlo)