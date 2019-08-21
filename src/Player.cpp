#include "Player.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Player::Player(float x, float y, float z, std::string texName, float angle,
               float width, float height, float step, GLFWwindow *window)
    : GameObject(x, y, z, texName, angle, width, height, step)
{
//    m_MissileSpeed = 5.0f;
    // Add the pointer to Player so the Player's methods could be called from KeyCallback in Game.cpp
//    glfwSetWindowUserPointer(window, this);
}

Player::~Player()
{

}

void Player::Move(int direction)
{
    // Update Player position with args
    // If the Player hit the leftmost/rightmost pixel of window,
    // prevent moving further left/right by leaving changePosition false
//    std::cout << "here i am" << std::endl;
    auto move = direction * (m_step + m_Velocity);

//    std::cout << move << std::endl;
    bool changePosition = false;

    if ( !((m_pos.x + m_width / 2 + move) >=  800 || (m_pos.x - m_width / 2 + move)  <= 0))
    {
        m_pos.x += move;
        changePosition = true;
    }

    if (changePosition)
        Transform(glm::vec2(m_pos.x, m_pos.y), glm::vec2(m_width, m_height), 0.0f);

    m_Velocity++;
//    std::cout << "Moved to (" << m_pos.x << ", " << m_pos.y << ") - left: " << m_pos.x - m_width/2
//              << ", right: " << m_pos.x + m_width/2 << std::endl;


}

Missile Player::MissileLaunch()
{
    Missile missile(m_pos.x, m_pos.y + m_height / 2, m_pos.z, "missile", 0.0f, 10.0f, 20.0f, 5.0f);
    return missile;
}

