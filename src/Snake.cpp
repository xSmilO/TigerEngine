#include "Game/Snake.h"
#include "Core/AudioSystem.h"
#include "Core/Input.h"
#include "Scene/Scene.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

Snake::Snake()
    : m_RandomGenerator(std::random_device{}()) {
}

TigerEngine::Vector2 Snake::GridToWorld(const GridPosition& position) const {
    return TigerEngine::Vector2(position.x * CellSize, position.y * CellSize);
}

void Snake::SetAudioButtons(TigerEngine::GameObject* volumeDown, TigerEngine::GameObject* mute,
                            TigerEngine::GameObject* volumeUp, TigerEngine::GameObject* bomb,
                            TigerEngine::Material* muteMaterial) {
    m_VolumeDownButton = volumeDown;
    m_MuteButton = mute;
    m_VolumeUpButton = volumeUp;
    m_BombButton = bomb;
    m_MuteButtonMaterial = muteMaterial;
    UpdateMuteButtonVisual();
}

TigerEngine::Vector2 Snake::GetMouseWorldPosition() const {
    if (!Owner || !Owner->CurrentScene || !Owner->CurrentScene->GetActiveCamera()) {
        return TigerEngine::Vector2(0.0f, 0.0f);
    }

    const glm::mat4 inverseViewProjection =
        glm::inverse(Owner->CurrentScene->GetActiveCamera()->GetViewProjectionMatrix());
    const glm::vec4 worldPosition = inverseViewProjection * glm::vec4(
        TigerEngine::Input::GetMouseX_NDC(), TigerEngine::Input::GetMouseY_NDC(), 0.0f, 1.0f);

    if (std::abs(worldPosition.w) < 0.0001f) return TigerEngine::Vector2(0.0f, 0.0f);
    return TigerEngine::Vector2(worldPosition.x / worldPosition.w, worldPosition.y / worldPosition.w);
}

bool Snake::IsInsideButton(const TigerEngine::GameObject* button,
                           const TigerEngine::Vector2& position) const {
    if (!button) return false;
    return std::abs(position.x - button->Position.x) <= button->Scale.x * 0.5f &&
           std::abs(position.y - button->Position.y) <= button->Scale.y * 0.5f;
}

void Snake::RequestDirection(int x, int y) {
    if (x == -m_Direction.x && y == -m_Direction.y) return;
    m_NextDirection = {x, y};
}

void Snake::AdjustMusicVolume(float amount) {
    m_MusicVolume = std::clamp(m_MusicVolume + amount, 0.0f, 1.0f);
    TigerEngine::AudioSystem::SetMusicVolume(m_IsMusicMuted ? 0.0f : m_MusicVolume);
    UpdateMuteButtonVisual();
}

void Snake::ToggleMusicMute() {
    m_IsMusicMuted = !m_IsMusicMuted;
    TigerEngine::AudioSystem::SetMusicVolume(m_IsMusicMuted ? 0.0f : m_MusicVolume);
    UpdateMuteButtonVisual();
}

void Snake::PlayBombSound() {
    TigerEngine::AudioSystem::PlaySound2D("assets/bomb.wav");
}

void Snake::UpdateMuteButtonVisual() {
    if (!m_MuteButtonMaterial) return;
    m_MuteButtonMaterial->TintColor = m_IsMusicMuted || m_MusicVolume <= 0.0f
        ? TigerEngine::Vector4(0.35f, 0.35f, 0.35f, 1.0f)
        : TigerEngine::Vector4(0.2f, 0.75f, 0.9f, 1.0f);
}

void Snake::HandleMouseClick() {
    const TigerEngine::Vector2 mousePosition = GetMouseWorldPosition();
    if (IsInsideButton(m_VolumeDownButton, mousePosition)) {
        AdjustMusicVolume(-0.1f);
        return;
    }
    if (IsInsideButton(m_MuteButton, mousePosition)) {
        ToggleMusicMute();
        return;
    }
    if (IsInsideButton(m_VolumeUpButton, mousePosition)) {
        AdjustMusicVolume(0.1f);
        return;
    }
    if (IsInsideButton(m_BombButton, mousePosition)) {
        PlayBombSound();
        return;
    }
    if (m_IsGameOver || m_IsVictory || m_Body.empty()) return;

    const TigerEngine::Vector2 head = GridToWorld(m_Body.front());
    const float dx = mousePosition.x - head.x;
    const float dy = mousePosition.y - head.y;
    if (std::abs(dx) < 0.01f && std::abs(dy) < 0.01f) return;

    if (std::abs(dx) > std::abs(dy)) {
        RequestDirection(dx > 0.0f ? 1 : -1, 0);
    } else {
        RequestDirection(0, dy > 0.0f ? 1 : -1);
    }
}

void Snake::ResetGame() {
    m_Body.clear();
    m_Body.push_back({0, 0});
    m_Direction = {1, 0};
    m_NextDirection = m_Direction;
    m_MoveTimer = 0.0f;
    m_IsGameOver = false;
    m_IsVictory = false;

    if (Owner) Owner->Position = GridToWorld(m_Body.front());
    RespawnApple();
    UpdateVisuals();

    if (m_BackgroundMat) {
        m_BackgroundMat->TintColor = TigerEngine::Vector4(0.1f, 0.1f, 0.1f, 1.0f);
    }
}

