#include <algorithm>

#include <Tmx.h>

#include "Actor.h"
#include "ResourceManager.h"



#define SIN_45 0.707



extern ResourceManager g_resourceMgr;



bool isCollision(const SDL_Rect &rect1, SDL_Rect *object)
{
//    if (rect1.x < rect2.x + rect2.w &&
//       rect1.x + rect1.w > rect2.x &&
//       rect1.y > rect2.y - rect2.h &&
//       rect1.y - rect1.h < rect2.y) {
//        return true;
//    }

//    return false;

    for (int group = 0; group < g_resourceMgr.m_map->GetNumObjectGroups(); ++group) {
        const Tmx::ObjectGroup *objGroup = g_resourceMgr.m_map->GetObjectGroup(group);

//                std::cout << "Character position = " << m_position.x << std::endl;
        for (int obj = 0; obj < objGroup->GetNumObjects(); ++obj) {
            int gid = objGroup->GetObject(obj)->GetGid();
            int tilesetIdx = g_resourceMgr.m_map->FindTilesetIndex(gid);
            const Tmx::Tileset *tileset = g_resourceMgr.m_map->GetTileset(tilesetIdx);
            int tileId = gid - tileset->GetFirstGid();

            if (tileset->GetTile(tileId) != nullptr && tileset->GetTile(tileId)->HasObjects()) {
//                        std::cout
                std::vector<Tmx::Object*> tileObjects = tileset->GetTile(tileId)->GetObjects();
                auto collisionBox = std::find_if(
                            tileObjects.begin(),
                            tileObjects.end(),
                            [] (const Tmx::Object *object) {
                                if (object->GetName() == "collision box") {
                                    return true;
                                }

                                return false;
                            }
                );

                if (collisionBox != tileObjects.end()) {
                    object->x = objGroup->GetObject(obj)->GetX() + (*collisionBox)->GetX();
                    // Tiled uses bottom left corner as reference point while SDL uses top left
                    // but the collision box inside a tile uses the same as SDL
                    object->y = objGroup->GetObject(obj)->GetY()
                            - objGroup->GetObject(obj)->GetHeight()
                            + (*collisionBox)->GetY();
                    object->w = (*collisionBox)->GetWidth();
                    object->h = (*collisionBox)->GetHeight();

//                    std::cout << "Char rect x=" << charRect.x << ", y=" << charRect.y << ", w=" << charRect.w << ", h=" << charRect.h << std::endl;
//                    std::cout << "Obj rect x=" << objRect.x << ", y=" << objRect.y << ", w=" << objRect.w << ", h=" << objRect.h << std::endl;

                    return SDL_HasIntersection(&rect1, object);
                }
            }
        }
    }

    return false;
}



void Actor::Move(Direction direction)
{
    int newX = m_position.x;
    int newY = m_position.y;

    // TO DO - scale traveled distance by time delta since last frame
    switch (direction) {
        case Direction::LEFT:
            // Map edge check
            // 32 = half of character rect width... in future replace by colision
            if (newX - m_speed >= 16) {
                newX -= m_speed;
            }
            break;
        case Direction::RIGHT:
            // To DO - Position of actor needs to be mapped to the center coordinates of animation
            // instead of checking map size etc., this has to be done by collision
            if (newX + m_speed <=
                    g_resourceMgr.m_map->GetTileWidth() * g_resourceMgr.m_map->GetWidth() - 16) {
                newX += m_speed;
            }
            break;
        case Direction::UP:
            if (newY - m_speed >= 16) {
                newY -= m_speed;
            }
            break;
        case Direction::DOWN:
            if (newY + m_speed <=
                    g_resourceMgr.m_map->GetHeight() * g_resourceMgr.m_map->GetTileHeight() - 16) {
                newY += m_speed;
            }
            break;
        case Direction::LEFT_UP:
            if (newX - m_speed * SIN_45 >= 16) {
                newX -= m_speed * SIN_45;
            }
            if (newY - m_speed * SIN_45 >= 16) {
                newY -= m_speed * SIN_45;
            }
            break;
        case Direction::LEFT_DOWN:
            if (newX - m_speed * SIN_45 >= 16) {
                newX -= m_speed * SIN_45;
            }
            if (newY + m_speed * SIN_45 <=
                    g_resourceMgr.m_map->GetHeight() * g_resourceMgr.m_map->GetTileHeight() - 16) {
                newY += m_speed * SIN_45;
            }
            break;
        case Direction::RIGHT_UP:
            if (newX + m_speed * SIN_45 <=
                    g_resourceMgr.m_map->GetTileWidth() * g_resourceMgr.m_map->GetWidth() - 16) {
                newX += m_speed * SIN_45;
            }
            if (newY - m_speed * SIN_45 >= 16) {
                newY -= m_speed * SIN_45;
            }
            break;
        case Direction::RIGHT_DOWN:
            if (newX + m_speed * SIN_45 <=
                    g_resourceMgr.m_map->GetTileWidth() * g_resourceMgr.m_map->GetWidth() - 16) {
                newX += m_speed * SIN_45;
            }
            if (newY + m_speed * SIN_45 <=
                    g_resourceMgr.m_map->GetHeight() * g_resourceMgr.m_map->GetTileHeight() - 16) {
                newY += m_speed * SIN_45;
            }
            break;
        default:
            break;
    }


    SDL_Rect charRect;
    charRect.x = newX + 20;
    charRect.y = newY - 12;
    charRect.w = 24;
    charRect.h = 12;
    SDL_Rect intersection;  // colliding object
    if (!isCollision(charRect, &intersection)) {
        m_position.x = newX;
        m_position.y = newY;
    }
//   move as far to the object as possible - currently broken by different coordinate system used by SDL and TMXs
//   TODO - load resource into own game representation with consolidate coordinates and easier to use structure
//    else {
//        // separate into function and unit test
//        int diffX = newX - m_position.x;
//        if (diffX < 0) {
//            m_position.x = intersection.x + intersection.w + 1;
//        }
//        else if (diffX > 0) {
//            m_position.x = intersection.x - 1;
//        }

//        int diffY = abs(newY - m_position.y);
//        if (diffY < 0) {
//            m_position.y = intersection.y + intersection.h + 1;
//        }
//        else if (diffY > 0) {
//            m_position.y = intersection.y - 1;
//        }
//    }
}



Direction getDirectionFromVector(Vector2D directionVec)
{
    if (directionVec.x == 0.0 && directionVec.y == 0.0) {
        return Direction::NONE;
    }
    if (directionVec.x < 0.0 && directionVec.y == 0.0) {
        return Direction::LEFT;
    }
    else if (directionVec.x > 0.0 && directionVec.y == 0.0) {
        return Direction::RIGHT;
    }
    else if (directionVec.x == 0.0 && directionVec.y < 0.0) {
        return Direction::UP;
    }
    else if (directionVec.x == 0.0 && directionVec.y > 0.0) {
        return Direction::DOWN;
    }
    else if (directionVec.x < 0.0 && directionVec.y < 0.0) {
        return Direction::LEFT_UP;
    }
    else if (directionVec.x > 0.0 && directionVec.y < 0.0) {
        return Direction::RIGHT_UP;
    }
    else if (directionVec.x < 0.0 && directionVec.y > 0.0) {
        return Direction::LEFT_DOWN;
    }
    else if (directionVec.x > 0.0 && directionVec.y > 0.0) {
        return Direction::RIGHT_DOWN;
    }
}
