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


class Spacecraft
{
    protected:
        Position m_pos;
        std::string m_tex;
        float m_glpos[16];
        unsigned int m_glind[6];
        float m_angle; // Angle from the x-axis
        float m_width, m_height;

        glm::mat4 m_projection;
        glm::mat4 m_view;
        glm::mat4 m_model;
        glm::mat4 m_mvp;

    public:
        Spacecraft(float x, float y, float z, const std::string& path, float angle, float width, float height);
        virtual ~Spacecraft();

        inline const Position& GetPos() const        { return m_pos; }
        inline const std::string& GetTex() const     { return m_tex; }
        inline const float GetAngle() const          { return m_angle; }
        inline const float GetHeight() const         { return m_height; }
        inline const float GetWidth() const          { return m_width; }
        inline const float* GetGLPos() const         { return m_glpos; }
        inline const unsigned int* GetGLInd() const  { return m_glind; }
        inline const glm::mat4& GetMVP() const       { return m_mvp; }

        void SetPosition(const float x, const float y, const float z);
        virtual void Move(const float x, const float y, const float z);
        void UpdatePos(const Position& pos);
        void UpdateAngle(const float a);

};
