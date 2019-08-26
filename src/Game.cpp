#include "Game.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <dirent.h>
#include <cstring>
#include <cmath>
#include <random>

// Using irrKlang library for sound handling
#include <irrKlang.h>


using namespace irrklang;
ISoundEngine *SoundEngine = createIrrKlangDevice();

// TODO:
//  -All this global vars in other file maybe
//  -Use context to get window - delete window parameter where one exists
//  -Scale everything when the window size change
//

// Player details
const float PLAYER_WIDTH = 50.0f;
const float PLAYER_HEIGHT = 50.0f;
const float PLAYER_STARTING_STEP = 15.0f;
const float PLAYER_STARTING_POSITION_X = 400.0f;
const float PLAYER_STARTING_POSITION_Y = 30.0f;
const float PLAYER_STARTING_POSITION_Z = 1.0f;
const float PLAYER_STARTING_ANGLE = 0.0f;
const float PLAYER_STARTING_VELOCITY = 2.0f;
const short int PLAYER_INITIAL_NUMBER_OF_LIVES = 3;
// TODO: Give different values for each enemy in the future, maybe
const int PLAYER_SCORE_FOR_KILL = 10;

// Enemy details
const float ENEMY_WIDTH = 30.0f;
const float ENEMY_HEIGHT = 30.0f;
const float ENEMY_RENDER_DISTANCE_X = 80.0f;
const float ENEMY_RENDER_DISTANCE_Y = 50.0f;
const float ENEMY_PADDING_X = 40.0f;
const float ENEMY_PADDING_Y = 25.0f;
const float ENEMY_EXPLOSION_STEP = 0.01f;
const float ENEMY_LAUNCH_RATE_LIMIT = 8.0f;

const std::string SHADER_LOCATION = "res/shaders/Basic.shader";

const float VERTICES[] = {
    // Pos        // Tex
    -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 1.0f,
};

const unsigned int INDICES[] = {
    0, 1, 2,
    2, 3, 0
};

std::vector<Missile> Game::PlayerMissiles;
std::vector<Missile> Game::EnemyMissiles;
float Game::ScreenWidth;
float Game::ScreenHeight;
GameState Game::State;

Game::Game(unsigned int width, unsigned int height)
    : m_Player(PLAYER_STARTING_POSITION_X, PLAYER_STARTING_POSITION_Y, PLAYER_STARTING_POSITION_Z,
               "player", "missile", PLAYER_STARTING_ANGLE, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_STARTING_STEP,
               PLAYER_STARTING_VELOCITY, PLAYER_INITIAL_NUMBER_OF_LIVES),
      m_Projection(glm::ortho(0.0f, static_cast<float>(width),
                              0.0f, static_cast<float>(height),
                              -1.0f, 1.0f)),
      m_View(glm::mat4(1.0f)),
      m_ActiveLevel(1),
      m_NumberOfEnemyTypes(6)
{
    LoadGameObjects();
    LoadLevelsStructure();
    Game::ScreenWidth = width;
    Game::ScreenHeight = height;
    Game::State = GameState::GAME_START;
}

Game::~Game()
{
    SoundEngine->drop();
}

