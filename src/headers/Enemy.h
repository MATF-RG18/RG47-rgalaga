#pragma once

#include "Debug.h"
#include "Spacecraft.h"


class Enemy : public Spacecraft
{
private:
    float m_step;
public:
    Enemy(float x, float y, float z, std::string texName,
          float angle, float width, float height, float step);
    virtual ~Enemy();

    void Move(const float x, const float y, const float z, const float angle);
};
