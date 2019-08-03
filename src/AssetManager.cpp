#include "AssetManager.h"



AssetManager::AssetManager()
{

}



AssetManager::~AssetManager()
{
    delete m_map;
}



int AssetManager::loadAssets(SDL_Renderer *renderer)
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
