#pragma once

#include "Enemy.h"

#include <string>
#include <vector>


class GameLevel
{
    private:
        std::vector<std::vector<int> > m_Enemies;
        std::string m_Background;

        glm::mat4 m_MVP;

        void ParseLevel(const std::string &levelPath);
    public:
        GameLevel(const std::string &levelPath);
        ~GameLevel();

        inline const std::string &GetBackgroundPath() const                 { return m_Background; }
        inline const glm::mat4 &GetMVP() const                              { return m_MVP; }
        inline const std::vector<std::vector<int> > &GetEnemies() const     { return m_Enemies; }
};

