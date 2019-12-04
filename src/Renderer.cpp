#include <iostream>

#include "Renderer.h"
#include "Animation.h"



extern Actor g_actor;
extern Animation g_animation;
extern AssetManager g_assetMgr;



Renderer::Renderer()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::cout << "Failed to initialize SDL";
   //     return -1;
    }

    m_window = SDL_CreateWindow(
                 "2D RPG",                  // window title
                 SDL_WINDOWPOS_UNDEFINED,           // initial x position
                 SDL_WINDOWPOS_UNDEFINED,           // initial y position
                 640,                               // width, in pixels
                 640,                               // height, in pixels
                 SDL_WINDOW_SHOWN                  // flags - see below
             );
    // Check that the window was successfully created
    if (m_window == nullptr) {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
      //  return 1;
    }
    else {}

    // We must call SDL_CreateRenderer in order for draw calls to affect this window.
    // SDL RENDERER_ACCELERATED sets renderer to use GPU
    m_sdlRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_sdlRenderer == nullptr) {
        std::cout << "Failed to create renderer" << std::endl;
        SDL_Quit();
   //     return -1;
    }

    g_assetMgr.loadAssets(m_sdlRenderer);
}



Renderer::~Renderer()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_sdlRenderer);
    SDL_Quit();
}



void Renderer::renderMap()
{
    // TO DO: Map does not have to contain directly Tile Layers.
    // They can be grouped and Tmx::Map then contains a Group layer
    // which stores it's Tile layers.
    // Iterate through the tile layers.
    for (int i = 0; i < g_assetMgr.m_map->GetNumTileLayers(); ++i) {
        const Tmx::TileLayer *tileLayer = g_assetMgr.m_map->GetTileLayer(i);

        for (int y = 0; y < tileLayer->GetHeight(); ++y) {
            for (int x = 0; x < tileLayer->GetWidth(); ++x) {

                int tilesetIdx = tileLayer->GetTileTilesetIndex(x, y);

                if (tilesetIdx != -1) {
                    unsigned tileId = tileLayer->GetTileId(x, y);
                    const Tmx::Tileset *tileset = g_assetMgr.m_map->GetTileset(tilesetIdx);

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

                        SDL_RenderCopy(
                                    m_sdlRenderer,
                                    g_assetMgr.m_tilesetList.find(tileset->GetName())->second,
                                    &tileRect,
                                    &targetRect
                                    );
                    }
                }
            }
        }
    }
}



void Renderer::renderScene()
{
    // Select the color for drawing. It is set to pink here.
    SDL_SetRenderDrawColor(m_sdlRenderer, 255, 50, 180, 255);
    // Clear the entire screen to our selected color.
    SDL_RenderClear(m_sdlRenderer);

    renderMap();

    std::shared_ptr<SDL_Rect> spriteRect = g_animation.getFrameRect();

    SDL_Rect charPosRect;
    charPosRect.x = g_actor.GetPosition().x - 32;
    charPosRect.y = g_actor.GetPosition().y - 64;
    charPosRect.w = 64;
    charPosRect.h = 64;

    SDL_RenderCopy(m_sdlRenderer, g_assetMgr.m_characterTexture, spriteRect.get(), &charPosRect);

    // Up until now everything was drawn behind the scenes.
    // This will show the new contents of the window.
    SDL_RenderPresent(m_sdlRenderer);
}



SDL_Renderer* Renderer::getSdlRenderer()
{
    return m_sdlRenderer;
}
