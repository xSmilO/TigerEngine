#pragma once
#include "SnakeModel.h"
#include "Core/Component.h"
#include "Core/Math.h"
#include "Core/KeyCodes.h"
#include "Graphics/FontRenderer.h"
#include <array>
#include <memory>

namespace TigerEngine {
class Application;
class GameObject;
class UIPanel;
class UIButton;
}

namespace TigerDemo {
class SnakeGame : public TigerEngine::Component {
public:
    SnakeGame(TigerEngine::Application& app, TigerEngine::GameObject& head,
              std::shared_ptr<TigerEngine::Font> font,
              std::shared_ptr<TigerEngine::Font> title,
              std::shared_ptr<TigerEngine::Font> small);
    void Update(float deltaTime) override;
    void Draw() override;
    void OnGUI() override;

private:
    enum class State { Menu, Playing, Paused, Lost, Won };
    void StartRound();
    void SetState(State state);
    void UpdateInterface();
    void UpdateHead();
    bool Pressed(TigerEngine::KeyCode key, bool& previous);
    float TileSize() const;
    TigerEngine::Point2D WorldPosition(Cell cell) const;
    void Centered(const std::string& text, float y, const TigerEngine::Font& font,
                  TigerEngine::Vector4 color = TigerEngine::Vector4{1.0f}) const;

    TigerEngine::Application& m_App;
    TigerEngine::GameObject& m_Head;
    std::shared_ptr<TigerEngine::Font> m_Font;
    std::shared_ptr<TigerEngine::Font> m_Title;
    std::shared_ptr<TigerEngine::Font> m_Small;
    SnakeModel m_Model;
    State m_State = State::Menu;
    TigerEngine::UIPanel* m_Overlay;
    TigerEngine::UIButton* m_Primary;
    TigerEngine::UIButton* m_Menu;
    TigerEngine::UIButton* m_Exit;
    TigerEngine::UIButton* m_Pause;
    std::array<bool, 4> m_DirectionHeld{};
    bool m_EnterHeld = false;
    bool m_RestartHeld = false;
    bool m_PauseHeld = false;
    bool m_SpaceHeld = false;
    bool m_MenuHeld = false;
    float m_Accumulator = 0.0f;
    float m_Time = 0.0f;
};
}
