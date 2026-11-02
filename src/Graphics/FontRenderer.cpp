#include "Graphics/FontRenderer.h"
#include "Graphics/Shader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#define STB_TRUETYPE_IMPLEMENTATION
#include <fstream>
#include <iostream>
#include <iterator>
#include <stb/stb_truetype.h>
#include <stdexcept>

namespace TigerEngine {
unsigned int FontRenderer::s_VAO = 0;
unsigned int FontRenderer::s_VBO = 0;
Shader *FontRenderer::s_Shader = nullptr;

Font::Font(const std::string &path, float pixelHeight) : m_PixelHeight(pixelHeight) {
    if(pixelHeight <= 0.0f)
        throw std::invalid_argument("Font pixel height must be positive");
    std::ifstream stream(path, std::ios::binary);
    if(!stream)
        throw std::runtime_error("Unable to open font: " + path);
    std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(stream)), {});
    if(bytes.empty())
        throw std::runtime_error("Font file is empty: " + path);

    std::vector<unsigned char> bitmap(AtlasSize * AtlasSize);
    std::array<stbtt_bakedchar, 95> baked{};
    int result = stbtt_BakeFontBitmap(bytes.data(), 0, pixelHeight, bitmap.data(), AtlasSize, AtlasSize, 32,
                                      static_cast<int>(baked.size()), baked.data());
    if(result <= 0)
        throw std::runtime_error("Font glyph atlas did not fit: " + path);
    for(size_t i = 0; i < baked.size(); ++i) {
        const auto &b = baked[i];
        m_Glyphs[i] = {float(b.x0), float(b.y0), float(b.x1), float(b.y1), b.xoff, b.yoff, b.xadvance};
    }

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    GLint previousAlignment = 4;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousAlignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, AtlasSize, AtlasSize, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, previousAlignment);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Font::~Font() {
    if(m_Texture)
        glDeleteTextures(1, &m_Texture);
}

void FontRenderer::Init() {
    if(s_Shader)
        return;
    s_Shader = new Shader("assets/shaders/font.vert", "assets/shaders/font.frag");
    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);
    glBindVertexArray(s_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
    glBindVertexArray(0);
}

void FontRenderer::Shutdown() {
    delete s_Shader;
    s_Shader = nullptr;
    if(s_VBO)
        glDeleteBuffers(1, &s_VBO);
    if(s_VAO)
        glDeleteVertexArrays(1, &s_VAO);
    s_VBO = s_VAO = 0;
}

void FontRenderer::DrawText(const Font &font, const std::string &text, const glm::vec2 &topLeft,
                            const glm::vec4 &color) {
    if(!s_Shader || !font.IsValid())
        return;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    if(viewport[2] <= 0 || viewport[3] <= 0)
        return;
    glm::mat4 projection = glm::ortho(0.0f, float(viewport[2]), float(viewport[3]), 0.0f);

    s_Shader->Bind();
    s_Shader->SetMat4("u_Projection", projection);
    s_Shader->SetVec4("u_Color", color);
    s_Shader->SetInt("u_FontAtlas", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font.m_Texture);
    glBindVertexArray(s_VAO);

    // stb_truetype's baked coordinates use a baseline and a downward Y axis.
    float x = topLeft.x, y = topLeft.y + font.m_PixelHeight;
    for(unsigned char c : text) {
        if(c == '\n') {
            x = topLeft.x;
            y += font.m_PixelHeight * 1.25f;
            continue;
        }
        if(c < 32 || c > 126)
            c = '?'; // ASCII-only initial implementation.
        const auto &b = font.m_Glyphs[c - 32];
        float x0 = x + b.xoff, y0 = y + b.yoff;
        float x1 = x0 + b.x1 - b.x0, y1 = y0 + b.y1 - b.y0;
        float u0 = b.x0 / Font::AtlasSize, v0 = b.y0 / Font::AtlasSize;
        float u1 = b.x1 / Font::AtlasSize, v1 = b.y1 / Font::AtlasSize;
        const float vertices[] = {x0, y0, u0, v0, x1, y0, u1, v0, x1, y1, u1, v1,
                                  x0, y0, u0, v0, x1, y1, u1, v1, x0, y1, u0, v1};
        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += b.xadvance;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace TigerEngine
