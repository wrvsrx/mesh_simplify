include_directories(parse)
include_directories(simplify)
include_directories(core)

add_subdirectory(parse)
add_subdirectory(simplify)
add_subdirectory(core)
add_executable(mesh main.cpp)
target_link_libraries(mesh Parse Simplify Core)
