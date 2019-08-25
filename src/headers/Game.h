#pragma once

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Player.h"
#include "Enemy.h"
#include "GameLevel.h"
#include "Missile.h"

#include <map>
#include <string>


// Represents the current state of the game
// TODO: Possibly add a state for level win, game over, ...
enum GameState {
    GAME_START,
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WON,
    GAME_OVER
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
    public:
        // Game state
        static GameState State;
        static float ScreenWidth;
        static float ScreenHeight;

        unsigned int m_RowsEnemiesCount;
        unsigned int m_ColsEnemiesCount;

        // OpenGL
        std::map<std::string, Shader> m_Shaders;
        std::map<std::string, Texture> m_Textures;

        // MVP
        glm::mat4 m_Projection;
        glm::mat4 m_View;

        // Game objects
        Player m_Player;
        std::vector<Enemy> m_Enemies;
        static std::vector<Missile> PlayerMissiles;
        static std::vector<Missile> EnemyMissiles;

        unsigned int m_ActiveLevel;
        std::vector<GameLevel> m_Levels;
        int m_NumberOfEnemyTypes;

        static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    public:
        Game(unsigned int width, unsigned int height);
        ~Game();

        static void SetState(GameState state);

        // Initialize game state (load all shaders/textures/levels)
        void Init();

        void AddShader(const Shader *shader, const std::string &name);
        Shader GetShader(const std::string &name);
        void AddTexture(const Texture *texture, const std::string &name);
        Texture GetTexture(const std::string &name);

        void LoadTexture(const std::string &path, const std::string &name, unsigned int slot);
        void LoadShader(const std::string &path, const std::string &name);
        void LoadLevelsStructure();
        void LoadNextLevel();
        void LoadGameObjects();
        void BindActiveLevelTexture();
        unsigned int GetActiveLevel();
        void SetEnemies();

        // GameLoop
        void Update();
        void GameLoop();
};
