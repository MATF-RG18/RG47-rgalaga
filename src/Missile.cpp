#include "Missile.h"

Missile::Missile(float x, float y, float z, std::string texName, float angle,
                 float width, float height, float step)
    : GameObject(x, y, z, texName, angle, width, height, step)
{

}

Missile::~Missile()
{

}
