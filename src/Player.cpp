#include "Player.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Player::Player(float x, float y, float z, std::string texName, float angle,
               float width, float height, float step, GLFWwindow *window)
    : GameObject(x, y, z, texName, angle, width, height, step)
{
    // Add the pointer to Player so the Player's methods could be called from KeyCallback in Game.cpp
    glfwSetWindowUserPointer(window, this);
}

Player::~Player()
{

}

void Player::Move(const float x, const float y, const float z, const float angle)
{
    // Update Player position with args
    // If the Player hit the leftmost/rightmost/uppermost/downmost pixel of window,
    // prevent moving further left/right/up/down by leaving changePosition false
    bool changePosition = false;
    if ( !((m_pos.x + m_width / 2 + x) >=  800 || (m_pos.x - m_width / 2 + x)  <= 0) &&
         !((m_pos.y + m_height / 2 + y) >= 600 || (m_pos.y - m_height / 2 + y) <= 0))
    {
        m_pos.x += x;
        m_pos.y += y;
        changePosition = true;
    }

    if (changePosition)
        Transform(glm::vec2(m_pos.x, m_pos.y), glm::vec2(m_width, m_height), angle);

//    std::cout << "Moved to (" << m_pos.x << ", " << m_pos.y << ") - left: " << m_pos.x - m_width/2
//              << ", right: " << m_pos.x + m_width/2 << std::endl;
}

void Player::HandleKeyPress(int key, int action)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        m_Velocity += 1.0f;
        switch (key)
        {
            case GLFW_KEY_LEFT :
                this->Move(-(m_step + m_Velocity), 0, 0, 0);
                break;
            case GLFW_KEY_RIGHT :
                Move(m_step + m_Velocity, 0, 0, 0);
                break;
            default:
                break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        m_Velocity = 0;
    }

//    std::cout << "Key pressed code: " << key << std::endl;
//    std::cout << "Left code: " << GLFW_KEY_LEFT << std::endl;
//    std::cout << "Right code: " << GLFW_KEY_RIGHT << std::endl;
}
