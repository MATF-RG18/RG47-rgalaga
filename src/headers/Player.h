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
        int m_Score;
        short int m_NumberOfLives;
    public:
        Player(float x, float y, float z, const std::string texName, const std::string missileTexName,
               float angle, float width, float height, float step, float velocity, short int lives);
        virtual ~Player();

        void Move(Movement direction) override;
        Missile MissileLaunch();
        inline void AddScore(int score)            { m_Score += score; }
        inline void AddLives(short int amount)     { m_NumberOfLives += amount; }
        inline void RemoveLives(short int amount)  { m_NumberOfLives -= amount; }

        inline short int GetNumberOfLives() const  { return m_NumberOfLives; }
        inline int GetScore() const                { return m_Score; }

        inline void SetNumberOfLives(short int n)  { m_NumberOfLives = n; }

        inline void AnnulateVelocity()             { m_Velocity = 0.0f; }
};
