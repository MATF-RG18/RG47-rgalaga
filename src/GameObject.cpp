#include "GameObject.h"


GameObject::GameObject(float x, float y, float z, const std::string texName,
                       float angle, float width, float height, float step)
    : m_pos{x, y, z}, m_tex(texName), m_angle(angle),
      m_width(width), m_height(height),
      m_step(step),
      m_projection(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f)),
      m_view(glm::mat4(1.0f)),
      m_model(glm::mat4(1.0f)),
      m_ExplosionTime(1.0f),
      m_State(LifeState::ALIVE),
      m_CoolDownTime(GunState::READY)
{
    Transform(glm::vec2(x, y), glm::vec2(width, height), angle);
}

GameObject::~GameObject()
{

}

void GameObject::Transform(glm::vec2 position, glm::vec2 size, float rotate)
{
    m_pos.x = position[0];
    m_pos.y = position[1];

    m_width = size[0];
    m_height = size[1];

    m_angle = rotate;

    // First translate (transformations are: scale happens first, then rotation
    // and then final translation happens; reversed order)
    m_model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    // Move origin of rotation to center of quad
    m_model = glm::translate(m_model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    // Then rotate
    m_model = glm::rotate(m_model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    // Move origin back
    m_model = glm::translate(m_model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    // Last scale
    m_model = glm::scale(m_model, glm::vec3(size, 1.0f));

    m_mvp = m_projection * m_view * m_model;
}


