#pragma once

#include "GameObject.h"


class Missile
    : public GameObject
{
    private:
    public:
        Missile(float x, float y, float z, const std::string texName,
                float angle, float width, float height, float step);
        virtual ~Missile();

        bool IsOutOfScreen(const float screenWidth, const float screenHeight);

};
