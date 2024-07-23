cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

project(
    opengl_renderer
    LANGUAGES CXX C
)

add_subdirectory(3rdlibs)
add_subdirectory(renderer)

option(BUILD_SANDBOX "build sandbox" ON)
if (BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()