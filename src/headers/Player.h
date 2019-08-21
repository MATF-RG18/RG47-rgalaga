#pragma once

#include "Debug.h"
#include "GameObject.h"
#include "Missile.h"

class Player : public GameObject
{
    private:
        float m_Velocity;
    public:
        Player(float x, float y, float z, std::string texName,
               float angle, float width, float height, float step, GLFWwindow *window);
        virtual ~Player();

        void Move(int direction) override;
//        void Move(const float x, const float y, const float z, const float angle);
        Missile MissileLaunch();
        void HandleKeyPress(int key, int action);

        inline void AnulateVelocity() { m_Velocity = 0.0f; }
};
