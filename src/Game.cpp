#include "Game.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Game::Game(unsigned int width, unsigned int height, GLFWwindow *window)
    : m_State(GAME_ACTIVE), m_Keys(), m_Width(width), m_Height(height), m_Window(window),
      m_Player(400, 30, 1, "player", 0.0f, 50, 50, 2.0f, window),
      m_Projection(glm::ortho( 0.0f, static_cast<float>(m_Width),
                               0.0f, static_cast<float>(m_Height),
                              -1.0f, 1.0f)),
      m_View(glm::mat4(1.0f))
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
        player->HandleKeyPress(key);
}

void Game::ProcessInput(GLfloat dt)
{

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
//    m_Textures[name].Bind(slot);
}

void Game::GameLoop()
{
    LoadShader("res/shaders/Basic.shader", "basic");

    LoadTexture("res/textures/spacecraft.png", "player", 0);
    LoadTexture("res/textures/enemy-type-01.png", "enemy_01", 0);
    LoadTexture("res/textures/enemy-type-02.png", "enemy_02", 0);
    LoadTexture("res/textures/enemy-type-03.png", "enemy_03", 0);
    LoadTexture("res/textures/enemy-type-04.png", "enemy_04", 0);
    LoadTexture("res/textures/enemy-type-05.png", "enemy_05", 0);
    m_Textures["enemy_03"].Bind();
    GetShader("basic").SetUniform1i("u_Texture", 0);

    Enemy enemy(0, 0, 1, "enemy_01", 0, 20, 20, 2.0f);

    Renderer renderer;

    VertexArray va;

    float vertices[] = {
        // Pos        // Tex
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexBuffer vb(vertices, 4 * 4 * sizeof(float));
    IndexBuffer ib(indices, 6);

    VertexBufferLayout layout;
    layout.AddFloat(2);
    layout.AddFloat(2);

    va.AddBuffer(vb, layout);

    do {
        renderer.Clear();

        GetShader("basic").Bind();
        m_Textures["player"].Bind();
        GetShader("basic").SetUniformMat4f("u_MVP", m_Player.GetMVP());
        renderer.Draw(va, ib, GetShader("basic"));

        GetShader("basic").Bind();
        enemy.Transform(glm::vec2(400, 550), glm::vec2(30, 30), 0.0f);
        m_Textures[enemy.GetTex()].Bind();
        GetShader("basic").SetUniformMat4f("u_MVP", enemy.GetMVP());
        renderer.Draw(va, ib, GetShader("basic"));

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
