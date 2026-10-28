#pragma once
#include <random>
#include <vector>
#include "Scene/GameObject.h"
#include "Graphics/Material.h"

class Snake : public TigerEngine::Component {
public:
    static constexpr int WinScore = 15;
    static constexpr float CellSize = 0.1f;
    static constexpr int BoardHalfWidth = 9;
    static constexpr int BoardHalfHeight = 7;

    Snake();
    void Update(float deltaTime) override;

    void SetApple(TigerEngine::GameObject* apple) { m_Apple = apple; }

    void AddTailSegmentVisual(TigerEngine::GameObject* segment) { m_TailObjects.push_back(segment); }
    void SetBackgroundMaterial(TigerEngine::Material* bgMat) { m_BackgroundMat = bgMat; }
    void SetAudioButtons(TigerEngine::GameObject* volumeDown, TigerEngine::GameObject* mute,
                         TigerEngine::GameObject* volumeUp, TigerEngine::GameObject* bomb,
                         TigerEngine::Material* muteMaterial);

private:
    struct GridPosition {
        int x;
        int y;

        bool operator==(const GridPosition& other) const {
            return x == other.x && y == other.y;
        }
    };

    std::mt19937 m_RandomGenerator;
    std::vector<GridPosition> m_Body;
    GridPosition m_Direction{1, 0};
    GridPosition m_NextDirection{1, 0};

    float m_MoveTimer = 0.0f;
    float m_MoveInterval = 0.15f;

    TigerEngine::GameObject* m_Apple = nullptr;
    GridPosition m_ApplePosition{0, 0};
    bool m_HasApple = false;

    std::vector<TigerEngine::GameObject*> m_TailObjects;
    TigerEngine::Material* m_BackgroundMat = nullptr;
    TigerEngine::GameObject* m_VolumeDownButton = nullptr;
    TigerEngine::GameObject* m_MuteButton = nullptr;
    TigerEngine::GameObject* m_VolumeUpButton = nullptr;
    TigerEngine::GameObject* m_BombButton = nullptr;
    TigerEngine::Material* m_MuteButtonMaterial = nullptr;

    bool m_IsGameOver = false;
    bool m_IsVictory = false;
    float m_MusicVolume = 0.5f;
    bool m_IsMusicMuted = false;
    bool m_MouseWasPressed = false;
    bool m_MuteKeyWasPressed = false;
    bool m_VolumeDownKeyWasPressed = false;
    bool m_VolumeUpKeyWasPressed = false;
    bool m_BombKeyWasPressed = false;

    TigerEngine::Vector2 GridToWorld(const GridPosition& position) const;
    TigerEngine::Vector2 GetMouseWorldPosition() const;
    void HandleMouseClick();
    bool IsInsideButton(const TigerEngine::GameObject* button, const TigerEngine::Vector2& position) const;
    void RequestDirection(int x, int y);
    void AdjustMusicVolume(float amount);
    void ToggleMusicMute();
    void PlayBombSound();
    void UpdateMuteButtonVisual();
    void MoveOneCell();
    void UpdateVisuals();
    void FinishGame(bool victory);
    void RespawnApple();
    void ResetGame();
};