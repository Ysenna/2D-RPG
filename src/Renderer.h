#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ResourceManager.h"

class Renderer
{
private:


    SDL_Window *m_window;
    SDL_Renderer *m_sdlRenderer;

    void renderTileLayer(const Tmx::TileLayer *layer);
    void renderObjectLayer(const Tmx::ObjectGroup *layer, bool renderActor);
    void renderActor();
public:
    Renderer();
    ~Renderer();

    void renderMap();
    int loadAssets();
    void renderScene();
    void renderObject(const Tmx::Object *object);

    // this is just wrong... it has to be structured better way
    SDL_Renderer* getSdlRenderer();
};

#endif // RENDERER_H
