#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>

#include <SDL2/SDL_image.h>
#include <Tmx.h>

#include "Actor.h"

class ResourceManager
{
public:
    const std::string m_assetPath = "../data/";
    Tmx::Map *m_map;

    std::map<std::string, SDL_Texture *> m_tilesetList; // List of tileset name + texture pairs
    SDL_Texture *m_characterTexture;

    ResourceManager();
    ~ResourceManager();

    int loadAssets(SDL_Renderer *renderer);
};

#endif // ASSETMANAGER_H
