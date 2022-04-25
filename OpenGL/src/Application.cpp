#include "Application.h"

#include <iostream>

#include "vendor/imgui/imgui.h"

#include "Engine.h"
#include "Constants.h"
#include "utils/Gizmos.h"
#include "chunks/Chunk.h"
#include "entity/Camera.h"
#include "render/Shader.h"
#include "utils/Instrumentor.h"
#include "chunks/WorldGenerator.h"

Application::Application() {}

void Application::Init()
{
    m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
    m_Camera = std::make_unique<Camera>(70.0f, m_Width, m_Height, 0.01f, 10000.0f);

    m_Generator = std::make_unique<WorldGenerator>();
    m_Generator->PrepareGenerate();

    Gizmos::create();
}

void Application::Update(float dt)
{
    Gizmos::clear();

    m_Camera->Update(1000.0f / dt);

    unsigned short heap = Constants::CHUNK_HEAP;
    {
        glm::ivec3 renderDistance = m_Camera->GetPosition() / glm::vec3(Chunk::c_ChunkSize * Constants::CHUNK_SCALE);
        for (unsigned short z = std::max(renderDistance.z - Constants::RENDER_DISTANCE, 0); z < renderDistance.z + Constants::RENDER_DISTANCE; z++)
        {
            for (unsigned short x = std::max(renderDistance.x - Constants::RENDER_DISTANCE, 0); x < renderDistance.x + Constants::RENDER_DISTANCE; x++)
            {
                if (m_Chunks.find(glm::ivec2(x, z)) != m_Chunks.end())
                    continue;

                InstrumentationTimer timer("Make Chunk!");

                m_Chunks[glm::ivec2(x, z)] = std::make_unique<Chunk>(m_Generator.get(), x, z);

                heap--;
                if (heap <= 0)
                    break;
            }

            if (heap <= 0)
                break;
        }
    }
}

void Application::Render()
{
    m_VisibleChunks = 0;
    for (const auto& chunk : m_Chunks)
    {
        chunk.second->Render(m_Camera.get(), m_Shader.get(), m_VisibleChunks);
    }

    Gizmos::draw(m_Camera->GetViewMatrix(), m_Camera->GetProjMatrix());
}

void Application::ImGuiRender(float dt)
{
    ImGui::Begin("Engine");
    ImGui::Text("Engine Stats:");
    ImGui::Text("Time: %.2f ms (%.1f FPS)", 1000.0f / dt, dt);
    ImGui::Text("Camera Pos: %.2f; %.2f; %.2f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
    ImGui::Text("Camera Rot: %.2f; %.2f; %.2f", m_Camera->GetRotation().x, m_Camera->GetRotation().y, m_Camera->GetRotation().z);
    ImGui::Separator();

    ImGui::Text("Render Stats:");
    ImGui::Text("Active Chunks: %i", m_Chunks.size());
    ImGui::Text("Visible Chunks: %i", m_VisibleChunks);
    ImGui::End();
}

void Application::Close()
{
    m_Generator.reset();
    m_Shader.reset();
    m_Camera.reset();

    for (auto it = m_Chunks.begin(); it != m_Chunks.end(); it++)
    {
        it->second.reset();
    }

    Gizmos::destroy();
}
