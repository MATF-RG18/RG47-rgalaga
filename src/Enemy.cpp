#include "Enemy.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Enemy::Enemy(float x, float y, float z, std::string texName,
               float angle, float width, float height, float step)
    : GameObject(x, y, z, texName, angle, width, height, step)
{

}

Enemy::~Enemy()
{

}

void Enemy::Move(const float x, const float y, const float z, const float angle)
{

}
