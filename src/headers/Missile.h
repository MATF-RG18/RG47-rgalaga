#pragma once

#include "GameObject.h"
#include "Missile.h"

class Missile : public GameObject
{
    public:
    Missile(float x, float y, float z, std::string texName,
            float angle, float width, float height, float step);
    virtual ~Missile();

    bool IsOutOfScreen();
};
