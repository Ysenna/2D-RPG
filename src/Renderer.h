#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "AssetManager.h"

class Renderer
{
private:


    SDL_Window *m_window;
    SDL_Renderer *m_sdlRenderer;

public:
    Renderer();
    ~Renderer();

    void renderMap();
    int loadAssets();
    void renderScene();

    // this is just wrong... it has to be structured better way
    SDL_Renderer* getSdlRenderer();
};

#endif // RENDERER_H