void Game::GameLoop()
{
    if (!SoundEngine) {
        std::cerr << "Could not startup engine" << std::endl;
        return; // error starting up the engine
    }
    SoundEngine->play2D("res/audio/rgalaga.wav", GL_TRUE);

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

//    LoadNextLevel();

    GLFWwindow *window = GLCall(glfwGetCurrentContext());

    do {
        Update();

        renderer.Clear();
        if (Game::State == GameState::GAME_START) {
            m_ActiveLevel = 1;
            LoadNextLevel();

            GetShader("basic").Bind();
            m_Textures["welcome-screen"].Bind();
            GetShader("basic").SetUniformMat4f("u_MVP", m_Levels[GetActiveLevel()].GetMVP());
            renderer.Draw(va, ib, GetShader("basic"));

            m_Player.Transform(glm::vec2(PLAYER_STARTING_POSITION_X, PLAYER_STARTING_POSITION_Y),
                               glm::vec2(PLAYER_WIDTH, PLAYER_HEIGHT), PLAYER_STARTING_ANGLE);
            m_Player.SetLifeState(LifeState::ALIVE);
            m_Player.SetNumberOfLives(PLAYER_INITIAL_NUMBER_OF_LIVES);
        } else if (Game::State == GameState::GAME_ACTIVE) {
            {   // Render background
                GetShader("basic").Bind();
                BindActiveLevelTexture();
                renderer.Draw(va, ib, GetShader("basic"));
            }
            {   // Render player
                GetShader("basic").Bind();
                m_Textures[m_Player.GetTex()].Bind();
                GetShader("basic").SetUniformMat4f("u_MVP", m_Player.GetMVP());
                renderer.Draw(va, ib, GetShader("basic"));
            }

            {   // Render Enemies
                for (int i = 0; i < m_RowsEnemiesCount; i++) {
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
                {
                    // Render Player's' Missiles
                    for (int i = 0; i < Game::PlayerMissiles.size(); i++) {
                        GetShader("basic").Bind();
                        m_Textures[Game::PlayerMissiles[i].GetTex()].Bind();
                        GetShader("basic").SetUniformMat4f("u_MVP", Game::PlayerMissiles[i].GetMVP());
                        renderer.Draw(va, ib, GetShader("basic"));
                    }

                    // Render Enemy's Missiles
                    for (int i = 0; i < Game::EnemyMissiles.size(); i++) {
                        GetShader("basic").Bind();
                        m_Textures[Game::EnemyMissiles[i].GetTex()].Bind();
                        GetShader("basic").SetUniformMat4f("u_MVP", Game::EnemyMissiles[i].GetMVP());
                        renderer.Draw(va, ib, GetShader("basic"));
                    }
                }
            }
        } else if(Game::State == GameState::GAME_WON) {
            GetShader("basic").Bind();
            m_Textures["game-won-screen"].Bind();
            GetShader("basic").SetUniformMat4f("u_MVP", m_Levels[GetActiveLevel()].GetMVP());
            renderer.Draw(va, ib, GetShader("basic"));
        } else if(Game::State == GameState::GAME_OVER) {
            GetShader("basic").Bind();
            m_Textures["game-over-screen"].Bind();
            GetShader("basic").SetUniformMat4f("u_MVP", m_Levels[GetActiveLevel()].GetMVP());
            renderer.Draw(va, ib, GetShader("basic"));
        }

        glfwSetWindowUserPointer(window, &m_Player);

        glfwSwapBuffers(window);
        glfwSwapInterval(1);

        glfwPollEvents();

        glfwSetKeyCallback(window, KeyCallback);

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
}

void Game::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto *player = reinterpret_cast<Player *>(glfwGetWindowUserPointer(window));
    if (player) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
                case GLFW_KEY_S :
                    (Game::State == GameState::GAME_START) ? SetState(GameState::GAME_ACTIVE) :
                                                             SetState(GameState::GAME_START);
                case GLFW_KEY_LEFT :
                    player->Move(Movement::LEFT);
                    break;
                case GLFW_KEY_RIGHT :
                    player->Move(Movement::RIGHT);
                    break;
                case GLFW_KEY_SPACE :
                    if (player->GetGunState() == GunState::READY && player->GetLifeState() == LifeState::ALIVE) {
                        Game::PlayerMissiles.emplace_back(player->MissileLaunch());
                        SoundEngine->play2D("res/audio/missile_launching.wav", GL_FALSE);
                        player->ResetGunState();
                    }
                    // TODO:
                    //  -Maybe change missiles vectors to lists because of frequent deletions
                    break;
                default:
                    break;
            }
        } else if (action == GLFW_RELEASE) {
            player->AnnulateVelocity();
        }
    }
}

