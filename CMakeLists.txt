cmake_minimum_required(VERSION 3.16)
project(ShadowLantern LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(SHADOW_LANTERN_BUILD_TOOLS "Build developer tools" ON)

add_subdirectory(third_party)
add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
