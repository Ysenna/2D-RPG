#include <iostream>
#include <cassert>

#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Tmx.h>

const std::string g_assetPath = "../Data/";
std::map<std::string, SDL_Texture*> g_tilesetList; // List of tileset name + texture pairs

void loadAssets(Tmx::Map &map, SDL_Renderer *renderer)
{
    for (int i = 0; i < map.GetTilesets().size(); ++i) {
        const Tmx::Tileset *tileset = map.GetTileset(i);
        SDL_Texture *texture = IMG_LoadTexture(renderer, (g_assetPath + tileset->GetImage()->GetSource()).c_str());
        if (texture == NULL)
        {
            std::cout << "Unable to load tileset" << std::endl;
        }
        g_tilesetList.insert(std::make_pair(tileset->GetName(), texture));
    }
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



int main()
{
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

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
    // Select the color for drawing. It is set to pink here.
    SDL_SetRenderDrawColor(renderer, 255, 50, 180, 255);
    // Clear the entire screen to our selected color.
    SDL_RenderClear(renderer);



    Tmx::Map *map = new Tmx::Map();
    std::string fileName = "../Data/test.tmx";
    map->ParseFile(fileName);
    if (map->HasError()) {
        printf("error code: %d\n", map->GetErrorCode());
        printf("error text: %s\n", map->GetErrorText().c_str());

        return map->GetErrorCode();
    }

    loadAssets(*map, renderer);
    renderMap(*map, renderer);

    delete map;

    // Up until now everything was drawn behind the scenes.
    // This will show the new contents of the window.
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);  // Pause execution for 3000 milliseconds

    // Close and destroy the window
    SDL_DestroyWindow(window);
    // Clean up
    SDL_Quit();

    return 0;
}
