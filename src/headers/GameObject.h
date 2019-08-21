#pragma once

#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


enum LifeState {
    DEAD = 0,
    ALIVE = 1
};

enum Movement {
    LEFT = -1,
    RIGHT = 1
};

enum GunState {
    READY = 0,
    FIRED = 50
};

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


class GameObject
{
    protected:
        Position m_pos;
        std::string m_tex;
        float m_angle; // Angle from the x-axis
        float m_width, m_height;
        float m_step;
        LifeState m_State;
        float m_ExplosionTime;

        short m_CoolDownTime;

        glm::mat4 m_projection;
        glm::mat4 m_view;
        glm::mat4 m_model;
        glm::mat4 m_mvp;
    public:
        GameObject(float x, float y, float z, std::string texName, float angle,
                   float width, float height, float step);
        virtual ~GameObject();

        inline const Position& GetPos() const                 { return m_pos; }
        inline const std::string& GetTex() const              { return m_tex; }
        inline const float GetAngle() const                   { return m_angle; }
        inline const float GetHeight() const                  { return m_height; }
        inline const float GetWidth() const                   { return m_width; }
        inline const float GetStep() const                    { return m_step; }
        inline const glm::mat4& GetMVP() const                { return m_mvp; }
        inline const float GetExplosionTime() const           { return m_ExplosionTime; }
        inline bool IsAlive()                                 { return m_State == LifeState::ALIVE; }
        inline const short& GetGunState() const            { return m_CoolDownTime; }
        inline void SetTexture(const std::string& texName)    { m_tex = texName; }
        inline void ExplosionTimeReduce(const float amount)   { m_ExplosionTime -= amount; }
        inline void SetState(LifeState state)                 { m_State = state; }
        inline void CoolGunDownBy(short amount)               { m_CoolDownTime = std::max(m_CoolDownTime - amount, 0); }
        inline void ResetGunState()                        { m_CoolDownTime = GunState::FIRED; }

        void SetPosition(const float x, const float y, const float z);
        void UpdatePos(const Position& pos);
        void UpdateAngle(const float a);

        void Transform(glm::vec2 position, glm::vec2 size, GLfloat rotate);

        virtual void Move(int direction);
};
