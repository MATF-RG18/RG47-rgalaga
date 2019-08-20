#pragma once

#include "Debug.h"
#include "GameObject.h"


class Player : public GameObject
{
    private:
        float m_Velocity;

    public:
        Player(float x, float y, float z, std::string texName,
               float angle, float width, float height, float step, GLFWwindow *window);
        virtual ~Player();

        void Move(const float x, const float y, const float z, const float angle) override;

        void HandleKeyPress(int key, int action);
};
