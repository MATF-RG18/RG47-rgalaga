#pragma once


#include <algorithm>

#include "Spacecraft.h"
#include "Renderer.h"

class Player : public Spacecraft
{
    private:
        float m_step;

    public:
        Player(float x, float y, float z, const std::string& path,
            float angle, float width, float height, GLFWwindow *window);
        virtual ~Player();

        void Move(const float x, const float y, const float z) override;

        void HandleKeyPress(int key);
};

