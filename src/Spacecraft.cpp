#include "Spacecraft.h"


Spacecraft::Spacecraft(float x, float y, float z,
    const std::string& path, float angle, float width, float height)
    : m_pos{ x, y, z }, m_glpos{}, m_glind{}, m_tex(path), m_angle(angle), m_width(width), m_height(height)
{
    float glpos[] = {
        x - m_width/2, y - m_height/2, 0.0f, 0.0f, // 0
        x + m_width/2, y - m_height/2, 1.0f, 0.0f, // 1
        x + m_width/2, y + m_height/2, 1.0f, 1.0f, // 2
        x - m_width/2, y + m_height/2, 0.0f, 1.0f  // 3
    };

    unsigned int glind[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::copy(glpos, glpos + 16, m_glpos);
    std::copy(glind, glind + 6, m_glind);

    m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    m_view = glm::mat4(1.0f);
    m_model = glm::mat4(1.0f);

    m_mvp = m_projection * m_view * m_model;
}

Spacecraft::~Spacecraft()
{

}

void Spacecraft::SetPosition(const float x, const float y, const float z)
{

}

void Spacecraft::Move(const float x, const float y, const float z)
{

}

void Spacecraft::UpdatePos(const Position &pos)
{

}

void Spacecraft::UpdateAngle(const float a)
{

}
