#include "SnakeGame.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "Graphics/Renderer2D.h"
#include "Scene/GameObject.h"
#include "Scene/SpriteAnimator.h"
#include "UI/UIButton.h"
#include "UI/UIPanel.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace TigerDemo {
using namespace TigerEngine;

namespace {
void Quad(Point2D position, Point2D size, Vector4 color) {
    auto transform = glm::translate(Matrix4{1.0f}, Vector3{position, 0.0f}) *
                     glm::scale(Matrix4{1.0f}, Vector3{size, 1.0f});
    Renderer2D::DrawQuad(transform, color);
}

void Style(UIButton* button, bool primary) {
    button->Background = primary ? Vector4{0.17f, 0.72f, 0.48f, 1.0f}
                                 : Vector4{0.15f, 0.21f, 0.29f, 1.0f};
    button->HoverBackground = primary ? Vector4{0.24f, 0.85f, 0.59f, 1.0f}
                                      : Vector4{0.23f, 0.32f, 0.42f, 1.0f};
    button->PressBackground = {0.10f, 0.43f, 0.32f, 1.0f};
    button->TextColor = {0.96f, 0.99f, 1.0f, 1.0f};
    button->SetAnchor(UIElement::Anchor::Center);
}
}

SnakeGame::SnakeGame(Application& app, GameObject& head, std::shared_ptr<Font> font,
                     std::shared_ptr<Font> title, std::shared_ptr<Font> small)
    : m_App(app), m_Head(head), m_Font(std::move(font)),
      m_Title(std::move(title)), m_Small(std::move(small)) {
    auto& ui = app.GetUIManager();
    ui.SetFont(m_Font.get());
    m_Overlay = ui.CreatePanel({0, 0}, {460, 410});
    m_Overlay->SetAnchor(UIElement::Anchor::Center);
    m_Overlay->Color = {0.055f, 0.085f, 0.13f, 0.98f};
    m_Primary = ui.CreateButton("START", {0, 20}, {280, 54});
    m_Menu = ui.CreateButton("MENU GLOWNE", {0, 80}, {280, 50});
    m_Exit = ui.CreateButton("KONIEC", {0, 95}, {280, 50});
    m_Pause = ui.CreateButton("PAUZA [P]", {-24, 24}, {160, 44});
    Style(m_Primary, true);
    Style(m_Menu, false);
    Style(m_Exit, false);
    Style(m_Pause, false);
    m_Pause->SetAnchor(UIElement::Anchor::TopRight);
    m_Primary->SetOnClick([this] {
        if (m_State == State::Paused) SetState(State::Playing);
        else StartRound();
    });
    m_Menu->SetOnClick([this] { SetState(State::Menu); });
    m_Exit->SetOnClick([this] { m_App.Close(); });
    m_Pause->SetOnClick([this] { SetState(State::Paused); });
    SetState(State::Menu);
}

void SnakeGame::StartRound() {
    m_Model.Reset();
    m_Accumulator = 0.0f;
    SetState(State::Playing);
}

void SnakeGame::SetState(State state) {
    m_State = state;
    m_Accumulator = 0.0f;
    auto* animator = m_Head.GetComponent<SpriteAnimator>();
    if (animator) {
        if (state == State::Playing) animator->Play("Idle", true);
        else animator->Stop();
    }
    UpdateInterface();
    UpdateHead();
    const char* names[] = {"Menu", "Playing", "Paused", "Lost", "Won"};
    std::cout << "TigerSnake: " << names[static_cast<int>(state)] << std::endl;
}

