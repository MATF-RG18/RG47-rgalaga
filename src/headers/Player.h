#pragma once

#include "Debug.h"
#include "Spacecraft.h"


class Player : public Spacecraft
{
    private:
        float m_step;

    public:
        Player(float x, float y, float z, const std::string& path,
               float angle, float width, float height, GLFWwindow *window);
        virtual ~Player();

        void Move(const float x, const float y, const float z);

        void HandleKeyPress(int key);
};
