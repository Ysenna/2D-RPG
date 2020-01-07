#include <Tmx.h>

#include "Actor.h"
#include "ResourceManager.h"



#define SIN_45 0.707



extern ResourceManager g_resourceMgr;



bool isCollision(SDL_Rect rect1, SDL_Rect rect2)
{
    if (rect1.x < rect2.x + rect2.w &&
       rect1.x + rect1.w > rect2.x &&
       rect1.y > rect2.y - rect2.h &&
       rect1.y - rect1.h < rect2.y) {
        return true;
    }

    return false;
}



void Actor::Move(Direction direction)
{
    SDL_Rect charRect;
    charRect.x = m_position.x + 24;
    charRect.y = m_position.y;
    charRect.w = 24;
    charRect.h = 8;

    // TO DO - scale traveled distance by time delta since last frame
    switch (direction) {
        case Direction::LEFT:
            charRect.x -= m_speed;

            for (int group = 0; group < g_resourceMgr.m_map->GetNumObjectGroups(); ++group) {
                const Tmx::ObjectGroup *objGroup = g_resourceMgr.m_map->GetObjectGroup(group);

//                std::cout << "Character position = " << m_position.x << std::endl;
                for (int obj = 0; obj < objGroup->GetNumObjects(); ++obj) {
                    SDL_Rect objRect;
                    objRect.x = objGroup->GetObject(obj)->GetX();
                    objRect.y = objGroup->GetObject(obj)->GetY();
                    objRect.w = objGroup->GetObject(obj)->GetWidth();
                    objRect.h = objGroup->GetObject(obj)->GetHeight();

//                    std::cout << "Char rect x=" << charRect.x << ", y=" << charRect.y << ", w=" << charRect.w << ", h=" << charRect.h << std::endl;
//                    std::cout << "Obj rect x=" << objRect.x << ", y=" << objRect.y << ", w=" << objRect.w << ", h=" << objRect.h << std::endl;

                    if (isCollision(charRect, objRect)) {
                        return;
                    }
                }
            }
            // Map edge check
            // 32 = half of character rect width... in future replace by colision
            if (m_position.x - m_speed >= 16) {
                m_position.x -= m_speed;
            }
            break;
        case Direction::RIGHT:
            // To DO - Position of actor needs to be mapped to the center coordinates of animation
            // instead of checking map size etc., this has to be done by collision
            if (m_position.x + m_speed <=
                    g_resourceMgr.m_map->GetTileWidth() * g_resourceMgr.m_map->GetWidth() - 16) {
                m_position.x += m_speed;
            }
            break;
        case Direction::UP:
            if (m_position.y - m_speed >= 16) {
                m_position.y -= m_speed;
            }
            break;
        case Direction::DOWN:
            if (m_position.y + m_speed <=
                    g_resourceMgr.m_map->GetHeight() * g_resourceMgr.m_map->GetTileHeight() - 16) {
                m_position.y += m_speed;
            }
            break;
        case Direction::LEFT_UP:
            if (m_position.x - m_speed * SIN_45 >= 16) {
                m_position.x -= m_speed * SIN_45;
            }
            if (m_position.y - m_speed * SIN_45 >= 16) {
                m_position.y -= m_speed * SIN_45;
            }
            break;
        case Direction::LEFT_DOWN:
            if (m_position.x - m_speed * SIN_45 >= 16) {
                m_position.x -= m_speed * SIN_45;
            }
            if (m_position.y + m_speed * SIN_45 <=
                    g_resourceMgr.m_map->GetHeight() * g_resourceMgr.m_map->GetTileHeight() - 16) {
                m_position.y += m_speed * SIN_45;
            }
            break;
        case Direction::RIGHT_UP:
            if (m_position.x + m_speed * SIN_45 <=
                    g_resourceMgr.m_map->GetTileWidth() * g_resourceMgr.m_map->GetWidth() - 16) {
                m_position.x += m_speed * SIN_45;
            }
            if (m_position.y - m_speed * SIN_45 >= 16) {
                m_position.y -= m_speed * SIN_45;
            }
            break;
        case Direction::RIGHT_DOWN:
            if (m_position.x + m_speed * SIN_45 <=
                    g_resourceMgr.m_map->GetTileWidth() * g_resourceMgr.m_map->GetWidth() - 16) {
                m_position.x += m_speed * SIN_45;
            }
            if (m_position.y + m_speed * SIN_45 <=
                    g_resourceMgr.m_map->GetHeight() * g_resourceMgr.m_map->GetTileHeight() - 16) {
                m_position.y += m_speed * SIN_45;
            }
            break;
    }
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
