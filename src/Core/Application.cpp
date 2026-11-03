#include "Core/Application.h"
#include "Core/AudioSystem.h"
#include "Core/Input.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/FontRenderer.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace TigerEngine {
Application *Application::s_Instance = nullptr;

Application::Application(int width, int height, const std::string &title)
    : m_Width(width), m_Height(height), m_Title(title), m_IsRunning(true) {
    if (s_Instance) throw std::logic_error("Only one Application can be active");
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Application window dimensions must be positive");
    s_Instance = this;
    try {
        Init();
    } catch (...) {
        Shutdown();
        throw;
    }
}

Application::~Application() {
    Shutdown();
}

void Application::Shutdown() {
    m_IsRunning = false;
    if (m_GLReady) {
        glfwMakeContextCurrent(m_Window);
        m_CurrentScene.Clear();
        m_UIManager.Shutdown();
        m_DemoFont.reset();
        if (m_ImGuiGLReady) ImGui_ImplOpenGL3_Shutdown();
        if (m_ImGuiGLFWReady) ImGui_ImplGlfw_Shutdown();
        if (ImGui::GetCurrentContext()) ImGui::DestroyContext();
        FontRenderer::Shutdown();
        Renderer2D::Shutdown();
    }
    m_ImGuiGLReady = m_ImGuiGLFWReady = m_GLReady = false;
    AudioSystem::Shutdown();
    Input::Init(nullptr);
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    if (m_GLFWInitialized) glfwTerminate();
    m_GLFWInitialized = false;
    if (s_Instance == this) s_Instance = nullptr;
}

void Application::Init() {
    glfwSetErrorCallback([](int code, const char* description) {
        std::cerr << "GLFW error " << code << ": " << description << '\n';
    });
    if(!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
    m_GLFWInitialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if(!m_Window) throw std::runtime_error("Failed to create the OpenGL 3.3 window");
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(m_VSync ? 1 : 0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to load OpenGL functions");
    m_GLReady = true;
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(m_Window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    m_CurrentScene.OnWindowResized(framebufferWidth, framebufferHeight);

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

    Renderer2D::Init();
    FontRenderer::Init();
    m_UIManager.Init();
    try { m_DemoFont = std::make_unique<Font>("assets/fonts/main.ttf", 36.0f); }
    catch (const std::exception& e) { std::cerr << "Font demo skipped: " << e.what() << "\n"; }
    if (m_DemoFont) m_UIManager.SetFont(m_DemoFont.get());
    AudioSystem::Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(m_Window, true))
        throw std::runtime_error("Failed to initialize ImGui GLFW backend");
    m_ImGuiGLFWReady = true;
    if (!ImGui_ImplOpenGL3_Init("#version 330"))
        throw std::runtime_error("Failed to initialize ImGui OpenGL backend");
    m_ImGuiGLReady = true;

    Input::Init(m_Window);
}

void Application::SetTargetFPS(int fps) {
    m_TargetFPS = fps > 0 ? fps : 0;
    // Disable vsync when explicitly capping FPS so the requested limit is not
    // constrained by the display's swap interval. Call SetVSync(true) to opt back in.
    if (m_TargetFPS > 0) SetVSync(false);
}

void Application::SetVSync(bool enabled) {
    m_VSync = enabled;
    if (m_Window) {
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(enabled ? 1 : 0);
    }
}

void Application::Run() {
    if (!m_Window || !m_GLReady)
        throw std::logic_error("Cannot run an uninitialized Application");
    double lastFrameTime = glfwGetTime();

    try {
        m_CurrentScene.Start();

        while(!glfwWindowShouldClose(m_Window) && m_IsRunning) {
            double currentFrameTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
            lastFrameTime = currentFrameTime;

            if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                m_IsRunning = false;

            float scaledDeltaTime = deltaTime * TimeScale;

            m_CurrentScene.Update(scaledDeltaTime);

            glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
            glClear(GL_COLOR_BUFFER_BIT);

            m_CurrentScene.Draw();
            ImGuiBegin();
            double mx = 0.0, my = 0.0;
            glfwGetCursorPos(m_Window, &mx, &my);
            int ww = 1, wh = 1, fw = 1, fh = 1;
            glfwGetWindowSize(m_Window, &ww, &wh);
            glfwGetFramebufferSize(m_Window, &fw, &fh);
            m_UIManager.SetViewportSize({float(fw), float(fh)});
            if (ww > 0 && wh > 0) {
                m_UIManager.Update(float(mx * fw / ww), float(my * fh / wh),
                    glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS,
                    ImGui::GetIO().WantCaptureMouse);
            }
            m_UIManager.Draw();
            m_CurrentScene.OnGUI();
            ImGuiEnd();

            glfwSwapBuffers(m_Window);
            glfwPollEvents();

            // Limit completed frames, including CPU update + rendering + swap.
            // Sleep until the current frame has consumed at least 1 / targetFPS.
            // VSync may also impose a slower rate if explicitly re-enabled.
            if (m_TargetFPS > 0) {
                const double frameSeconds = 1.0 / static_cast<double>(m_TargetFPS);
                const double remaining = frameSeconds - (glfwGetTime() - currentFrameTime);
                if (remaining > 0.0) {
                    std::this_thread::sleep_for(std::chrono::duration<double>(remaining));
                }
            }
        }
    } catch(const std::exception &e) {
        std::cout << "\nFATAL ENGINE CRASH: " << e.what() << "\n" << std::endl;
    } catch(...) {
        std::cout << "\nUNKNOWN FATAL CRASH OCCURRED!\n" << std::endl;
    }
}

void Application::ImGuiBegin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Application::ImGuiEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace TigerEngine