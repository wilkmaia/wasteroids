cmake_minimum_required (VERSION 2.6)

project (wasteroids C)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# Versioning
set (wasteroids_VERSION_MAJOR 0)
set (wasteroids_VERSION_MINOR 1)

# Add Allegro dependency
set (GCC_COVERAGE_LINK_FLAGS "-lallegro -lallegro_primitives -lallegro_font -lallegro_image -lm")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(wasteroids.out
    source/common.c
    source/ship.c
    source/main.c
    source/hiscore.c
    source/input.c
    source/blast.c
    source/asteroid.c
    source/text.c
)