#include <algorithm>

#include "ResourceManager.h"



ResourceManager::ResourceManager()
{

}



ResourceManager::~ResourceManager()
{
    delete m_map;
}



// the renderer here can propbably be owned by asset manager
// storage and rendering should be decoupled
int ResourceManager::loadAssets(SDL_Renderer *renderer)
{
    // load map
    m_map = new Tmx::Map();
    std::string fileName = "../data/test.tmx";
    m_map->ParseFile(fileName);
    if (m_map->HasError()) {
        printf("error code: %d\n", m_map->GetErrorCode());
        printf("error text: %s\n", m_map->GetErrorText().c_str());

        return m_map->GetErrorCode();
    }

    sortObjects();

    // Load tilesets
    for (int i = 0; i < m_map->GetTilesets().size(); ++i) {
        const Tmx::Tileset *tileset = m_map->GetTileset(i);
        SDL_Texture *texture = IMG_LoadTexture(renderer, (m_assetPath + tileset->GetImage()->GetSource()).c_str());
        if (texture == nullptr) {
            return -1;
        }
        m_tilesetList.insert(std::make_pair(tileset->GetName(), texture));
    }

    // Load sprites
    m_characterTexture = IMG_LoadTexture(renderer, (m_assetPath + "female_walkcycle.png").c_str());
    if (m_characterTexture == nullptr) {
        return -1;
    }

    return 0;
}



void ResourceManager::sortObjects()
{
    for (Tmx::ObjectGroup *group : m_map->GetObjectGroups()) {
        const std::string MULTI_ID = "multiID";
        std::vector<std::shared_ptr<LargeObject>> largeObjectVec;
        std::vector<const Tmx::Object*> smallObjectVec;
        for (auto object : group->GetObjects())  {

            // handle multitile object
            if (object->GetProperties().HasProperty(MULTI_ID)) {
                auto posIt = std::find_if(
                            largeObjectVec.begin(),
                            largeObjectVec.end(),
                            [object, MULTI_ID] (std::shared_ptr<LargeObject> multi) {
                                return !multi->parts.empty() &&
                                multi->parts.at(0)->GetProperties().GetStringProperty(MULTI_ID) ==
                                object->GetProperties().GetStringProperty(MULTI_ID);
                            }
                );

                // create a new large object
                if (posIt == largeObjectVec.end()) {
                    std::shared_ptr<LargeObject> largeObject = std::make_shared<LargeObject>();
                    if (largeObject->y < object->GetY()) {
                        largeObject->y = object->GetY();
                    }
                    largeObject->parts.push_back(object);
                    largeObjectVec.push_back(largeObject);
                }
                // add part to found large object
                else {
                    largeObjectVec.at(posIt - largeObjectVec.begin())->parts.push_back(object);
                }
                continue;
            }

            // handle single tile object
            smallObjectVec.push_back(object);
        }

        // sort objects
        std::sort(
                    smallObjectVec.begin(),
                    smallObjectVec.end(),
                    [] (const Tmx::Object *left, const Tmx::Object *right) {
                        return left->GetY() < right->GetY();
                    }
        );

        std::sort(
                    largeObjectVec.begin(),
                    largeObjectVec.end(),
                    [] (std::shared_ptr<LargeObject> left, std::shared_ptr<LargeObject> right) {
                        return left->y < right->y;
                    }
        );

        // store sorted objects
        SortedObjects sortedObjects;
        sortedObjects.name = group->GetName();
        sortedObjects.simpleObjects = smallObjectVec;
        sortedObjects.largeObjects = largeObjectVec;
        m_sortedObjects.push_back(sortedObjects);
    }
}
