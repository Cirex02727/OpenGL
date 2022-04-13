#pragma once

#include "Engine.h"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "chunks/Chunk.h"
#include "entity/Camera.h"
#include "render/Shader.h"
#include "chunks/WorldGenerator.h"

class Application : public Engine
{
public:
    Application();

private:
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<Shader> m_Shader;

    std::unique_ptr<WorldGenerator> m_Generator;
    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>> m_Chunks;

    int m_VisibleChunks = 0;

protected:
    void Init();

    void Update(float dt);
    void Render();

    void ImGuiRender(float dt);

    void Close();
};