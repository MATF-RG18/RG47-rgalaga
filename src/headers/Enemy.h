#pragma once

#include "Debug.h"
#include "GameObject.h"
#include "Missile.h"

class Enemy : public GameObject
{
private:
    Position m_InitialPosition;
    Movement m_CurrentDirection;
    float m_PatrolMoveLength;
    std::string m_MissileTexName;
public:
    Enemy(float x, float y, float z, std::string texName, std::string missileTexName,
          float angle, float width, float height, float step, float patrol);
    virtual ~Enemy();

    void Move(int direction);
    // TODO: Rename Movement to Direction maybe
    Movement GetDirection();
    Missile MissileLaunch();
    void ChangeDirection();
//    bool ToLaunch();
//    void Move(int direction);
};
