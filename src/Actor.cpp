#include <Tmx.h>

#include "Actor.h"
#include "ResourceManager.h"



#define SIN_45 0.707



extern ResourceManager g_resourceMgr;



void Actor::Move(Direction direction)
{
    // TO DO - scale traveled distance by time delta since last frame
    switch (direction) {
        case Direction::LEFT:
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
