#include "Core/Application.h"
#include "Core/Input.h"
#include "Graphics/Renderer2D.h"
#include <iostream>

namespace TigerEngine {
Application *Application::s_Instance = nullptr;

Application::Application(int width, int height, const std::string &title)
    : m_Width(width), m_Height(height), m_Title(title), m_IsRunning(true) {
    Init();
    s_Instance = this;
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

    // Tell GLFW that the "User Pointer" for this window is our Application instance
    // This allows us to access m_CurrentScene from inside a static C-style callback function
    glfwSetWindowUserPointer(m_Window, this);

    // Set the mouse button callback event
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
        Application *app = (Application *)glfwGetWindowUserPointer(window);

        // Get current mouse position for the event
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if(action == GLFW_PRESS) {
            app->GetScene().OnMousePressed(button, (float)xpos, (float)ypos);
        } else if(action == GLFW_RELEASE) {
            app->GetScene().OnMouseReleased(button, (float)xpos, (float)ypos);
        }
    });

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
        // 1. Tell OpenGL to scale the drawing canvas to the new window size
        glViewport(0, 0, width, height);

        // 2. Tell our Scene to fix the camera's aspect ratio so shapes don't stretch
        Application *app = (Application *)glfwGetWindowUserPointer(window);
        if(app) {
            app->GetScene().OnWindowResized(width, height);
        }
    });

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return;

    Renderer2D::Init();
    Input::Init(m_Window);
}

void Application::Run() {
    float lastFrameTime = 0.0f;

    try {
        std::cout << "Starting Scene..." << std::endl;
        m_CurrentScene.Start();

        std::cout << "Starting Engine Loop..." << std::endl;

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
    } catch(const std::exception &e) {
        std::cout << "\nFATAL ENGINE CRASH: " << e.what() << "\n" << std::endl;
    } catch(...) {
        std::cout << "\nUNKNOWN FATAL CRASH OCCURRED!\n" << std::endl;
    }
}
} // namespace TigerEngine