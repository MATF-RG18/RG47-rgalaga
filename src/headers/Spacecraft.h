#pragma once

#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


struct Position
{
    float x, y, z;

    Position& operator+(Position const& a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }

    Position& operator+=(Position const& a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }
};

//std::ostream& operator<<(std::ostream& strm, const Position& p)
//{
//    return strm << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
//}


class Spacecraft
{
    protected:
        Position m_pos;
        std::string m_tex;
        float m_glpos[16];
        unsigned int m_glind[6];
        float m_angle; // Angle from the x-axis
        float m_width, m_height;
    public:
        Spacecraft(float x, float y, float z, const std::string& path, float angle, float width, float height);
        virtual ~Spacecraft();

        inline const Position& getPos() const        { return m_pos; }
        inline const std::string& getTex() const     { return m_tex; }
        inline const float getAngle() const          { return m_angle; }
        inline const float getHeight() const         { return m_height; }
        inline const float getWidth() const          { return m_width; }
        inline const float* getGLPos() const         { return m_glpos; }
        inline const unsigned int* getGLInd() const  { return m_glind; }

        void setPosition(const float x, const float y, const float z);
        virtual void move(const float x, const float y, const float z);
        void updatePos(const Position& pos);
        void updateAngle(const float a);

};
