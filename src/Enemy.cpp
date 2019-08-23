#include "Enemy.h"

#include "glm/glm.hpp"

#include <random>


Enemy::Enemy(float x, float y, float z, const std::string texName, const std::string missileTexName,
             float angle, float width, float height, float step, float patrol)
    : GameObject(x, y, z, texName, angle, width, height, step),
      m_InitialPosition{x, y, z},
      m_CurrentDirection(Movement::LEFT),
      m_PatrolMoveLength(patrol),
      m_MissileTexName(missileTexName)
{

}

Enemy::~Enemy()
{
    
}

void Enemy::Move(Movement direction)
{
    // TODO:
    //  Kamikaza move maybe
    m_pos.x += m_step * direction;
    Transform(glm::vec2(m_pos.x, m_pos.y), glm::vec2(m_width, m_height), 0.0f);
}

void Enemy::ChangeDirection()
{
    (m_CurrentDirection == Movement::LEFT) ? m_CurrentDirection = Movement::RIGHT : m_CurrentDirection = Movement::LEFT;
}

Movement Enemy::GetDirection()
{
    if (fabs(m_pos.x - m_InitialPosition.x) >= m_PatrolMoveLength)
        ChangeDirection();
    return m_CurrentDirection;
}

Missile Enemy::MissileLaunch()
{
    Missile missile(m_pos.x, m_pos.y - m_height / 2, m_pos.z, m_MissileTexName, 0.0f, 10.0f, 20.0f, 5.0f);
    return missile;
}
