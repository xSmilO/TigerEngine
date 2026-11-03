#pragma once
#include <glm/glm.hpp>
#include <array>
#include <string>
#include <vector>

namespace TigerEngine {

class Font {
public:
    Font(const std::string& path, float pixelHeight = 32.0f);
    ~Font();
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(Font&&) = delete;

    bool IsValid() const { return m_Texture != 0; }
    float GetPixelHeight() const { return m_PixelHeight; }
    glm::vec2 MeasureText(const std::string& text) const;

    struct TextBounds {
        glm::vec2 Min{0.0f};
        glm::vec2 Max{0.0f};
        glm::vec2 Size() const { return Max - Min; }
    };
    TextBounds MeasureVisibleBounds(const std::string& text) const;

private:
    friend class FontRenderer;
    struct Glyph { float x0, y0, x1, y1, xoff, yoff, xadvance; };

    std::array<Glyph, 95> m_Glyphs{};
    unsigned int m_Texture = 0;
    float m_PixelHeight = 0.0f;
    static constexpr int AtlasSize = 1024;
};

class FontRenderer {
public:
    static void Init();
    static void Shutdown();

    static void DrawText(const Font& font, const std::string& text,
                         const glm::vec2& topLeft, const glm::vec4& color = glm::vec4(1.0f));
private:
    static unsigned int s_VAO, s_VBO;
    static class Shader* s_Shader;
};
}
