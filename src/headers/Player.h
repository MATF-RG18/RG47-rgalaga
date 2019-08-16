#pragma once

#include "Spacecraft.h"
#include "Renderer.h"

class Player : public Spacecraft
{
    public:
        Player(float x, float y, float z, const std::string& path,
            float angle, float width, float height, GLFWwindow *window);
        virtual ~Player();

        void move(const float x, const float y, const float z) override;

        void HandleKeyPress(int key);
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

