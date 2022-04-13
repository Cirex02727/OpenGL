#include "Engine.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "chunks/Chunk.h"
#include "static/Input.h"
#include "chunks/Blocks.h"
#include "static/Cursor.h"
#include "entity/Camera.h"
#include "render/Loader.h"
#include "render/Renderer.h"
#include "utils/Instrumentor.h"

Engine::Engine() : m_Width(960), m_Height(540) {}

int Engine::Run()
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

    Loader::unloadResources();

    Instrumentor::Get().EndSession();

    glfwTerminate();
    return 0;
}

void Engine::SetSize(int width, int height)
{
    m_Width = width;
    m_Height = height;
}
