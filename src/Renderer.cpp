#include <iostream>
#include <algorithm>

#include <TmxPropertySet.h>

#include "Renderer.h"
#include "Animation.h"
#include "MultiObject.h"



extern Actor g_actor;
extern Animation g_animation;
extern ResourceManager g_resourceMgr;



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

    g_resourceMgr.loadAssets(m_sdlRenderer);
}



Renderer::~Renderer()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_sdlRenderer);
    SDL_Quit();
}



void Renderer::renderMap()
{
    // Go layer by layer and decide how to render it by it's type
    int numLayers = g_resourceMgr.m_map->GetNumLayers();
    for (int idx = 0; idx < numLayers; ++idx) {

        bool isActorLayer = false;
        if (idx == numLayers -1) {
            isActorLayer = true;
        }

        const Tmx::Layer *layer = g_resourceMgr.m_map->GetLayer(idx);
        switch (layer->GetLayerType()) {
            case Tmx::LayerType::TMX_LAYERTYPE_TILE:
                renderTileLayer(dynamic_cast<const Tmx::TileLayer*>(layer));
                break;
            case Tmx::LayerType::TMX_LAYERTYPE_OBJECTGROUP:
                renderObjectLayer(dynamic_cast<const Tmx::ObjectGroup*>(layer), isActorLayer);
                break;
            default:
                break;
        }
    }
}



void Renderer::renderTileLayer(const Tmx::TileLayer *layer)
{
    for (int y = 0; y < layer->GetHeight(); ++y) {
        for (int x = 0; x < layer->GetWidth(); ++x) {

            int tilesetIdx = layer->GetTileTilesetIndex(x, y);

            if (tilesetIdx != -1) {
                unsigned tileId = layer->GetTileId(x, y);
                const Tmx::Tileset *tileset = g_resourceMgr.m_map->GetTileset(tilesetIdx);

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
                                g_resourceMgr.m_tilesetList.find(tileset->GetName())->second,
                                &tileRect,
                                &targetRect
                                );
                }
            }
        }
    }
}



void Renderer::renderObjectLayer(const Tmx::ObjectGroup *layer, bool isActorLayer)
{
    // TO DO: render actor among objects

    // sort single tile objects by their position instead of ID
    // and create large objects
    const std::string MULTI_ID = "multiID";
    std::vector<std::shared_ptr<MultiObject>> largeObjects;
    std::vector<const Tmx::Object*> sortedObjects;
    for (auto object : layer->GetObjects())  {

        if (object->GetProperties().HasProperty(MULTI_ID)) {
            auto posIt = std::find_if(
                        largeObjects.begin(),
                        largeObjects.end(),
                        [object, MULTI_ID] (std::shared_ptr<MultiObject> multi) {
                            return !multi->parts.empty() &&
                            multi->parts.at(0)->GetProperties().GetStringProperty(MULTI_ID) ==
                            object->GetProperties().GetStringProperty(MULTI_ID);
                        }
            );

            // create a new large object
            if (posIt == largeObjects.end()) {
                std::shared_ptr<MultiObject> multiObj = std::make_shared<MultiObject>();
                if (multiObj->y < object->GetY()) {
                    multiObj->y = object->GetY();
                }
                multiObj->parts.push_back(object);
                // has to be sorted
                largeObjects.push_back(multiObj);
            }
            // add part to found large object
            else {
                largeObjects.at(posIt - largeObjects.begin())->parts.push_back(object);
            }
            continue;
        }

        auto posIt = std::find_if(
                    sortedObjects.begin(),
                    sortedObjects.end(),
                    [object] (const Tmx::Object *item) {
                        return object->GetY() < item->GetY();
                    }
        );
        sortedObjects.insert(posIt, object);
    }

    // sort large objects by y coordinate
    std::sort(
                largeObjects.begin(),
                largeObjects.end(),
                [] (std::shared_ptr<MultiObject> left, std::shared_ptr<MultiObject> right) {
                    return left->y < right->y;
                }
    );

    // Render objects in sorted order
    for (int objectIdx = 0, largeIdx = 0;
         objectIdx < sortedObjects.size() || largeIdx < largeObjects.size();
         ) {

        const Tmx::Object *object = nullptr;
        if (objectIdx < sortedObjects.size()) {
            object = sortedObjects.at(objectIdx);
        }
        std::shared_ptr<MultiObject> largeObject = nullptr;
        if (largeIdx < largeObjects.size()) {
            largeObject = largeObjects.at(largeIdx);
        }

        if (object != nullptr && (largeObject == nullptr || object->GetY() <= largeObject->y)) {
            renderObject(object);
            objectIdx++;
        }
        else {
            for (auto part : largeObject->parts) {
                renderObject(part);
            }
            largeIdx++;
        }
    }
}



void Renderer::renderObject(const Tmx::Object *object)
{
    int tilesetIdx = g_resourceMgr.m_map->FindTilesetIndex(object->GetGid());
    //    std::cout << "object Gid = " << object->GetGid() << ", tilesetIdx = " << tilesetIdx << std::endl;

    if (tilesetIdx != -1) {
    //   std::cout << "Getting object tileset" << std::endl;
    const Tmx::Tileset *tileset = g_resourceMgr.m_map->GetTileset(tilesetIdx);
    int tileId = object->GetGid() - tileset->GetFirstGid();

        if (tileset != nullptr) {
            //    std::cout << "Rendering object tile" << std::endl;
            int tilesPerRow = tileset->GetImage()->GetWidth() / tileset->GetTileWidth();
            int tileCol = tileId / tilesPerRow;

            SDL_Rect tileRect;
            tileRect.x = (tileId - (tileCol * tilesPerRow)) * tileset->GetTileWidth();
            tileRect.y = tileCol * tileset->GetTileHeight();
            tileRect.w = object->GetWidth();
            tileRect.h = object->GetHeight();

            SDL_Rect targetRect;
            targetRect.x = object->GetX();
            // Tiled uses bottom left corner as reference point while SDL uses top left
            targetRect.y = object->GetY() - tileset->GetTileHeight();
            targetRect.w = tileset->GetTileWidth();
            targetRect.h = tileset->GetTileHeight();

            SDL_RenderCopy(
                     m_sdlRenderer,
                     g_resourceMgr.m_tilesetList.find(tileset->GetName())->second,
                     &tileRect,
                     &targetRect
            );
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
    renderActor();

    // Up until now everything was drawn behind the scenes.
    // This will show the new contents of the window.
    SDL_RenderPresent(m_sdlRenderer);
}



void Renderer::renderActor()
{
    std::shared_ptr<SDL_Rect> spriteRect = g_animation.getFrameRect();

    SDL_Rect charPosRect;
    charPosRect.x = g_actor.GetPosition().x;
    charPosRect.y = g_actor.GetPosition().y - 64;
    charPosRect.w = 64;
    charPosRect.h = 64;

    SDL_RenderCopy(m_sdlRenderer, g_resourceMgr.m_characterTexture, spriteRect.get(), &charPosRect);

    SDL_Rect charRcharCollisionRect;
    charRcharCollisionRect.x = g_actor.GetPosition().x + 20;
    charRcharCollisionRect.y = g_actor.GetPosition().y - 12;
    charRcharCollisionRect.w = 24;
    charRcharCollisionRect.h = 12;

    SDL_RenderDrawRect(m_sdlRenderer, &charRcharCollisionRect);
}



SDL_Renderer* Renderer::getSdlRenderer()
{
    return m_sdlRenderer;
}
