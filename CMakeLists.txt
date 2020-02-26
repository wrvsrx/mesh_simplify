cmake_minimum_required(VERSION 3.0)

project(Mesh)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_subdirectory(src)
