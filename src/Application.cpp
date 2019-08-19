#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

//#include "VertexBuffer.h"
//#include "VertexArray.h"
//#include "IndexBuffer.h"
//#include "Shader.h"
//#include "Renderer.h"
//#include "Texture.h"
//#include "Spacecraft.h"
#include "Game.h"
#include "Player.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

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

int main()
{
    GLFWwindow* window = WindowInitialization();
    if (!window)
        return -1;

    // TODO:
    //  -Change hardcoded values
    //  -Fix game loop


    Game RGalaga(SCREEN_WIDTH, SCREEN_HEIGHT, window);

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    RGalaga.GameLoop();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
