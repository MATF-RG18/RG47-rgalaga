#pragma once

#include "Debug.h"
#include "GameObject.h"


class Enemy : public GameObject
{
private:
    Position m_InitialPosition;
    Movement m_CurrentDirection;
    float m_ShootingProbability;
    float m_PatrolMoveLength;

public:
    Enemy(float x, float y, float z, std::string texName,
          float angle, float width, float height, float step, float patrol);
    virtual ~Enemy();

    void Move(int direction);
    // TODO: Rename Movement to Direction maybe
    Movement GetDirection();
    void ChangeDirection();
//    void Move(int direction);
};
