#include "Player.h"


Player::Player(float x, float y, float z, const std::string &path,
    float angle, float width, float height, GLFWwindow *window)
    : Spacecraft(x, y, z, path, angle, width, height)
{
    glfwSetWindowUserPointer(window, this);
}

Player::~Player()
{

}

void Player::move(const float x, const float y, const float z)
{
    m_pos += { x, y, z };

    float glpos[] = {
        m_pos.x - m_width/2 + x, m_pos.y - m_height/2, 0.0f, 0.0f, // 0
        m_pos.x + m_width/2 + x, m_pos.y - m_height/2, 1.0f, 0.0f, // 1
        m_pos.x + m_width/2 + x, m_pos.y + m_height/2, 1.0f, 1.0f, // 2
        m_pos.x - m_width/2 + x, m_pos.y + m_height/2, 0.0f, 1.0f  // 3
    };

    unsigned int glind[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::copy(glpos, glpos + 16, m_glpos);
    std::copy(glind, glind + 6, m_glind);

    std::cout << "Moved to " << m_pos.x << ", " << m_pos.y << " !" << std::endl;
}

void Player::HandleKeyPress(int key)
{
    if (key == GLFW_KEY_LEFT)
        move(-3, 0, 0);
    else if (key == GLFW_KEY_RIGHT)
        move(3, 0, 0);
    std::cout << "Key pressed code: " << key << std::endl;
    std::cout << "Left code: " << GLFW_KEY_LEFT << std::endl;
    std::cout << "Right code: " << GLFW_KEY_RIGHT << std::endl;
}
