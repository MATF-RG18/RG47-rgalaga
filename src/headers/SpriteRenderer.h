#pragma once

#include "Renderer.h"

class SpriteRenderer : public Renderer
{
    private:
        // Render state
        Shader m_shader;
        GLuint m_va;

        // Initializes and configures the quad's buffer and vertex attributes
        void InitRenderData();
    public:
        SpriteRenderer(Shader& shader);
        ~SpriteRenderer();

        // Renders a defined quad texture with given sprite
        void DrawSprite(Texture &texture,
                        glm::vec2 position,
                        glm::vec2 size = glm::vec2(10, 10),
                        GLfloat rotate = 0.0f,
                        glm::vec3 color = glm::vec3(1.0f));
};

