#include "Game.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <dirent.h>
#include <cstring>
#include <cmath>

// Player details
const float PLAYER_WIDTH = 50.0f;
const float PLAYER_HEIGHT = 50.0f;
const float PLAYER_STARTING_STEP = 1.0f;
const float PLAYER_STARTING_POSITION_X = 400.0f;
const float PLAYER_STARTING_POSITION_Y = 30.0f;
const float PLAYER_STARTING_POSITION_Z = 1.0f;
const float PLAYER_STARTING_ANGLE = 0.0f;

// Enemy details
const float ENEMY_WIDTH = 40.0f;
const float ENEMY_HEIGHT = 40.0f;
const float ENEMY_RENDER_DISTANCE_X = 80.0f;
const float ENEMY_RENDER_DISTANCE_Y = 50.0f;
const float ENEMY_PADDING_X = 40.0f;
const float ENEMY_PADDING_Y = 25.0f;
const float ENEMY_EXPLOSION_STEP = 0.01f;

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

std::vector<Missile> Game::PlayerMissiles;
std::vector<Missile> Game::EnemyMissiles;
float Game::ScreenWidth;
float Game::ScreenHeight;

Game::Game(unsigned int width, unsigned int height, GLFWwindow *window)
    : m_State(GAME_ACTIVE), m_Keys(), m_Window(window),
      m_Player(PLAYER_STARTING_POSITION_X, PLAYER_STARTING_POSITION_Y, PLAYER_STARTING_POSITION_Z,
               "player", PLAYER_STARTING_ANGLE, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_STARTING_STEP, window),
      m_Projection(glm::ortho( 0.0f, static_cast<float>(width),
                               0.0f, static_cast<float>(height),
                              -1.0f, 1.0f)),
      m_View(glm::mat4(1.0f)),
      m_ActiveLevel(1),
      m_NumberOfEnemyTypes(6)
{
    LoadGameObjects();
    LoadLevels();
    Game::ScreenWidth = width;
    Game::ScreenHeight = height;
}

Game::~Game()
{

}

void Game::Init()
{

}

void Game::Update()
{
    // Cool the player's gun on each update if it's not already prepared for the next launch
    if (m_Player.GetGunState() != GunState::READY)
        m_Player.CoolGunDownBy(1);
    // Process all player's missiles: check if they are out of the screen or they hit some enemy's spaceship
    for (int i = 0; i < Game::PlayerMissiles.size(); )
    {
        Missile& missile = Game::PlayerMissiles[i];
        // Move the missile towards the enemy
        missile.Transform(glm::vec2(missile.GetPos().x, missile.GetPos().y + missile.GetStep()),
                          glm::vec2(missile.GetWidth(), missile.GetHeight()), 0.0f);

        // Clear missiles that are out of the screen
        if (missile.IsOutOfScreen(Game::ScreenWidth, Game::ScreenHeight))
        {
            Game::PlayerMissiles.erase(Game::PlayerMissiles.begin() + i);
            i--;
        }

        // Destroy enemy's spaceship if hit by player's missile
        for (int j = 0; j < m_Enemies.size(); j++)
        {
            Enemy& enemy = m_Enemies[j];
            if (fabs(enemy.GetPos().x - missile.GetPos().x) <= enemy.GetWidth() &&
                       fabs(enemy.GetPos().y - missile.GetPos().y) <= enemy.GetHeight()) {
                Game::PlayerMissiles.erase(Game::PlayerMissiles.begin() + i);
                i--;
                enemy.SetTexture("explosion");
                enemy.ExplosionTimeReduce(0.1);
                enemy.SetState(LifeState::DEAD);
                break;
            }
        }
        // 'i' will increment by 1 comparing to the previous step in the loop, only if there were no deletions
        // from Missile vectors - if some missile hit the enemy
        i++;
    }

    // Check if the level is over by counting dead enemies
    short int deadEnemiesCount = 0;
    for (int i = 0; i < m_Enemies.size(); i++)
    {
        Enemy& enemy = m_Enemies[i];
        if (!enemy.IsAlive())
            deadEnemiesCount++;

        // Scale down the explosion until it reaches zero by 'EXPLOSION_STEP'
        if (enemy.GetExplosionTime() < 1.0f && enemy.GetExplosionTime() > 0) {
            enemy.Transform(glm::vec2(enemy.GetPos().x, enemy.GetPos().y),
                            glm::vec2(enemy.GetExplosionTime() * enemy.GetWidth(),
                                      enemy.GetExplosionTime() * enemy.GetHeight()), 0.0f);
            enemy.ExplosionTimeReduce(ENEMY_EXPLOSION_STEP);
        } else {
            auto direction = enemy.GetDirection();
            enemy.Move(direction);
        }
    }
    if (deadEnemiesCount == m_Enemies.size())
        m_ActiveLevel++;
}

