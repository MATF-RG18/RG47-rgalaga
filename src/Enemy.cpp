#include "Enemy.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <ctime>

float Enemy::InitialShootingProbability;

Enemy::Enemy(float x, float y, float z, std::string texName, std::string missileTexName,
             float angle, float width, float height, float step, float patrol)
    : GameObject(x, y, z, texName, angle, width, height, step),
      m_InitialPosition{ x, y, z },
      m_CurrentDirection(Movement::LEFT),
      m_PatrolMoveLength(patrol),
      m_MissileTexName(missileTexName)
{
    // Generate random number between 0.0 and 0.1, using coordinates x and y because that's where they differ mostly
    std::srand(std::time(nullptr) + x + y);
    m_ShootingProbability = static_cast <float> (std::rand()) / ( static_cast <float> (RAND_MAX/(0.1)));
    Enemy::InitialShootingProbability = m_ShootingProbability;
    m_CoolDownTime = 500;
}

Enemy::~Enemy()
{

}

void Enemy::Move(int direction)
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
    // Reset the probability for launching when missiled is launched
    m_ShootingProbability = InitialShootingProbability;
    return missile;
}

bool Enemy::ToLaunch()
{
    std::srand(std::time(nullptr) + m_pos.x + m_pos.y);
    auto prob = static_cast <float> (std::rand()) / ( static_cast <float> (RAND_MAX));
    // There is greater probability for an enemy to shoot if he didn't launch for awhile
    m_ShootingProbability += 0.001;
    return prob <= m_ShootingProbability;
}
