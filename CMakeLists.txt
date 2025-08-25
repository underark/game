cmake_minimum_required(VERSION 3.10.0)
project(2DGame VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(2DGame 
    src/main.c
)

include_directories(include)
include_directories(resources)

target_link_libraries(2DGame m)
target_link_libraries(2DGame PkgConfig::SDL2)
target_link_libraries(2DGame SDL2_image)


