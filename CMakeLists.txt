PROJECT(2D-RPG)
cmake_minimum_required(VERSION 3.7)

# build executable
ADD_EXECUTABLE(my_rpg
        src/main.cpp
        src/Actor.cpp
        src/Renderer.cpp
        src/AssetManager.cpp
        )

target_link_libraries(my_rpg
    SDL2
    SDL2_image
    tmxparser
    )
