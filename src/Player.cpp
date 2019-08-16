#include "Player.h"


Player::Player(float x, float y, float z, const std::string &path,
    float angle, float width, float height, GLFWwindow *window)
    : Spacecraft(x, y, z, path, angle, width, height), m_step(2.0f)
{
    // Add the pointer to Player so the Player's methods could be called from KeyCallback in Game.cpp
    glfwSetWindowUserPointer(window, this);
}

Player::~Player()
{

}

void Player::Move(const float x, const float y, const float z)
{
    // Update Player position with args
    // If the Player hit the leftmost/rightmost pixel of window, prevent moving further left/right
    bool change_position = false;
    if ( !((m_pos.x + m_width / 2 + x) >= 800 || (m_pos.x - m_width / 2 + x) <= 0))
    {
        m_pos.x += x;
        change_position = true;
    }

    // Update GL positions
    float glpos[] = {
        m_pos.x - m_width/2, m_pos.y - m_height/2, 0.0f, 0.0f, // 0
        m_pos.x + m_width/2, m_pos.y - m_height/2, 1.0f, 0.0f, // 1
        m_pos.x + m_width/2, m_pos.y + m_height/2, 1.0f, 1.0f, // 2
        m_pos.x - m_width/2, m_pos.y + m_height/2, 0.0f, 1.0f  // 3
    };

    // Indices will always be in this order
    unsigned int glind[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Update GL arrays members
    std::copy(glpos, glpos + 16, m_glpos);
    std::copy(glind, glind + 6, m_glind);

    // If the Player is not in the furthermost corners of the screen, update mvp matrix
    if (change_position)
    {
        m_model = glm::translate(m_model, glm::vec3(x, 0, 0));
        m_mvp = m_projection * m_view * m_model;
    }

    std::cout << "Moved to (" << m_pos.x << ", " << m_pos.y << ") - left: " << m_pos.x - m_width/2 << ", right: " << m_pos.x + m_width/2 << std::endl;
}

void Player::HandleKeyPress(int key)
{
    switch(key) {
        case GLFW_KEY_LEFT :
            Move(-m_step, 0, 0);
            break;
        case GLFW_KEY_RIGHT :
            Move(m_step, 0, 0);
            break;
        default:
            break;
    }

//    std::cout << "Key pressed code: " << key << std::endl;
//    std::cout << "Left code: " << GLFW_KEY_LEFT << std::endl;
//    std::cout << "Right code: " << GLFW_KEY_RIGHT << std::endl;
}
