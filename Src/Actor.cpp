#include "Actor.h"

#define SIN_45 0.707


void Actor::Move(Direction direction)
{
    // TO DO - scale traveled distance by time delta since last frame
    switch (direction) {
        case Direction::LEFT:
            m_position.x -= m_speed;
            break;
        case Direction::RIGHT:
            m_position.x += m_speed;
            break;
        case Direction::UP:
            m_position.y -= m_speed;
            break;
        case Direction::DOWN:
            m_position.y += m_speed;
            break;
        case Direction::LEFT_UP:
            m_position.x -= m_speed * SIN_45;
            m_position.y -= m_speed * SIN_45;
            break;
        case Direction::LEFT_DOWN:
            m_position.x -= m_speed * SIN_45;
            m_position.y += m_speed * SIN_45;
            break;
        case Direction::RIGHT_UP:
            m_position.x += m_speed * SIN_45;
            m_position.y -= m_speed * SIN_45;
            break;
        case Direction::RIGHT_DOWN:
            m_position.x += m_speed * SIN_45;
            m_position.y += m_speed * SIN_45;
            break;
    }
}
