#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#if SP_DEBUG == 1
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#elif defined(SP_RELEASE)
#define GLCall(x) GLClearError();\
    x;\
    GLLogCall(#x, __FILE__, __LINE__)
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const Shader& shader) const;
};