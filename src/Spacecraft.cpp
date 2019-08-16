#include "Spacecraft.h"


Spacecraft::Spacecraft(float x, float y, float z,
    const std::string& path, float angle, float width, float height)
    : m_pos{ x, y, z }, m_glpos{}, m_glind{}, m_tex(path), m_angle(angle), m_width(width), m_height(height)
{
    float glpos[] = {
        x - m_width/2,   y - m_height/2, 0.0f, 0.0f, // 0
        x + m_width/2,   y - m_height/2, 1.0f, 0.0f, // 1
        x + m_width/2,   y + m_height/2, 1.0f, 1.0f, // 2
        x - m_width/2,   y + m_height/2, 0.0f, 1.0f  // 3
    };

    unsigned int glind[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::copy(glpos, glpos + 16, m_glpos);
    std::copy(glind, glind + 6, m_glind);
}

Spacecraft::~Spacecraft()
{

}

void Spacecraft::setPosition(const float x, const float y, const float z)
{

}

void Spacecraft::move(const float x, const float y, const float z)
{

}

void Spacecraft::updatePos(const Position &pos)
{

}

void Spacecraft::updateAngle(const float a)
{

}
