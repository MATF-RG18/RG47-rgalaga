#include "GameLevel.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>


GameLevel::GameLevel(const std::string &levelPath)
{
    ParseLevel(levelPath);
    // TODO: Screen size hardcoded
    glm::mat4 projection(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f));
    glm::mat4 view(glm::mat4(1.0f));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 300.0f, -0.5f));
    model = glm::scale(model, glm::vec3(800, 600, 1.0f));

    m_MVP = projection * view * model;
}

GameLevel::~GameLevel()
{

}

void GameLevel::ParseLevel(const std::string &levelPath)
{
    std::ifstream stream("res/levels/" + levelPath);
    std::string line;
    while (getline(stream, line)) {
        std::istringstream is(line);
        if (line.empty()) break;
        m_Enemies.emplace_back(std::vector<int>(std::istream_iterator<int>(is), std::istream_iterator<int>()));
    }
    m_Background = "res/textures/level-" + levelPath.substr(0, levelPath.length() - 4) + "-background.png";
}