void SnakeGame::UpdateInterface() {
    const bool playing = m_State == State::Playing;
    m_Overlay->Visible = !playing;
    m_Primary->Visible = !playing;
    m_Exit->Visible = m_State == State::Menu;
    m_Menu->Visible = m_State != State::Menu;
    m_Pause->Visible = playing;
    m_Primary->Text = m_State == State::Menu ? "START" :
                      m_State == State::Paused ? "WZNOW" : "JESZCZE RAZ";
    m_Menu->SetAnchor(playing ? UIElement::Anchor::TopLeft : UIElement::Anchor::Center);
    m_Menu->Size = playing ? Point2D{160, 44} : Point2D{280, 50};
    m_Menu->Position = playing ? Point2D{24, 24} : Point2D{0, 85};
    m_Menu->Text = playing ? "MENU [M]" : "MENU GLOWNE";
}

bool SnakeGame::Pressed(KeyCode key, bool& previous) {
    const bool down = Input::IsKeyPressed(key);
    const bool pressed = down && !previous;
    previous = down;
    return pressed;
}

void SnakeGame::Update(float deltaTime) {
    const float dt = std::isfinite(deltaTime) ? std::clamp(deltaTime, 0.0f, 0.25f) : 0.0f;
    m_Time += dt;
    const bool enter = Pressed(Key::Enter, m_EnterHeld);
    const bool restart = Pressed(Key::R, m_RestartHeld);
    const bool pause = Pressed(Key::P, m_PauseHeld);
    const bool space = Pressed(Key::Space, m_SpaceHeld);
    const bool menu = Pressed(Key::M, m_MenuHeld);
    const std::array<bool, 4> held{
        Input::IsKeyPressed(Key::Up) || Input::IsKeyPressed(Key::W),
        Input::IsKeyPressed(Key::Down) || Input::IsKeyPressed(Key::S),
        Input::IsKeyPressed(Key::Left) || Input::IsKeyPressed(Key::A),
        Input::IsKeyPressed(Key::Right) || Input::IsKeyPressed(Key::D)
    };
    const std::array<Cell, 4> directions{{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}};
    if (m_State == State::Playing) {
        for (std::size_t i = 0; i < held.size(); ++i)
            if (held[i] && !m_DirectionHeld[i]) m_Model.QueueDirection(directions[i]);
    }
    m_DirectionHeld = held;

    if (menu && m_State != State::Menu) SetState(State::Menu);
    else if (m_State == State::Menu && enter) StartRound();
    else if ((m_State == State::Lost || m_State == State::Won) && (restart || enter)) StartRound();
    else if (m_State == State::Playing && (pause || space)) SetState(State::Paused);
    else if (m_State == State::Paused && (pause || space || enter)) SetState(State::Playing);

    if (m_State == State::Playing) {
        m_Accumulator += dt;
        float interval = std::max(0.075f, 0.16f - 0.003f * (m_Model.Score() / 10));
        while (m_Accumulator >= interval && m_State == State::Playing) {
            m_Accumulator -= interval;
            const auto result = m_Model.Step();
            if (result == StepResult::Lost) SetState(State::Lost);
            if (result == StepResult::Won) SetState(State::Won);
            interval = std::max(0.075f, 0.16f - 0.003f * (m_Model.Score() / 10));
        }
    }
    UpdateHead();
}

float SnakeGame::TileSize() const {
    const float aspect = m_App.GetAspectRatio();
    return std::max(0.005f, std::min(1.20f / m_Model.Height(),
                                    (aspect * 1.8f - 0.18f) / m_Model.Width()));
}

Point2D SnakeGame::WorldPosition(Cell cell) const {
    const float tile = TileSize();
    return {-m_Model.Width() * tile * 0.5f + (cell.X + 0.5f) * tile,
            0.50f - (cell.Y + 0.5f) * tile};
}

void SnakeGame::UpdateHead() {
    const float tile = TileSize();
    m_Head.Position = WorldPosition(m_Model.Body().front());
    m_Head.Scale = m_State == State::Menu ? Point2D{0.0f} : Point2D{tile * 0.94f};
    const auto direction = m_Model.Direction();
    m_Head.Rotation = glm::degrees(std::atan2(float(-direction.Y), float(direction.X)));
}

