#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (unsigned int error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));
}

void Renderer::Draw(const VertexArray& va, const Shader& shader) const
{
    shader.Bind();

    va.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, va.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
}