void Game::MissileLaunch(const GameObject& launcher)
{

}

void Game::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *player = reinterpret_cast<Player *>(glfwGetWindowUserPointer(window));
    if (player) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
                case GLFW_KEY_LEFT :
                    player->Move(Movement::LEFT);
                    break;
                case GLFW_KEY_RIGHT :
                    player->Move(Movement::RIGHT);
                    break;
                case GLFW_KEY_SPACE :
                    if (player->GetGunState() == GunState::READY) {
                        Game::PlayerMissiles.emplace_back(player->MissileLaunch());
                        player->ResetGunState();
                    }
                    // TODO:
                    //  -Maybe change missiles vectors to lists because of frequent deletions
                    break;
                default:
                    break;
            }
        }
        else if (action == GLFW_RELEASE)
        {
            player->AnulateVelocity();
        }
    }
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

void Game::SetEnemies()
{
    auto currentLevelEnemies = m_Levels[GetActiveLevel()].GetEnemies();
    m_RowsEnemiesCount = currentLevelEnemies.size();
    m_ColsEnemiesCount = currentLevelEnemies[0].size();

    for (int i = 0; i < m_RowsEnemiesCount; i++) {
        for (int j = 0; j < m_ColsEnemiesCount; j++) {
            m_Enemies.emplace_back(ENEMY_PADDING_X + j * ENEMY_RENDER_DISTANCE_X,
                                   Game::ScreenHeight - ENEMY_PADDING_Y - i * ENEMY_RENDER_DISTANCE_Y, 1,
                                   "enemy_" + std::to_string(currentLevelEnemies[i][j]), 0,
                                   ENEMY_WIDTH, ENEMY_HEIGHT, 0.5f, 10.0f);
        }
    }
}

void Game::LoadGameObjects()
{
    LoadTexture("res/textures/spacecraft.png", "player", 0);
    for (int i = 0; i < m_NumberOfEnemyTypes; i++)
        LoadTexture("res/textures/enemy-type-" + std::to_string(i + 1) + ".png",
                    "enemy_" + std::to_string(i + 1), 0);
    LoadTexture("res/textures/missile.png", "missile", 0);
    LoadTexture("res/textures/explosion.png", "explosion", 0);
}

unsigned int Game::GetActiveLevel()
{
    return m_ActiveLevel - 1;
}

void Game::GameLoop()
{
    LoadShader(SHADER_LOCATION, "basic");

    GetShader("basic").SetUniform1i("u_Texture", 0);

    Renderer renderer;

    VertexArray va;

    VertexBuffer vb(VERTICES, 4 * 4 * sizeof(float));
    IndexBuffer ib(INDICES, 6);

    VertexBufferLayout layout;
    layout.AddFloat(2);
    layout.AddFloat(2);

    va.AddBuffer(vb, layout);

    SetEnemies();

    do {
        Update();

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
            for (int i = 0; i < m_RowsEnemiesCount; i++)
            {
                for (int j = 0; j < m_ColsEnemiesCount; j++) {
                    GetShader("basic").Bind();
                    unsigned int location = m_ColsEnemiesCount * i + j;
                    m_Textures[m_Enemies[location].GetTex()].Bind();
                    GetShader("basic").SetUniformMat4f("u_MVP", m_Enemies[location].GetMVP());
                    renderer.Draw(va, ib, GetShader("basic"));
                }
            }
        }

        {   // Render Missiles
            for (int i = 0; i < Game::PlayerMissiles.size(); i++)
            {
                    GetShader("basic").Bind();
                    m_Textures[Game::PlayerMissiles[i].GetTex()].Bind();
                    GetShader("basic").SetUniformMat4f("u_MVP", Game::PlayerMissiles[i].GetMVP());
                    renderer.Draw(va, ib, GetShader("basic"));
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
