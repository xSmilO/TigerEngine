#include "Core/AssetManager.h"
#include "Graphics/Texture.h"
#include "Graphics/FontRenderer.h"
#include <cmath>
#include <stdexcept>

namespace TigerEngine {

AssetManager::TextureHandle AssetManager::LoadTexture(const std::string& path) {
    if (path.empty()) throw std::invalid_argument("Texture path must not be empty");
    auto existing = m_Textures.find(path);
    if (existing != m_Textures.end()) return existing->second;

    auto texture = std::make_shared<Texture>(path);
    if (!texture->IsValid())
        throw std::runtime_error("Unable to load texture: " + path);
    m_Textures.emplace(path, texture);
    return texture;
}

AssetManager::FontHandle AssetManager::LoadFont(const std::string& path, float pixelHeight) {
    if (path.empty()) throw std::invalid_argument("Font path must not be empty");
    if (!std::isfinite(pixelHeight) || pixelHeight <= 0.0f)
        throw std::invalid_argument("Font pixelHeight must be finite and positive");

    FontKey key{path, pixelHeight};
    auto existing = m_Fonts.find(key);
    if (existing != m_Fonts.end()) return existing->second;

    auto font = std::make_shared<Font>(path, pixelHeight);
    if (!font->IsValid()) throw std::runtime_error("Unable to load font: " + path);
    m_Fonts.emplace(std::move(key), font);
    return font;
}

bool AssetManager::UnloadTexture(const std::string& path) {
    return m_Textures.erase(path) != 0;
}

bool AssetManager::UnloadFont(const std::string& path, float pixelHeight) {
    return m_Fonts.erase(FontKey{path, pixelHeight}) != 0;
}

void AssetManager::Clear() {
    m_Fonts.clear();
    m_Textures.clear();
}

} // namespace TigerEngine
