cmake_minimum_required(VERSION 3.16)
project(ShadowLantern LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(shadow_lantern
    src/main.cpp
    src/game.cpp
)

target_include_directories(shadow_lantern PRIVATE include)

target_compile_features(shadow_lantern PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(shadow_lantern PRIVATE /W4 /permissive-)
else()
    target_compile_options(shadow_lantern PRIVATE -Wall -Wextra -Wpedantic)
endif()