// TODO: Refactor this function
void Game::Update()
{
    if (m_Player.GetLifeState() == LifeState::DEAD) {
        m_Player.SetTexture("explosion");
        m_Player.ExplosionTimeReduce(0.1f);
    }

    if (m_Player.GetExplosionTime() <= 0) {
        m_Player.SetTexture("player");
        m_Player.Transform(glm::vec2(PLAYER_STARTING_POSITION_X, PLAYER_STARTING_POSITION_Y),
                           glm::vec2(PLAYER_WIDTH, PLAYER_HEIGHT), PLAYER_STARTING_ANGLE);
        m_Player.SetExplosionTime(1.0f);
        m_Player.SetLifeState(LifeState::ALIVE);
    }

    // Cool down the player's gun on each update if it's not already prepared for the next missile launch
    if (m_Player.GetGunState() != GunState::READY)
        m_Player.CoolGunDownBy(1);

    // Process all player's missiles: check if they are out of the screen or they hit some enemy's spaceship
    for (int i = 0; i < Game::PlayerMissiles.size();) {
        Missile &missile = Game::PlayerMissiles[i];
        // Move the missile towards the enemy
        missile.Transform(glm::vec2(missile.GetPos().x, missile.GetPos().y + missile.GetStep()),
                          glm::vec2(missile.GetWidth(), missile.GetHeight()), 0.0f);

        // Clear missiles that are out of the screen
        if (missile.IsOutOfScreen(Game::ScreenWidth, Game::ScreenHeight)) {
            Game::PlayerMissiles.erase(Game::PlayerMissiles.begin() + i);
            i--;
        }

        // Destroy enemy's spaceship if hit by player's missile
        for (int j = 0; j < m_Enemies.size(); j++) {
            Enemy &enemy = m_Enemies[j];
            if (fabs(enemy.GetPos().x - missile.GetPos().x) <= enemy.GetWidth() / 2 &&
                fabs(enemy.GetPos().y - missile.GetPos().y) <= enemy.GetHeight() / 2) {
                Game::PlayerMissiles.erase(Game::PlayerMissiles.begin() + i);
                SoundEngine->play2D("res/audio/blast.wav", GL_FALSE);
                i--;
                m_Player.AddScore(PLAYER_SCORE_FOR_KILL);
                enemy.SetTexture("explosion");
                enemy.ExplosionTimeReduce(0.2f);
                enemy.SetLifeState(LifeState::DEAD);
                break;
            }
        }
        // 'i' will increment by 1 comparing to the previous step in the loop, only if there were no deletions
        // from Missile vectors - if some missile hit the enemy
        i++;
    }

    // Generate values from exponential distribution which will then be used to determine whether the
    // enemy's spaceship will launch missile or not
    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> d(1);
    std::vector<float> launchProb;
    for (int i = 0; i < m_Enemies.size(); i++) {
        launchProb.emplace_back(d(gen));
    }

    short int deadEnemiesCount = 0;
    // Check if the level is over by counting dead enemies and launch enemy's missiles
    // TODO: Really erase enemies from the vector when they are destroyed; there will be no need to count them
    for (int i = 0; i < m_Enemies.size(); i++) {
        Enemy &enemy = m_Enemies[i];
        // Cool down gun of the enemy to prevent frequent missiles
        enemy.CoolGunDownBy(1);

        if (!enemy.IsAlive()) {
            deadEnemiesCount++;
            // Scale down the explosion until it reaches zero by 'EXPLOSION_STEP'
            if (enemy.GetExplosionTime() < 1.0f && enemy.GetExplosionTime() > 0) {
                enemy.Transform(glm::vec2(enemy.GetPos().x, enemy.GetPos().y),
                                glm::vec2(enemy.GetExplosionTime() * enemy.GetWidth(),
                                          enemy.GetExplosionTime() * enemy.GetHeight()), 0.0f);
                enemy.ExplosionTimeReduce(ENEMY_EXPLOSION_STEP);
            }
        } else {
            enemy.Move(enemy.GetDirection());
            // Decrease limit which is used to calculate if the enemy should fire - Increase possibility of shooting
            // if the number of enemies decreases
            // TODO: This needs some adjusting
            float decreaseLimit = 5 * (1 / (m_Enemies.size() - deadEnemiesCount + 1));
            if (enemy.GetGunState() == GunState::READY && (launchProb[i] > ENEMY_LAUNCH_RATE_LIMIT - decreaseLimit) &&
                Game::State == GameState::GAME_ACTIVE) {

                Game::EnemyMissiles.emplace_back(enemy.MissileLaunch());
                SoundEngine->play2D("res/audio/missile_launching.wav", GL_FALSE);
                enemy.ResetGunState();
            }
        }
        if (enemy.GetExplosionTime() <= 0.0f) {
            // Move the enemy away from the scene
            // TODO: This will be removed when the enemy's spaceship is actually destroyed from the vector
            enemy.Transform(glm::vec2(-1, -1), glm::vec2(0, 0), 0.0f);
        }
    }

    // Process all enemy's missiles: check if they are out of the screen or they hit the player
    for (int i = 0; i < Game::EnemyMissiles.size();) {
        Missile &missile = Game::EnemyMissiles[i];
        // Move the missile towards the enemy
        missile.Transform(glm::vec2(missile.GetPos().x, missile.GetPos().y - missile.GetStep()),
                          glm::vec2(missile.GetWidth(), missile.GetHeight()), 0.0f);

        // Clear missiles that are out of the screen
        if (missile.IsOutOfScreen(Game::ScreenWidth, Game::ScreenHeight)) {
            Game::EnemyMissiles.erase(Game::EnemyMissiles.begin() + i);
            i--;
        }

        // Destroy player's spaceship if hit by enemy's missile
        if (fabs(m_Player.GetPos().x - missile.GetPos().x) <= m_Player.GetWidth() / 2 &&
            fabs(m_Player.GetPos().y - missile.GetPos().y) <= m_Player.GetHeight() / 2) {
            Game::EnemyMissiles.erase(Game::EnemyMissiles.begin() + i);
            SoundEngine->play2D("res/audio/blast.wav", GL_FALSE);
            m_Player.SetTexture("explosion");
            m_Player.ExplosionTimeReduce(0.1);
            m_Player.SetLifeState(LifeState::DEAD);
            m_Player.RemoveLives(1);
            if (m_Player.GetNumberOfLives() <= 0) {
                SetState(GameState::GAME_OVER);
            }
            break;
        }

        // 'i' will increment by 1 comparing to the previous step in the loop, only if there were no deletions
        // from Missile vectors - if some missile hit the enemy
        i++;
    }

    if (deadEnemiesCount == m_Enemies.size() && Game::State == GameState::GAME_ACTIVE) {
        m_ActiveLevel++;
        m_Enemies.clear();
        LoadNextLevel();
    }
}

