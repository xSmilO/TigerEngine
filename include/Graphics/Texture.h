#pragma once
#include <string>

namespace TigerEngine {
class Texture {
  public:
    Texture(const std::string &path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

  private:
    unsigned int m_RendererID;
    int m_Width, m_Height, m_BPP;
};
} // namespace TigerEngine