#pragma once

#include "Debug.h"
#include "Spacecraft.h"


class Player : public Spacecraft
{
    private:

    public:
        Player(float x, float y, float z, std::string texName,
               float angle, float width, float height, float step, GLFWwindow *window);
        virtual ~Player();

        void Move(const float x, const float y, const float z, const float angle) override;

        void HandleKeyPress(int key);
};
