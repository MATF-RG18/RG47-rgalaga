#pragma once

#include "Debug.h"
#include "GameObject.h"
#include "Missile.h"


class Player
    : public GameObject
{
    private:
        float m_Velocity;
        std::string m_MissileTexName;
    public:
        Player(float x, float y, float z, const std::string texName, const std::string missileTexName,
               float angle, float width, float height, float step, float velocity);
        virtual ~Player();

        void Move(Movement direction) override;
        Missile MissileLaunch();

        inline void AnnulateVelocity()   { m_Velocity = 0.0f; }
};
