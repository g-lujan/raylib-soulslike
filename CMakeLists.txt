cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(game VERSION 0.0.1 DESCRIPTION "A game")

find_package(raylib CONFIG REQUIRED)

set(SOURCE_FILES 
	source/main.c
    source/controls/input.h
    source/controls/input.c
    source/datastructures/hashmap.h
    source/datastructures/hashmap.c
    source/entities/cell.h
    source/entities/cell.c
    source/entities/player.h
    source/entities/player.c
    source/entities/player.c
    source/graphics/animation.h
    source/graphics/animation.c
    source/graphics/draw.h
    source/graphics/draw.c
    source/graphics/level.h
    source/graphics/level.c
    source/graphics/tilemap.h
    source/graphics/tilemap.c
    source/physics/body.h
    source/physics/body.c
    source/physics/incline.h
    source/physics/incline.c
)

set(ASSET_FILES 
    resources/castle-entrance/castle-entrance-map.json
    resources/castle-entrance/castle-entrance-tileset.json
    resources/castle-entrance/castle-entrance-tiles-packaged/castle-entrances-tiles-packaged.png
)

set(ALL_PROJECT_FILES ${SOURCE_FILES} ${ASSET_FILES})

add_executable(${PROJECT_NAME} ${ALL_PROJECT_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib opengl32 gdi32 winmm)

file(COPY resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
