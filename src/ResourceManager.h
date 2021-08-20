#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <memory>

#include <SDL2/SDL_image.h>
#include <Tmx.h>

#include "Actor.h"
#include "MultiObject.h";



struct SortedObjects {
    std::string name;   // using ID would be better, but parser doesn't provide it
    std::vector<const Tmx::Object*> simpleObjects;
    std::vector<std::shared_ptr<LargeObject>> largeObjects;
};



class ResourceManager
{
public:
    const std::string m_assetPath = "../data/";
    Tmx::Map *m_map;
    std::vector<SortedObjects> m_sortedObjects;

    std::map<std::string, SDL_Texture *> m_tilesetList; // List of tileset name + texture pairs
    SDL_Texture *m_characterTexture;

    ResourceManager();
    ~ResourceManager();

    int loadAssets(SDL_Renderer *renderer);

private:
    void sortObjects();
};

#endif // ASSETMANAGER_H
