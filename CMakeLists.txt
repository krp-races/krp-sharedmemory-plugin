cmake_minimum_required(VERSION 3.5.0)
project(krp-sharedmemory-plugin VERSION 0.1.0 LANGUAGES C CXX)

add_library(krp-sharedmemory-plugin SHARED
    src/krp/krp-kart.h
    src/krp/krp-draw.h
    src/krp/krp-track.h
    src/krp/krp-race.h
    src/krp/krp-replay.h
    src/logger/logger.cpp
    src/logger/logger.h
    src/memory/memory.h
    src/memory/memory.cpp
    src/sharedmemory.h
    src/lib.cpp
    src/lib.h)

target_include_directories(krp-sharedmemory-plugin PRIVATE src)

# Copy dll to dlo file as kart racing pro only uses dlo files
add_custom_command(TARGET krp-sharedmemory-plugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/Debug/krp-sharedmemory-plugin.dll
    ${CMAKE_CURRENT_BINARY_DIR}/Debug/krp-sharedmemory-plugin.dlo)
