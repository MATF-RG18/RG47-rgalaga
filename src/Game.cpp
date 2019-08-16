#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Spacecraft.h"
#include "Player.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

GLFWwindow* WindowInitialization()
{
    // Initialize GLFW
    if(!glfwInit())
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "RGalaga", nullptr, nullptr);
    if( window == nullptr){
        fprintf(stderr, "Failed to create GLFW window.\n");
        getchar();
        glfwTerminate();
        return nullptr;

    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = (GLboolean) true; // Ensures that all extensions with valid entry points will be exposed
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return nullptr;
    }

    std::cout << "Using GL Version: " << glGetString(GL_VERSION) << std::endl;

    // Ensure capturing the pressing of ESC key
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}

static void key_callback (GLFWwindow * window, int key, int scancode, int action, int mods) {
    auto *player = reinterpret_cast<Player *>(glfwGetWindowUserPointer(window));
    if (player)
        player->HandleKeyPress(key);
}

int main()
{
    GLFWwindow* window = WindowInitialization();
    if (!window)
        return -1;

    // TODO:
    //  -Change hardcoded values
    //  -Fix game loop

    Player player(400, 25, 1, "res/textures/spacecraft.png", 0, 50, 50, window);

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    {
        do {
            VertexArray va;

            VertexBuffer vb(player.getGLPos(), 4 * 4 * sizeof(float));
            IndexBuffer ib(player.getGLInd(), 6);

            glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 model = glm::mat4(1.0f);

            glm::mat4 mvp = projection * view * model;

            VertexBufferLayout layout;
            layout.AddFloat(2);
            layout.AddFloat(2);

            va.AddBuffer(vb, layout);

            Shader shader("res/shaders/Basic.shader");
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);

            Texture texture(player.getTex());
            texture.Bind();
            shader.SetUniform1i("u_Texture", 0);

            Renderer renderer;

            renderer.Clear();

            shader.Bind();

            renderer.Draw(va, ib, shader);

            glfwSetWindowUserPointer(window, &player);

            glfwSwapBuffers(window);
            glfwSwapInterval(1);

            glfwPollEvents();

            glfwSetKeyCallback(window, key_callback);

        } // Check if the ESC key was pressed or the window was closed
        while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
              glfwWindowShouldClose(window) == 0);
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
