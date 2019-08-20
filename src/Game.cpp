#include "Game.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <dirent.h>
#include <string.h>

// Player details
const float PLAYER_WIDTH = 50.0f;
const float PLAYER_HEIGHT = 50.0f;
const float PLAYER_STARTING_STEP = 1.0f;
const float PLAYER_STARTING_POSITION_X = 400.0f;
const float PLAYER_STARTING_POSITION_Y = 30.0f;
const float PLAYER_STARTING_POSITION_Z = 1.0f;
const float PLAYER_STARTING_ANGLE = 0.0f;

const float ENEMY_WIDTH = 40.0f;
const float ENEMY_HEIGHT = 40.0f;
const float ENEMY_RENDER_DISTANCE_X = 80.0f;
const float ENEMY_RENDER_DISTANCE_Y = 50.0f;
const float ENEMY_PADDING_X = 40.0f;
const float ENEMY_PADDING_Y = 25.0f;

const std::string SHADER_LOCATION = "res/shaders/Basic.shader";

const float VERTICES[] = {
    // Pos        // Tex
    -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 1.0f,
};

const unsigned int INDICES[] = {
    0, 1, 2,
    2, 3, 0
};

Game::Game(unsigned int width, unsigned int height, GLFWwindow *window)
    : m_State(GAME_ACTIVE), m_Keys(), m_Width(width), m_Height(height), m_Window(window),
      m_Player(PLAYER_STARTING_POSITION_X, PLAYER_STARTING_POSITION_Y, PLAYER_STARTING_POSITION_Z,
               "player", PLAYER_STARTING_ANGLE, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_STARTING_STEP, window),
      m_Projection(glm::ortho( 0.0f, static_cast<float>(m_Width),
                               0.0f, static_cast<float>(m_Height),
                              -1.0f, 1.0f)),
      m_View(glm::mat4(1.0f)),
      m_ActiveLevel(1),
      m_NumberOfEnemyTypes(6)
{

}

Game::~Game()
{

}

void Game::Init()
{

}

void Game::Update(GLfloat dt)
{

}

void Game::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *player = reinterpret_cast<Player *>(glfwGetWindowUserPointer(window));
    if (player)
        player->HandleKeyPress(key, action);
}

void Game::AddShader(const Shader *shader, const std::string& name)
{
    m_Shaders[name] = *shader;
}

Shader Game::GetShader(const std::string& name)
{
    return m_Shaders[name];
}

void Game::AddTexture(const Texture *texture, const std::string& name)
{
    m_Textures[name] = *texture;
}

Texture Game::GetTexture(const std::string& name)
{
    return m_Textures[name];
}

void Game::LoadShader(const std::string& path, const std::string& name)
{
    Shader *shader = new Shader(path);
    AddShader(shader, name);
}

void Game::LoadTexture(const std::string& path, const std::string& name, unsigned int slot)
{
    Texture *texture = new Texture(path);
    AddTexture(texture, name);
}

void Game::LoadLevels()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("res/levels")) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
            {
                m_Levels.emplace_back(ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror ("Could not open directory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m_Levels.size(); i++)
        LoadTexture(m_Levels[i].GetBackgroundPath(), "level-" + std::to_string(i + 1), 0);
}

void Game::BindActiveLevelTexture()
{
    m_Textures["level-" + std::to_string(m_ActiveLevel)].Bind();
    GetShader("basic").SetUniformMat4f("u_MVP", m_Levels[GetActiveLevel()].GetMVP());
}

void Game::LoadGameObjects()
{
    LoadTexture("res/textures/spacecraft.png", "player", 0);
    for (int i = 0; i < m_NumberOfEnemyTypes; i++)
        LoadTexture("res/textures/enemy-type-" + std::to_string(i + 1) + ".png",
                    "enemy_" + std::to_string(i + 1), 0);
}

unsigned int Game::GetActiveLevel()
{
    return m_ActiveLevel - 1;
}

void Game::GameLoop()
{
    LoadShader(SHADER_LOCATION, "basic");
    LoadGameObjects();
    LoadLevels();

    GetShader("basic").SetUniform1i("u_Texture", 0);

    Renderer renderer;

    VertexArray va;

    VertexBuffer vb(VERTICES, 4 * 4 * sizeof(float));
    IndexBuffer ib(INDICES, 6);

    VertexBufferLayout layout;
    layout.AddFloat(2);
    layout.AddFloat(2);

    va.AddBuffer(vb, layout);

    do {
        renderer.Clear();

        {   // Render background
            GetShader("basic").Bind();
            BindActiveLevelTexture();
            renderer.Draw(va, ib, GetShader("basic"));
        }
        {   // Render player
            GetShader("basic").Bind();
            m_Textures["player"].Bind();
            GetShader("basic").SetUniformMat4f("u_MVP", m_Player.GetMVP());
            renderer.Draw(va, ib, GetShader("basic"));
        }

        {   // Render Enemies
            auto currentLevelEnemies = m_Levels[GetActiveLevel()].GetEnemies();
            for (int i = 0; i < currentLevelEnemies.size(); i++) {
                for (int j = 0; j < currentLevelEnemies[i].size(); j++) {
                    Enemy enemy(ENEMY_PADDING_X + j * ENEMY_RENDER_DISTANCE_X,
                                Game::m_Height - ENEMY_PADDING_Y - i * ENEMY_RENDER_DISTANCE_Y, 1,
                                "enemy_" + std::to_string(currentLevelEnemies[i][j]), 0,
                                ENEMY_WIDTH, ENEMY_HEIGHT, 2.0f);
                    GetShader("basic").Bind();
                    m_Textures[enemy.GetTex()].Bind();
                    GetShader("basic").SetUniformMat4f("u_MVP", enemy.GetMVP());
                    renderer.Draw(va, ib, GetShader("basic"));
                }
            }
        }

        glfwSetWindowUserPointer(m_Window, &m_Player);

        glfwSwapBuffers(m_Window);
        glfwSwapInterval(1);

        glfwPollEvents();

        glfwSetKeyCallback(m_Window, KeyCallback);

    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
          glfwWindowShouldClose(m_Window) == 0);
}

void Game::SetState(GameState state)
{
    m_State = state;
}
