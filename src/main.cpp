#include "Core/Application.h"
#include "Core/AudioSystem.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"
#include "Scene/SpriteRenderer.h"
#include "Game/Snake.h"
#include <iostream>

int main() {
    TigerEngine::Application app(1280, 720, "Snake Game");

    TigerEngine::Texture snakeTexture("assets/czadomen.jpg");
    TigerEngine::Material matBackground(TigerEngine::Vector4(0.1f, 0.1f, 0.1f, 1.0f));
    TigerEngine::Material matBorder(TigerEngine::Vector4(0.2f, 0.75f, 0.9f, 1.0f));
    TigerEngine::Material matVolumeDown(TigerEngine::Vector4(0.9f, 0.55f, 0.15f, 1.0f));
    TigerEngine::Material matMuteButton(TigerEngine::Vector4(0.2f, 0.75f, 0.9f, 1.0f));
    TigerEngine::Material matVolumeUp(TigerEngine::Vector4(0.25f, 0.75f, 0.3f, 1.0f));
    TigerEngine::Material matBombButton(TigerEngine::Vector4(0.9f, 0.2f, 0.15f, 1.0f));
    TigerEngine::Material matApple(TigerEngine::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    TigerEngine::Material matSnake(TigerEngine::Vector4(1.0f, 1.0f, 1.0f, 1.0f), &snakeTexture);
    TigerEngine::Material matTail(TigerEngine::Vector4(0.0f, 0.8f, 0.0f, 1.0f));

    TigerEngine::AudioSystem::PlayMusic("assets/main_music.mp3", true);
    TigerEngine::AudioSystem::SetMusicVolume(0.5f);

    TigerEngine::GameObject* bgObj = new TigerEngine::GameObject();
    bgObj->Scale = TigerEngine::Vector2(10.0f, 10.0f);
    bgObj->AddComponent<TigerEngine::SpriteRenderer>(&matBackground);
    app.GetScene().AddObject(bgObj);

    const float cellSize = Snake::CellSize;
    const float borderThickness = cellSize * 1.5f;
    const float innerHalfWidth = (Snake::BoardHalfWidth + 0.5f) * cellSize;
    const float innerHalfHeight = (Snake::BoardHalfHeight + 0.5f) * cellSize;
    const float borderX = innerHalfWidth + borderThickness * 0.5f;
    const float borderY = innerHalfHeight + borderThickness * 0.5f;
    const auto addBorderSegment = [&app, &matBorder](const TigerEngine::Vector2& position,
                                                      const TigerEngine::Vector2& scale) {
        TigerEngine::GameObject* borderSegment = new TigerEngine::GameObject();
        borderSegment->Position = position;
        borderSegment->Scale = scale;
        borderSegment->AddComponent<TigerEngine::SpriteRenderer>(&matBorder);
        app.GetScene().AddObject(borderSegment);
    };

    addBorderSegment(TigerEngine::Vector2(-borderX, 0.0f),
                     TigerEngine::Vector2(borderThickness, 2.0f * innerHalfHeight + 2.0f * borderThickness));
    addBorderSegment(TigerEngine::Vector2(borderX, 0.0f),
                     TigerEngine::Vector2(borderThickness, 2.0f * innerHalfHeight + 2.0f * borderThickness));
    addBorderSegment(TigerEngine::Vector2(0.0f, -borderY),
                     TigerEngine::Vector2(2.0f * innerHalfWidth + 2.0f * borderThickness, borderThickness));
    addBorderSegment(TigerEngine::Vector2(0.0f, borderY),
                     TigerEngine::Vector2(2.0f * innerHalfWidth + 2.0f * borderThickness, borderThickness));

    const auto addAudioButton = [&app](const TigerEngine::Vector2& position,
                                       TigerEngine::Material* material) {
        TigerEngine::GameObject* button = new TigerEngine::GameObject();
        button->Position = position;
        button->Scale = TigerEngine::Vector2(0.16f, 0.16f);
        button->AddComponent<TigerEngine::SpriteRenderer>(material);
        app.GetScene().AddObject(button);
        return button;
    };

    const float buttonX = -1.35f;
    TigerEngine::GameObject* volumeDownButton = addAudioButton(
        TigerEngine::Vector2(buttonX, 0.45f), &matVolumeDown);
    TigerEngine::GameObject* muteButton = addAudioButton(
        TigerEngine::Vector2(buttonX, 0.20f), &matMuteButton);
    TigerEngine::GameObject* volumeUpButton = addAudioButton(
        TigerEngine::Vector2(buttonX, -0.05f), &matVolumeUp);
    TigerEngine::GameObject* bombButton = addAudioButton(
        TigerEngine::Vector2(buttonX, -0.30f), &matBombButton);

    TigerEngine::GameObject* appleObj = new TigerEngine::GameObject();
    appleObj->Scale = TigerEngine::Vector2(0.1f, 0.1f);
    appleObj->AddComponent<TigerEngine::SpriteRenderer>(&matApple);
    app.GetScene().AddObject(appleObj);

    TigerEngine::GameObject* snakeObj = new TigerEngine::GameObject();
    snakeObj->Scale = TigerEngine::Vector2(0.1f, 0.1f);
    snakeObj->AddComponent<TigerEngine::SpriteRenderer>(&matSnake);

    Snake* snakeLogic = snakeObj->AddComponent<Snake>();
    snakeLogic->SetApple(appleObj);
    snakeLogic->SetBackgroundMaterial(&matBackground);
    snakeLogic->SetAudioButtons(volumeDownButton, muteButton, volumeUpButton, bombButton, &matMuteButton);
    app.GetScene().AddObject(snakeObj);

    for (int i = 0; i < Snake::WinScore - 1; i++) {
        TigerEngine::GameObject* tailSegment = new TigerEngine::GameObject();
        tailSegment->Scale = TigerEngine::Vector2(0.1f, 0.1f);
        tailSegment->Position = TigerEngine::Vector2(99.0f, 99.0f);
        tailSegment->AddComponent<TigerEngine::SpriteRenderer>(&matTail);

        app.GetScene().AddObject(tailSegment);
        snakeLogic->AddTailSegmentVisual(tailSegment);
    }

    std::cout << "Sterowanie: WASD lub strzalki; kliknij kierunek na planszy. "
                 "Przyciski po lewej: ciszej, wycisz, glosniej, bomba. "
                 "Klawisze: -, M, +, B.\n";

    try {
        app.Run();
    } catch (const std::exception& exception) {
        std::cerr << "Błąd podczas uruchamiania gry: " << exception.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Wystąpił nieznany błąd podczas uruchamiania gry." << std::endl;
        return 1;
    }

    return 0;
}