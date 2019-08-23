#include "Missile.h"


Missile::Missile(float x, float y, float z, const std::string texName,
                 float angle, float width, float height, float step)
    : GameObject(x, y, z, texName, angle, width, height, step)
{

}

Missile::~Missile()
{

}

bool Missile::IsOutOfScreen(const float screenWidth, const float screenHeight)
{
    return (m_pos.x > screenWidth || m_pos.y > screenHeight || m_pos.x < 0 || m_pos.y < 0);
}