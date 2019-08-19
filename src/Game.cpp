#include "Game.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Game::Game(unsigned int width, unsigned int height, GLFWwindow *window)
    : m_State(GAME_ACTIVE), m_Keys(), m_Width(width), m_Height(height), m_Window(window),
      m_Player(Player(0, 0, 1, "res/textures/spacecraft.png", 0, 50, 50, window)),
      m_Projection(glm::ortho( 0.0f, static_cast<float>(m_Width),
                               0.0f, static_cast<float>(m_Height),
                               -1.0f, 1.0f)),
      m_View(glm::mat4(1.0f))
{
    // Add the pointer to Game so the Game's methods could be called from ProcessInput
//    glfwSetWindowUserPointer(window, this);
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

    LoadTexture(m_Player.GetTex(), "player", 0);
    LoadTexture("res/textures/enemy-type-01.png", "enemy_01", 0);
    LoadTexture("res/textures/enemy-type-02.png", "enemy_02", 0);
    LoadTexture("res/textures/enemy-type-03.png", "enemy_03", 0);
    LoadTexture("res/textures/enemy-type-04.png", "enemy_04", 0);
    LoadTexture("res/textures/enemy-type-05.png", "enemy_05", 0);

    GetShader("basic").SetUniform1i("u_Texture", 0);

    Renderer renderer;

    VertexArray va;

    VertexBuffer vb(m_Player.GetGLPos(), 4 * 4 * sizeof(float));
    IndexBuffer ib(m_Player.GetGLInd(), 6);

    VertexBufferLayout layout;
    layout.AddFloat(2);
    layout.AddFloat(2);

    va.AddBuffer(vb, layout);

    glm::vec3 translationA(400,  30, 0);
    glm::vec3 translationB(400, 130, 0);
    glm::mat4 model;
    glm::mat4 mvp;
    do {
        renderer.Clear();

        GetShader("basic").Bind();
        model = glm::translate(glm::mat4(1.0f), translationA);
        mvp = m_Projection * m_View * model;
        GetShader("basic").SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, GetShader("basic"));

        GetShader("basic").Bind();
        model = glm::translate(glm::mat4(1.0f), translationB);
        mvp = m_Projection * m_View * model;
        GetShader("basic").SetUniformMat4f("u_MVP", mvp);
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
