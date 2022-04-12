#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Constants.h"
#include "chunks/Chunk.h"
#include "static/Input.h"
#include "chunks/Blocks.h"
#include "static/Cursor.h"
#include "entity/Camera.h"
#include "render/Renderer.h"
#include "utils/FastNoise.h"
#include "utils/Instrumentor.h"
#include "chunks/WorldGenerator.h"
#include "render/VertexBufferLayout.h"

class Engine
{
public:
    int Run()
    {
        Instrumentor::Get().BeginSession("Engine");

        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
        {
            std::cout << "Error Init GLFW!" << std::endl;
            return -1;
        }

        const char* glsl_version = "#version 460";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        /* Create a windowed mode window and its OpenGL context */
        m_Width = 960, m_Height = 540;
        window = glfwCreateWindow(m_Width, m_Height, "Hello World", NULL, NULL);
        if (!window)
        {
            std::cout << "Error Create Window" << std::endl;
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glfwSwapInterval(0); // Set V-Sync

        if (glewInit() != GLEW_OK)
        {
            std::cout << "Error Init Glew!" << std::endl;
            return -1;
        }

        std::cout << "Opengl Version / GPU Type, Version:" << std::endl;
        std::cout << glGetString(GL_VERSION) << std::endl;

        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            GLCall(glEnable(GL_DEPTH_TEST));
            GLCall(glEnable(GL_CULL_FACE));
            GLCall(glCullFace(GL_BACK));

            Renderer render;

            ImGui::CreateContext();

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);
            ImGui::StyleColorsDark();

            glfwSetKeyCallback(window, Input::keyCallback);
            glfwSetMouseButtonCallback(window, Input::mouseCallback);

            Cursor::setWindow(window, m_Width, m_Height);
            Cursor::SetCursorMode(Cursor::CursorMode::DISABLED);

            Blocks::Init();

            Init();

            while (!glfwWindowShouldClose(window))
            {
                GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
                render.Clear();

                Input::Update(window);

                Update(ImGui::GetIO().Framerate);
                Render();

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                ImGuiRender(ImGui::GetIO().Framerate);

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(window);

                if(Input::IsKeyDown(GLFW_KEY_1))
                    Cursor::SetCursorMode(Cursor::CursorMode::NORMAL);
                if (Input::IsKeyDown(GLFW_KEY_2))
                    Cursor::SetCursorMode(Cursor::CursorMode::DISABLED);

                if (Input::IsKeyDown(GLFW_KEY_ESCAPE))
                    glfwSetWindowShouldClose(window, GL_TRUE);

                glfwPollEvents();
            }
        }

        Close();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        Instrumentor::Get().EndSession();

        glfwTerminate();
        return 0;
    }

private:
    int m_Width, m_Height;

    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;

    std::unique_ptr<WorldGenerator> m_Generator;
    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>> m_Chunks;

    int m_VisibleChunks = 0;

    void Init()
    {
        float positions[] = {
            /* Back Face -Z */
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            /* Front Face +Z */
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,

            /* Left Face -X */
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            /* Right Face +X */
             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,

            /* Bottom Face -Y */
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,

            /* Top Face +Y */
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        unsigned int indices[] = {
            /* Back Face -Z */
             0,  1,  2,
             1,  3,  2,

            /* Front Face +Z */
             4,  5,  6,
             5,  7,  6,

            /* Left Face -X */
             8,  9, 10,
             9, 11, 10,

            /* Left Face +X */
            12, 13, 14,
            13, 15, 14,

            /* Left Face -Y */
            16, 17, 18,
            17, 19, 18,

            /* Left Face +Y */
            20, 21, 22,
            21, 23, 22,
        };
        
        m_VAO = std::make_unique<VertexArray>();
        VertexBufferLayout layout;
        layout.Push<float>(3);

        m_VAO->AddBuffer(positions, sizeof(positions), layout);
        m_VAO->AddIndexBuffer(indices, sizeof(indices), GL_UNSIGNED_INT);

        m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
        m_Camera = std::make_unique<Camera>(70.0f, m_Width, m_Height, 0.01f, 10000.0f);

        m_Generator = std::make_unique<WorldGenerator>();
        
        /*
        {
            InstrumentationTimer timer("Create All Chunks!");

            m_Generator->PrepareGenerate();

            std::this_thread::sleep_for(std::chrono::nanoseconds(10));

            for (unsigned short z = 0; z < 40; z++)
            {
                for (unsigned short x = 0; x < 40; x++)
                {
                    InstrumentationTimer timer("Make Chunk!");

                    m_Chunks.push_back(std::make_unique<Chunk>(m_Generator.get(), x, z));
                }
            }
        }
        */
    }

    void Update(float dt)
    {
        m_Camera->Update(1000.0f / dt);

        unsigned short heap = Constants::CHUNK_HEAP;
        {
            glm::ivec3 renderDistance = m_Camera->GetPosition() / glm::vec3(Chunk::c_ChunkSize);
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

    void Render()
    {
        m_VisibleChunks = 0;
        for (const auto& chunk : m_Chunks)
        {
            chunk.second->Render(m_Camera.get(), m_Shader.get(), m_VisibleChunks);
        }

        /*
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
        glm::mat4 mvp = m_Camera->GetProjMatrix() * m_Camera->GetViewMatrix() * model;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_VAO->Bind();
        glDrawElements(GL_TRIANGLES, m_VAO->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
        */
    }

    void ImGuiRender(float dt)
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

    void Close()
    {
        m_VAO.reset();
        m_Generator.reset();
        m_Shader.reset();
        m_Camera.reset();

        for (auto it = m_Chunks.begin(); it != m_Chunks.end(); it++)
        {
            it->second.reset();
        }
    }
};
