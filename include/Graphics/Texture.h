#pragma once
#include <string>

namespace TigerEngine {
class Texture {
  public:
    Texture(const std::string &path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

  private:
    unsigned int m_RendererID = 0;
    int m_Width = 0, m_Height = 0, m_BPP = 0;
};
} // namespace TigerEngine