#pragma once

#include <vector>
#include <GL/glew.h>
#include "Debug.h"


struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        // Choosing switch over simple return, for now, in case of a need for more types
        switch (type) {
            case GL_FLOAT :
                return sizeof(GLfloat);
        }
        return 0;
    }
};

class VertexBufferLayout
{
    private:
        unsigned int m_Stride;
        std::vector<VertexBufferElement> m_Elements;

    public:
        VertexBufferLayout() :
            m_Stride(0)
        {

        }

        void AddFloat(unsigned int count)   { Push(GL_FLOAT, count, GL_FALSE); }

        inline const std::vector<VertexBufferElement> &GetElements() const          { return m_Elements; };
        inline unsigned int GetStride() const                                       { return m_Stride; };

    private:
        void Push(unsigned int type, unsigned int count, unsigned char normalized)
        {
            struct VertexBufferElement vbe = {type, count, normalized};
            m_Elements.push_back(vbe);
            m_Stride += count * VertexBufferElement::GetSizeOfType(type);
        };
};
