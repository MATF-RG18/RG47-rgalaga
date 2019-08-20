#pragma once

#include "Debug.h"
#include "GameObject.h"


class Enemy : public GameObject
{
private:
    float m_step;
public:
    Enemy(float x, float y, float z, std::string texName,
          float angle, float width, float height, float step);
    virtual ~Enemy();

    void Move(const float x, const float y, const float z, const float angle);
};
