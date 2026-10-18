#include "Core/Application.h"
#include "Graphics/Renderer2D.h"
#include <iostream>

namespace TigerEngine {
Application::Application(int width, int height, const std::string &title)
    : m_Width(width), m_Height(height), m_Title(title), m_IsRunning(true) {
    Init();
}

Application::~Application() {
    Renderer2D::Shutdown();
    glfwTerminate();
}

void Application::Init() {
    if(!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if(!m_Window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_Window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return;

    Renderer2D::Init();
}

void Application::Run() {
    float lastFrameTime = 0.0f;
    m_CurrentScene.Start();

    while(!glfwWindowShouldClose(m_Window) && m_IsRunning) {
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            m_IsRunning = false;

        m_CurrentScene.Update(deltaTime);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_CurrentScene.Draw();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}
} // namespace TigerEngine