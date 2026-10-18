#include "Graphics/Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <stb_image/stb_image.h>

namespace TigerEngine {
Texture::Texture(const std::string &path) : m_RendererID(0) {
    stbi_set_flip_vertically_on_load(1);
    unsigned char *localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    if(!localBuffer)
        return;

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(localBuffer);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_RendererID);
}
void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace TigerEngine