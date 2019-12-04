#ifndef INC_2D_RPG_ACTOR_H
#define INC_2D_RPG_ACTOR_H

#include <iostream>
#include <ostream>

struct Vector2D
{
    double x;
    double y;

    Vector2D(double posX, double posY) : x(posX), y(posY) {};

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& vector)
    {
        os << "X = " << vector.x << ", Y = " << vector.y << std::endl;
        return os;
    }
};



enum class Direction
{
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    LEFT_UP,
    LEFT_DOWN,
    RIGHT_UP,
    RIGHT_DOWN,

    SIZE
};


Direction getDirectionFromVector(Vector2D directionVec);

class Actor
{
private:
    unsigned m_id;

protected:
    Vector2D m_position;
    double m_speed;

public:
    Actor() : m_id(0), m_position(100.0, 100.0), m_speed(5.0) {};
    ~Actor() = default;

    Vector2D GetPosition()
    {
        return m_position;
    }

    double GetSpeed()
    {
        return m_speed;
    }

    void Move(Direction direction);
};


#endif //INC_2D_RPG_ACTOR_H
