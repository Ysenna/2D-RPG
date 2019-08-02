#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "AssetManager.h"

class Renderer
{
private:
    AssetManager m_assetMgr;

    SDL_Window *m_window;
    SDL_Renderer *m_sdlRenderer;

public:
    Renderer();
    ~Renderer();

    void renderMap();
    int loadAssets();
    void renderScene();
};

#endif // RENDERER_H