void SnakeGame::Draw() {
    const float tile = TileSize();
    const float width = tile * m_Model.Width();
    const float height = tile * m_Model.Height();
    const Point2D center{0, 0.50f - height * 0.5f};
    Quad(center, {width + 0.035f, height + 0.035f}, {0.16f, 0.27f, 0.33f, 1.0f});
    Quad(center, {width, height}, {0.045f, 0.073f, 0.10f, 1.0f});
    for (int x = 1; x < m_Model.Width(); ++x) {
        const float px = -width * 0.5f + x * tile;
        Renderer2D::DrawLine({px, 0.50f}, {px, 0.50f - height}, {0.075f, 0.11f, 0.14f, 1});
    }
    for (int y = 1; y < m_Model.Height(); ++y) {
        const float py = 0.50f - y * tile;
        Renderer2D::DrawLine({-width * 0.5f, py}, {width * 0.5f, py}, {0.075f, 0.11f, 0.14f, 1});
    }
    if (m_State == State::Menu) return;
    const auto& body = m_Model.Body();
    for (std::size_t i = body.size(); i-- > 1;) {
        const float shade = 0.12f * float(i) / float(body.size());
        Quad(WorldPosition(body[i]), {tile * 0.86f, tile * 0.86f},
             {0.19f - shade, 0.80f - shade, 0.52f - shade, 1.0f});
    }
    if (m_Model.HasFood()) {
        const float pulse = 0.71f + 0.05f * std::sin(m_Time * 4.0f);
        Renderer2D::DrawPolygon(WorldPosition(m_Model.Food()), {tile * pulse, tile * pulse},
                                6, {1.0f, 0.65f, 0.20f, 1.0f}, m_Time * 80.0f);
    }
}

void SnakeGame::Centered(const std::string& text, float y, const Font& font, Vector4 color) const {
    const auto size = m_App.GetFramebufferSize();
    const auto bounds = font.MeasureVisibleBounds(text);
    const Point2D position{(size.x - bounds.Size().x) * 0.5f - bounds.Min.x,
                           y - bounds.Min.y};
    FontRenderer::DrawText(font, text, position, color);
}

void SnakeGame::OnGUI() {
    const auto size = m_App.GetFramebufferSize();
    if (size.x <= 0 || size.y <= 0) return;
    const float middle = size.y * 0.5f;
    if (m_State == State::Playing) {
        Centered("TIGER SNAKE", 26, *m_Font, {0.94f, 0.98f, 1, 1});
        Centered("WYNIK  " + std::to_string(m_Model.Score()), 64, *m_Font,
                  {0.33f, 0.91f, 0.65f, 1});
    } else {
        const std::string title = m_State == State::Menu ? "TIGER SNAKE" :
                                  m_State == State::Paused ? "PAUZA" :
                                  m_State == State::Won ? "WYGRANA!" : "KONIEC GRY";
        Centered(title, middle - 143, *m_Title, {0.93f, 0.99f, 1, 1});
        if (m_State == State::Menu) {
            Centered("Male demo. Klasyczna rozgrywka.", middle - 71, *m_Small,
                      {0.61f, 0.73f, 0.80f, 1});
            Centered("Zbieraj jedzenie. Unikaj scian i ogona.", middle - 38, *m_Small,
                      {0.61f, 0.73f, 0.80f, 1});
        } else {
            Centered("WYNIK  " + std::to_string(m_Model.Score()), middle - 65, *m_Font,
                      {1.0f, 0.70f, 0.28f, 1});
            if (m_State == State::Paused)
                Centered("P / Spacja / Enter: wroc do gry", middle - 27, *m_Small,
                          {0.61f, 0.73f, 0.80f, 1});
        }
    }
    Centered("WASD / strzalki: ruch    P / Spacja: pauza    M: menu    Esc: koniec",
              size.y - 43.0f, *m_Small, {0.56f, 0.68f, 0.76f, 1});
}
}
