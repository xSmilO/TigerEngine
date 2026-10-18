#pragma once

namespace TigerEngine {
class Texture;

class Renderer2D {
  public:
    static void Init();
    static void Shutdown();
    static void DrawQuad(float x, float y, float width, float height, const Texture *texture, float r = 1.0f,
                         float g = 1.0f, float b = 1.0f, float a = 1.0f);

  private:
    static unsigned int s_VAO, s_VBO, s_ShaderProgram;
};
} // namespace TigerEngine