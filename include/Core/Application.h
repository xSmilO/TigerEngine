#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include "Scene/Scene.h"
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <stdexcept>
#include "Graphics/FontRenderer.h"
#include "UI/UIManager.h"

namespace TigerEngine {
class Application {
  public:

    Application(int width, int height, const std::string &title);
    ~Application();

    void Run();
    void Close() { m_IsRunning = false; }
    Scene &GetScene() {
        return m_CurrentScene;
    }

    UIManager& GetUIManager() { return m_UIManager; }

    static Application &Get() {
        if (!s_Instance) throw std::logic_error("No active Application");
        return *s_Instance;
    }

    float TimeScale = 1.0f;
    bool ShowHitboxes = false;

    void SetTargetFPS(int fps);
    int GetTargetFPS() const { return m_TargetFPS; }

    void SetVSync(bool enabled);
    bool IsVSyncEnabled() const { return m_VSync; }

    Point2D GetFramebufferSize() const { return m_FramebufferSize; }
    float GetAspectRatio() const {
        return m_FramebufferSize.y > 0.0f ? m_FramebufferSize.x / m_FramebufferSize.y : 1.0f;
    }

    void SetClearColor(const Vector4& color) { m_ClearColor = color; }
    const Vector4& GetClearColor() const { return m_ClearColor; }

  private:
    static Application *s_Instance;

    void Init();
    void Shutdown();
    void UpdateFramebufferSize(int width, int height);

    GLFWwindow *m_Window = nullptr;
    bool m_GLFWInitialized = false;
    bool m_GLReady = false;
    bool m_ImGuiGLFWReady = false;
    bool m_ImGuiGLReady = false;
    int m_Width, m_Height;
    Point2D m_FramebufferSize{0.0f};
    std::string m_Title;
    bool m_IsRunning;
    int m_TargetFPS = 0;
    bool m_VSync = true;
    Vector4 m_ClearColor{0.1f, 0.1f, 0.15f, 1.0f};

    Scene m_CurrentScene;
    UIManager m_UIManager;
    std::unique_ptr<Font> m_DemoFont;

    void ImGuiBegin();

    void ImGuiEnd();
};
}
