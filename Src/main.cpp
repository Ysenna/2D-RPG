#include <stdio.h>

#include <iostream>
#include <cassert>
#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Tmx.h>

#include "Actor.h"



const std::string g_assetPath = "../Data/";
std::map<std::string, SDL_Texture*> g_tilesetList; // List of tileset name + texture pairs
SDL_Texture *g_character;
Actor g_actor;

Direction getDirectionFromVector(Vector2D directionVec)
{
    if (directionVec.x == 0.0 && directionVec.y == 0.0) {
        return Direction::NONE;
    }
    if (directionVec.x < 0.0 && directionVec.y == 0.0) {
        return Direction::LEFT;
    }
    else if (directionVec.x > 0.0 && directionVec.y == 0.0) {
        return Direction::RIGHT;
    }
    else if (directionVec.x == 0.0 && directionVec.y < 0.0) {
        return Direction::UP;
    }
    else if (directionVec.x == 0.0 && directionVec.y > 0.0) {
        return Direction::DOWN;
    }
    else if (directionVec.x < 0.0 && directionVec.y < 0.0) {
        return Direction::LEFT_UP;
    }
    else if (directionVec.x > 0.0 && directionVec.y < 0.0) {
        return Direction::RIGHT_UP;
    }
    else if (directionVec.x < 0.0 && directionVec.y > 0.0) {
        return Direction::LEFT_DOWN;
    }
    else if (directionVec.x > 0.0 && directionVec.y > 0.0) {
        return Direction::RIGHT_DOWN;
    }
}


int loadAssets(Tmx::Map &map, SDL_Renderer *renderer)
{
    // Load tilesets
    for (int i = 0; i < map.GetTilesets().size(); ++i) {
        const Tmx::Tileset *tileset = map.GetTileset(i);
        SDL_Texture *texture = IMG_LoadTexture(renderer, (g_assetPath + tileset->GetImage()->GetSource()).c_str());
        if (texture == nullptr) {
            return -1;
        }
        g_tilesetList.insert(std::make_pair(tileset->GetName(), texture));
    }

    // Load sprites
    g_character = IMG_LoadTexture(renderer, (g_assetPath + "female_walkcycle.png").c_str());
    if (g_character == nullptr) {
        return -1;
    }

    return 0;
}



void renderMap(Tmx::Map &map, SDL_Renderer *renderer)
{
    // TO DO: Map does not have to contain directly Tile Layers.
    // They can be grouped and Tmx::Map then contains a Group layer
    // which stores it's Tile layers.
    // Iterate through the tile layers.
    for (int i = 0; i < map.GetNumTileLayers(); ++i) {
        const Tmx::TileLayer *tileLayer = map.GetTileLayer(i);

        for (int y = 0; y < tileLayer->GetHeight(); ++y) {
            for (int x = 0; x < tileLayer->GetWidth(); ++x) {

                int tilesetIdx = tileLayer->GetTileTilesetIndex(x, y);

                if (tilesetIdx != -1) {
                    unsigned tileId = tileLayer->GetTileId(x, y);
                    const Tmx::Tileset *tileset = map.GetTileset(tilesetIdx);

                    if (tileset != nullptr) {
                        int tilesPerRow = tileset->GetImage()->GetWidth() / tileset->GetTileWidth();
                        int tileCol = tileId / tilesPerRow;

                        SDL_Rect tileRect;
                        tileRect.x = (tileId - (tileCol * tilesPerRow)) * tileset->GetTileWidth();
                        tileRect.y = tileCol * tileset->GetTileHeight();
                        tileRect.w = tileset->GetTileWidth();
                        tileRect.h = tileset->GetTileHeight();

                        SDL_Rect targetRect;
                        targetRect.x = x * tileset->GetTileWidth();
                        targetRect.y = y * tileset->GetTileHeight();
                        targetRect.w = tileset->GetTileWidth();
                        targetRect.h = tileset->GetTileHeight();

                        SDL_RenderCopy(renderer, g_tilesetList.find(tileset->GetName())->second, &tileRect, &targetRect);
                    }
                }
            }
        }
    }
}




void renderScene(Tmx::Map &map, SDL_Renderer *renderer)
{
    // Select the color for drawing. It is set to pink here.
    SDL_SetRenderDrawColor(renderer, 255, 50, 180, 255);
    // Clear the entire screen to our selected color.
    SDL_RenderClear(renderer);

    renderMap(map, renderer);

    SDL_Rect spriteRect;
    spriteRect.x = 0;
    spriteRect.y = 0;
    spriteRect.w = 64;
    spriteRect.h = 64;

    SDL_Rect charPosRect;
    charPosRect.x = g_actor.GetPosition().x;
    charPosRect.y = g_actor.GetPosition().y;
    charPosRect.w = 64;
    charPosRect.h = 64;

    SDL_RenderCopy(renderer, g_character, &spriteRect, &charPosRect);

    // Up until now everything was drawn behind the scenes.
    // This will show the new contents of the window.
    SDL_RenderPresent(renderer);
}



int main()
{
    Actor a();

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::cout << "Failed to initialize SDL";
        return -1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    window = SDL_CreateWindow(
                 "2D RPG",                  // window title
                 SDL_WINDOWPOS_UNDEFINED,           // initial x position
                 SDL_WINDOWPOS_UNDEFINED,           // initial y position
                 1024,                               // width, in pixels
                 900,                               // height, in pixels
                 SDL_WINDOW_SHOWN                  // flags - see below
             );
    // Check that the window was successfully created
    if (window == nullptr) {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }
    else {}

    // We must call SDL_CreateRenderer in order for draw calls to affect this window.
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        std::cout << "Failed to create renderer" << std::endl;
        SDL_Quit();
        return -1;
    }

    Tmx::Map *map = new Tmx::Map();
    std::string fileName = "../Data/test.tmx";
    map->ParseFile(fileName);
    if (map->HasError()) {
        printf("error code: %d\n", map->GetErrorCode());
        printf("error text: %s\n", map->GetErrorText().c_str());

        return map->GetErrorCode();
    }

    loadAssets(*map, renderer);

    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
    if (keystate == nullptr) {
        std::cout << "Failed to get keystate" << std::endl;
        return -1;
    }
    // Boolean data type not supported
    int gameRunning = 1;
    SDL_Event event;

    while (gameRunning)
    {
        renderScene(*map, renderer);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = 0;
                    break;
            }
        }

        SDL_PumpEvents();
        Vector2D directionVector(0.0, 0.0);
        if (keystate[SDL_SCANCODE_LEFT]  || keystate[SDL_SCANCODE_A]) {
            directionVector.x -= 1.0;
        }
        if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
            directionVector.x += 1.0;
        }
        if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) {
            directionVector.y -= 1.0;
        }
        if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
            directionVector.y += 1.0;
        }
        g_actor.Move(getDirectionFromVector(directionVector));

        // TO DO - Replace static value with a time remaining to next frame
        SDL_Delay(10);

    }

    delete map;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
