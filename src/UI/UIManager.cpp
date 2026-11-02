#include "UI/UIManager.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace TigerEngine {
UIManager* UIManager::s_Current = nullptr;
namespace {
unsigned int Compile(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) { char log[1024]{}; glGetShaderInfoLog(shader, sizeof(log), nullptr, log); glDeleteShader(shader); throw std::runtime_error(std::string("UI shader error: ") + log); }
    return shader;
}
}
std::vector<UIElement*> UIManager::OrderedElements() const {
    std::vector<UIElement*> result;
    auto visit = [&](auto&& self, UIElement* element) -> void {
        result.push_back(element);
        for (const auto& item : m_Elements)
            if (item->Parent == element) self(self, item.get());
    };
    for (const auto& item : m_Elements) if (!item->Parent) visit(visit, item.get());
    return result;
}
UIManager::~UIManager() { /* Explicit Shutdown before destroying OpenGL context. */ }
void UIManager::Init() {
    if (m_Program) return;
    constexpr const char* vs = R"(#version 330 core
layout(location = 0) in vec2 a_Pos;
uniform mat4 u_Projection;
void main() { gl_Position = u_Projection * vec4(a_Pos, 0.0, 1.0); })";
    constexpr const char* fs = R"(#version 330 core
out vec4 FragColor;
uniform vec4 u_Color;
void main() { FragColor = u_Color; })";
    unsigned int vertex = Compile(GL_VERTEX_SHADER, vs);
    unsigned int fragment = Compile(GL_FRAGMENT_SHADER, fs);
    m_Program = glCreateProgram();
    glAttachShader(m_Program, vertex);
    glAttachShader(m_Program, fragment);
    glLinkProgram(m_Program);
    glDeleteShader(vertex); glDeleteShader(fragment);
    int ok = GL_FALSE;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &ok);
    if (!ok) throw std::runtime_error("UI shader program link failed");
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void UIManager::Shutdown() {
    m_Elements.clear(); m_PressedButton = nullptr; m_Font = nullptr;
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_Program) glDeleteProgram(m_Program);
    m_VBO = m_VAO = m_Program = 0;
}
UILabel* UIManager::CreateLabel(const std::string& text, glm::vec2 pos) {
    auto* label = Add<UILabel>(); label->Text = text; label->Position = pos; return label;
}
UIPanel* UIManager::CreatePanel(glm::vec2 pos, glm::vec2 size) {
    auto* panel = Add<UIPanel>(); panel->Position = pos; panel->Size = size; return panel;
}
UIButton* UIManager::CreateButton(const std::string& text, glm::vec2 pos, glm::vec2 size) {
    auto* button = Add<UIButton>(); button->Text = text; button->Position = pos; button->Size = size; return button;
}
void UIManager::Update(float mouseX, float mouseY, bool leftDown, bool inputBlocked) {
    glm::vec2 mouse(mouseX, mouseY);
    UIButton* hovered = nullptr;
    auto ordered = OrderedElements();
    if (!inputBlocked) {
        for (auto it = ordered.rbegin(); it != ordered.rend(); ++it) {
            UIElement* element = *it;
            if (!element->IsVisibleInHierarchy()) continue;
            const glm::vec2 p = element->GetScreenPosition(m_ViewportSize);
            if (mouse.x < p.x || mouse.y < p.y || mouse.x >= p.x + element->Size.x || mouse.y >= p.y + element->Size.y) continue;
            // Labels are non-interactive, panels block clicks behind them.
            if (dynamic_cast<UILabel*>(element)) continue;
            hovered = dynamic_cast<UIButton*>(element);
            break;
        }
    }
    for (auto* element : ordered) {
        if (auto* button = dynamic_cast<UIButton*>(element)) {
            button->Hovered = button == hovered;
            button->Pressed = button == m_PressedButton && leftDown && !inputBlocked;
        }
    }
    if (leftDown && !m_LastLeftDown) m_PressedButton = hovered;
    if (!leftDown && m_LastLeftDown) {
        UIButton* clicked = m_PressedButton == hovered ? hovered : nullptr;
        m_PressedButton = nullptr;
        if (clicked && clicked->OnClick) clicked->OnClick();
    }
    if (inputBlocked) m_PressedButton = nullptr;
    m_LastLeftDown = leftDown;
}
void UIManager::DrawRect(glm::vec2 p, glm::vec2 s, glm::vec4 color) const {
    if (s.x <= 0 || s.y <= 0 || !m_Program) return;
    GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
    if (vp[2] <= 0 || vp[3] <= 0) return;
    const auto proj = glm::ortho(0.0f, float(vp[2]), float(vp[3]), 0.0f);
    float v[] = {p.x,p.y, p.x+s.x,p.y, p.x+s.x,p.y+s.y,
                 p.x,p.y, p.x+s.x,p.y+s.y, p.x,p.y+s.y};
    glUseProgram(m_Program);
    glUniformMatrix4fv(glGetUniformLocation(m_Program,"u_Projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniform4fv(glGetUniformLocation(m_Program,"u_Color"), 1, glm::value_ptr(color));
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(v),v);
    glDrawArrays(GL_TRIANGLES,0,6);
    glBindVertexArray(0);
}
void UIManager::Draw() const {
    if (!m_Program) return;
    s_Current = const_cast<UIManager*>(this);
    const GLboolean hadDepth = glIsEnabled(GL_DEPTH_TEST);
    const GLboolean hadBlend = glIsEnabled(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto* element : OrderedElements()) {
        if (!element->IsVisibleInHierarchy()) continue;
        // Draw implementations use Position; draw a temporary copy for resolved screen coords
        // to avoid changing persistent local offsets.
        const glm::vec2 saved = element->Position;
        element->Position = element->GetScreenPosition(m_ViewportSize);
        element->Draw(m_Font);
        element->Position = saved;
    }
    if (hadDepth) glEnable(GL_DEPTH_TEST);
    if (!hadBlend) glDisable(GL_BLEND);
    s_Current = nullptr;
}
} // namespace TigerEngine
