#include "SnakeGame.h"
#include "Core/Application.h"
#include "Core/AssetManager.h"
#include "Graphics/Material.h"
#include "Scene/SpriteAnimator.h"
#include "Scene/SpriteRenderer.h"
#include <exception>
#include <iostream>
#include <memory>

int main() {
    using namespace TigerEngine;
    try {
        Application app(1280, 800, "TigerSnake - TigerEngine Demo");
        app.SetClearColor({0.025f, 0.043f, 0.068f, 1.0f});
        app.SetTargetFPS(60);
        AssetManager assets;
        auto font = assets.LoadFont("fonts/main.ttf", 24.0f);
        auto title = assets.LoadFont("fonts/main.ttf", 44.0f);
        auto small = assets.LoadFont("fonts/main.ttf", 18.0f);
        auto atlas = assets.LoadTexture("demo/animated_sprite.png");
        auto head = std::make_unique<GameObject>();
        head->Name = "Snake head";
        head->AddComponent<SpriteRenderer>(std::make_shared<Material>(Vector4{1.0f}, atlas));
        auto* animator = head->AddComponent<SpriteAnimator>();
        animator->AddAnimation("Idle", SpriteAnimator::Grid(4, 1, 0, 4, 0.15f));
        auto game = std::make_unique<GameObject>();
        game->Name = "Snake game";
        game->AddComponent<TigerDemo::SnakeGame>(app, *head, font, title, small);
        app.GetScene().AddObject(std::move(game));
        app.GetScene().AddObject(std::move(head));
        app.Run();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "TigerSnake: " << error.what() << '\n';
        return 1;
    }
}
