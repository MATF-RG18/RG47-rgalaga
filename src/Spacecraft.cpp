#include "Spacecraft.h"


Spacecraft::Spacecraft(float x, float y, float z, std::string texName, float angle,
                       float width, float height, float step)
    : m_pos{ x, y, z }, m_tex(texName), m_angle(angle),
      m_width(width), m_height(height),
      m_step(step),
      m_projection(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f)),
      m_view(glm::mat4(1.0f)),
      m_model(glm::mat4(1.0f))
{
    Transform(glm::vec2(x, y), glm::vec2(width, height), angle);
}

Spacecraft::~Spacecraft()
{

}

void Spacecraft::SetPosition(const float x, const float y, const float z)
{

}

void Spacecraft::Move(const float x, const float y, const float z, const float angle)
{

}

void Spacecraft::UpdatePos(const Position &pos)
{

}

void Spacecraft::UpdateAngle(const float a)
{

}

void Spacecraft::Transform(glm::vec2 position, glm::vec2 size, GLfloat rotate)
{
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
