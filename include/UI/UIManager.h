#pragma once
#include "UI/UIElement.h"
#include "UI/UILabel.h"
#include "UI/UIPanel.h"
#include "UI/UIButton.h"
#include <memory>
#include <vector>
namespace TigerEngine {
class UIManager {
public:
    UIManager() = default;
    ~UIManager();
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    void Init(); // must be called with OpenGL context active
    void Shutdown();
    UILabel* CreateLabel(const std::string& text, glm::vec2 pos);
    UIPanel* CreatePanel(glm::vec2 pos, glm::vec2 size);
    UIButton* CreateButton(const std::string& text, glm::vec2 pos, glm::vec2 size);
    void SetFont(const Font* font) { m_Font = font; } // non-owning, font must outlive UI render
    void Update(float mouseX, float mouseY, bool leftDown, bool inputBlocked = false);
    void Draw() const;
    void SetViewportSize(glm::vec2 pixelSize) { m_ViewportSize = pixelSize; }
private:
    template<typename T> T* Add() {
        auto ptr = std::make_unique<T>();
        T* result = ptr.get();
        m_Elements.push_back(std::move(ptr));
        return result;
    }
    std::vector<std::unique_ptr<UIElement>> m_Elements;
    const Font* m_Font = nullptr;
    UIButton* m_PressedButton = nullptr;
    bool m_LastLeftDown = false;
    glm::vec2 m_ViewportSize{1280.0f, 720.0f};
    std::vector<UIElement*> OrderedElements() const;
    unsigned int m_Program = 0, m_VAO = 0, m_VBO = 0;
    void DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color) const;
    friend class UIPanel;
    friend class UIButton;
    static UIManager* s_Current;
};
} // namespace TigerEngine
