#include "Core/Application.h"
#include "Graphics/Material.h"
#include "Scene/SpriteRenderer.h"
#include <iostream>

int main() {
    TigerEngine::Application app(1280, 720, "TigerEngine");

    // 1. CREATE MATERIALS (Do this once)
    // TigerEngine::Texture *playerTex = new TigerEngine::Texture("assets/player.png");

    TigerEngine::Material *matPlayer = new TigerEngine::Material(TigerEngine::Vector4(0.0f, 0.8f, 1.0f, 1.0f)); // Cyan
    TigerEngine::Material *matFloor = new TigerEngine::Material(TigerEngine::Vector4(0.0f, 1.0f, 0.0f, 1.0f));

    // 2. CREATE PLAYER
    TigerEngine::GameObject *player = new TigerEngine::GameObject();
    player->Scale = TigerEngine::Vector2(0.5f, 0.5f);
    player->AddComponent<TigerEngine::SpriteRenderer>(matPlayer); // Pass the material!

    // 3. CREATE FLOOR
    TigerEngine::GameObject *floor = new TigerEngine::GameObject();
    floor->Position = TigerEngine::Vector2(0.0f, -0.8f);
    floor->Scale = TigerEngine::Vector2(2.0f, 0.2f);
    floor->AddComponent<TigerEngine::SpriteRenderer>(matFloor); // Pass the material!

    // Add to scene...
    app.GetScene().AddObject(player);
    app.GetScene().AddObject(floor);

    try {
        std::cout << "Starting Engine Loop..." << std::endl;
        app.Run();
    } catch(const std::exception &e) {
        // If C++ throws a fatal error, we catch it here and print it!
        std::cout << "\nFATAL ENGINE CRASH: " << e.what() << "\n" << std::endl;
    } catch(...) {
        std::cout << "\nUNKNOWN FATAL CRASH OCCURRED!\n" << std::endl;
    }

    // Force the console window to stay open so you can read the error!
    std::cout << "Press ENTER to exit..." << std::endl;
    std::cin.get();
    return 0;
}