void Game::LoadGameObjects()
{
    LoadTexture("res/textures/spacecraft.png", "player", 0);
    for (int i = 0; i < m_NumberOfEnemyTypes; i++)
        LoadTexture("res/textures/enemy-type-" + std::to_string(i + 1) + ".png",
                    "enemy_" + std::to_string(i + 1), 0);
    LoadTexture("res/textures/missile.png", "missile", 0);
    LoadTexture("res/textures/missile-enemy.png", "missile-enemy", 0);
    LoadTexture("res/textures/explosion.png", "explosion", 0);

    LoadTexture("res/textures/welcome-screen.png", "welcome-screen", 0);
    LoadTexture("res/textures/game-over-screen.png", "game-over-screen", 0);
    LoadTexture("res/textures/end-game-screen.png", "game-won-screen", 0);
}

void Game::LoadNextLevel()
{
    if (m_ActiveLevel >= m_Levels.size()) {
        SetState(GameState::GAME_WON);
    } else {
        SetEnemies();
        BindActiveLevelTexture();
        m_Player.Transform(glm::vec2(PLAYER_STARTING_POSITION_X, PLAYER_STARTING_POSITION_Y),
                           glm::vec2(PLAYER_WIDTH, PLAYER_HEIGHT), PLAYER_STARTING_ANGLE);
    }
}

void Game::LoadLevelsStructure()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("res/levels")) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) {
                m_Levels.emplace_back(ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m_Levels.size(); i++) {
        LoadTexture(m_Levels[i].GetBackgroundPath(), "level-" + std::to_string(i + 1), 0);
    }
}

void Game::BindActiveLevelTexture()
{
    m_Textures["level-" + std::to_string(m_ActiveLevel)].Bind();
    GetShader("basic").SetUniformMat4f("u_MVP", m_Levels[GetActiveLevel()].GetMVP());
}

void Game::SetEnemies()
{
    m_Enemies.clear();
    auto currentLevelEnemies = m_Levels[GetActiveLevel()].GetEnemies();
    m_RowsEnemiesCount = currentLevelEnemies.size();
    m_ColsEnemiesCount = currentLevelEnemies[0].size();

    for (int i = 0; i < m_RowsEnemiesCount; i++) {
        for (int j = 0; j < m_ColsEnemiesCount; j++) {
            m_Enemies.emplace_back(ENEMY_PADDING_X + j * ENEMY_RENDER_DISTANCE_X,
                                   Game::ScreenHeight - ENEMY_PADDING_Y - i * ENEMY_RENDER_DISTANCE_Y, 1,
                                   "enemy_" + std::to_string(currentLevelEnemies[i][j]),
                                   "missile-enemy", 0,
                                   ENEMY_WIDTH, ENEMY_HEIGHT, 0.5f, 10.0f);
        }
    }
}

unsigned int Game::GetActiveLevel()
{
    return m_ActiveLevel - 1;
}

void Game::SetState(GameState state)
{
    State = state;
}

void Game::AddShader(const Shader *shader, const std::string &name)
{
    m_Shaders[name] = *shader;
}

Shader Game::GetShader(const std::string &name)
{
    return m_Shaders[name];
}

void Game::AddTexture(const Texture *texture, const std::string &name)
{
    m_Textures[name] = *texture;
}

Texture Game::GetTexture(const std::string &name)
{
    return m_Textures[name];
}

void Game::LoadShader(const std::string &path, const std::string &name)
{
    Shader *shader = new Shader(path);
    AddShader(shader, name);
}

void Game::LoadTexture(const std::string &path, const std::string &name, unsigned int slot)
{
    Texture *texture = new Texture(path);
    AddTexture(texture, name);
}