#include "Player.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Player::Player(float x, float y, float z, const std::string texName, const std::string missileTexName,
               float angle, float width, float height, float step, float velocity, short int lives)
    : GameObject(x, y, z, texName, angle, width, height, step), m_Velocity(velocity),
      m_MissileTexName(missileTexName), m_Score(0), m_NumberOfLives(lives)
{

}

Player::~Player()
{

}

void Player::Move(Movement direction)
{
    // If the Player hit the leftmost/rightmost pixel of window,
    // prevent moving further left/right by leaving changePosition false
    auto move = direction * (m_step + m_Velocity);

    bool changePosition = false;

    if (!((m_pos.x + m_width / 2 + move) >= 800 || (m_pos.x - m_width / 2 + move) <= 0)) {
        m_pos.x += move;
        changePosition = true;
    }

    if (changePosition)
        Transform(glm::vec2(m_pos.x, m_pos.y), glm::vec2(m_width, m_height), 0.0f);

    m_Velocity++;
}

Missile Player::MissileLaunch()
{
    Missile missile(m_pos.x, m_pos.y + m_height / 2, m_pos.z, m_MissileTexName, 0.0f, 10.0f, 20.0f, 5.0f);
    return missile;
}