void Snake::RespawnApple() {
    m_HasApple = false;
    if (!m_Apple) return;

    std::vector<GridPosition> freePositions;
    for (int y = -BoardHalfHeight; y <= BoardHalfHeight; ++y) {
        for (int x = -BoardHalfWidth; x <= BoardHalfWidth; ++x) {
            const GridPosition candidate{x, y};
            if (std::find(m_Body.begin(), m_Body.end(), candidate) == m_Body.end()) {
                freePositions.push_back(candidate);
            }
        }
    }

    if (freePositions.empty()) {
        FinishGame(true);
        return;
    }

    std::uniform_int_distribution<size_t> distribution(0, freePositions.size() - 1);
    m_ApplePosition = freePositions[distribution(m_RandomGenerator)];
    m_Apple->Position = GridToWorld(m_ApplePosition);
    m_HasApple = true;
}

void Snake::Update(float deltaTime) {
    if (m_Body.empty()) ResetGame();

    const bool muteKey = TigerEngine::Input::IsKeyPressed(GLFW_KEY_M);
    const bool volumeDownKey = TigerEngine::Input::IsKeyPressed(GLFW_KEY_MINUS) ||
                               TigerEngine::Input::IsKeyPressed(GLFW_KEY_KP_SUBTRACT);
    const bool volumeUpKey = TigerEngine::Input::IsKeyPressed(GLFW_KEY_EQUAL) ||
                             TigerEngine::Input::IsKeyPressed(GLFW_KEY_KP_ADD);
    const bool bombKey = TigerEngine::Input::IsKeyPressed(GLFW_KEY_B);

    if (muteKey && !m_MuteKeyWasPressed) ToggleMusicMute();
    if (volumeDownKey && !m_VolumeDownKeyWasPressed) AdjustMusicVolume(-0.1f);
    if (volumeUpKey && !m_VolumeUpKeyWasPressed) AdjustMusicVolume(0.1f);
    if (bombKey && !m_BombKeyWasPressed) PlayBombSound();

    m_MuteKeyWasPressed = muteKey;
    m_VolumeDownKeyWasPressed = volumeDownKey;
    m_VolumeUpKeyWasPressed = volumeUpKey;
    m_BombKeyWasPressed = bombKey;

    const bool mousePressed = TigerEngine::Input::IsMouseButtonPressed(TigerEngine::Mouse::ButtonLeft);
    if (mousePressed && !m_MouseWasPressed) HandleMouseClick();
    m_MouseWasPressed = mousePressed;

    if (m_IsGameOver || m_IsVictory) {
        if (TigerEngine::Input::IsKeyPressed(TigerEngine::Key::R)) ResetGame();
        return;
    }

    if (TigerEngine::Input::IsKeyPressed(TigerEngine::Key::W) ||
        TigerEngine::Input::IsKeyPressed(TigerEngine::Key::Up))
        RequestDirection(0, 1);
    else if (TigerEngine::Input::IsKeyPressed(TigerEngine::Key::S) ||
             TigerEngine::Input::IsKeyPressed(TigerEngine::Key::Down))
        RequestDirection(0, -1);
    else if (TigerEngine::Input::IsKeyPressed(TigerEngine::Key::A) ||
             TigerEngine::Input::IsKeyPressed(TigerEngine::Key::Left))
        RequestDirection(-1, 0);
    else if (TigerEngine::Input::IsKeyPressed(TigerEngine::Key::D) ||
             TigerEngine::Input::IsKeyPressed(TigerEngine::Key::Right))
        RequestDirection(1, 0);

    if (deltaTime > 0.0f) m_MoveTimer += deltaTime;
    while (m_MoveTimer >= m_MoveInterval && !m_IsGameOver && !m_IsVictory) {
        m_MoveTimer -= m_MoveInterval;
        MoveOneCell();
    }
}

void Snake::MoveOneCell() {
    const GridPosition nextHead{
        m_Body.front().x + m_NextDirection.x,
        m_Body.front().y + m_NextDirection.y
    };

    if (nextHead.x < -BoardHalfWidth || nextHead.x > BoardHalfWidth ||
        nextHead.y < -BoardHalfHeight || nextHead.y > BoardHalfHeight) {
        FinishGame(false);
        return;
    }

    const bool willEat = m_HasApple && nextHead == m_ApplePosition;
    for (size_t i = 1; i < m_Body.size(); ++i) {
        const bool tailMovesAway = !willEat && i == m_Body.size() - 1;
        if (!tailMovesAway && m_Body[i] == nextHead) {
            FinishGame(false);
            return;
        }
    }

    m_Direction = m_NextDirection;
    m_Body.insert(m_Body.begin(), nextHead);
    if (!willEat) {
        m_Body.pop_back();
    } else {
        std::cout << "Mniam! Dlugosc weza: " << m_Body.size() << " / " << WinScore << std::endl;
        if (m_Body.size() >= WinScore) {
            FinishGame(true);
        } else {
            RespawnApple();
        }
    }

    if (Owner) Owner->Position = GridToWorld(m_Body.front());
    UpdateVisuals();
}

void Snake::UpdateVisuals() {
    for (size_t i = 0; i < m_TailObjects.size(); ++i) {
        if (!m_TailObjects[i]) continue;

        if (i + 1 < m_Body.size()) {
            m_TailObjects[i]->Position = GridToWorld(m_Body[i + 1]);
        } else {
            m_TailObjects[i]->Position = TigerEngine::Vector2(99.0f, 99.0f);
        }
    }
}

void Snake::FinishGame(bool victory) {
    m_IsGameOver = !victory;
    m_IsVictory = victory;

    if (m_BackgroundMat) {
        m_BackgroundMat->TintColor = victory
            ? TigerEngine::Vector4(0.8f, 0.8f, 0.0f, 1.0f)
            : TigerEngine::Vector4(0.8f, 0.0f, 0.0f, 1.0f);
    }

    if (victory) {
        std::cout << "WYGRANA! Wciśnij R, aby zagrać ponownie." << std::endl;
    } else {
        PlayBombSound();
        std::cout << "PRZEGRANA! Wciśnij R, aby zagrać ponownie." << std::endl;
    }
}