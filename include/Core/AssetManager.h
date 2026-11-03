#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <utility>
#include <cstddef>

namespace TigerEngine {
class Texture;
class Font;

class AssetManager {
public:
    using TextureHandle = std::shared_ptr<Texture>;
    using FontHandle = std::shared_ptr<Font>;

    TextureHandle LoadTexture(const std::string& path);
    FontHandle LoadFont(const std::string& path, float pixelHeight = 32.0f);

    bool UnloadTexture(const std::string& path);
    bool UnloadFont(const std::string& path, float pixelHeight = 32.0f);
    void Clear();

    std::size_t TextureCount() const { return m_Textures.size(); }
    std::size_t FontCount() const { return m_Fonts.size(); }

private:
    using FontKey = std::pair<std::string, float>;
    std::unordered_map<std::string, TextureHandle> m_Textures;
    std::map<FontKey, FontHandle> m_Fonts;
};
}
