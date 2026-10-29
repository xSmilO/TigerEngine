#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include "Scene/Scene.h"
#include <GLFW/glfw3.h>
#include <string>

namespace TigerEngine {
class Application {
  public:
    Application(int width, int height, const std::string &title);
    ~Application();

    void Run();
    Scene &GetScene() {
        return m_CurrentScene;
    }

    static Application &Get() {
        return *s_Instance;
    }

    float TimeScale = 1.0f;
    bool ShowHitboxes = false;

  private:
    static Application *s_Instance;

    void Init();

    GLFWwindow *m_Window;
    int m_Width, m_Height;
    std::string m_Title;
    bool m_IsRunning;

    Scene m_CurrentScene;

    void ImGuiBegin();

    void ImGuiEnd();
};
} // namespace TigerEngine