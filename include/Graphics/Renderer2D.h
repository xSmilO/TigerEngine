#pragma once
#include "Core/Math.h"
#include "Graphics/OrthographicCamera.h"
#include <glm/glm.hpp>

namespace TigerEngine {
class Texture;

class Renderer2D {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const OrthographicCamera &camera);
    static void EndScene();

    // Draw a solid colored primitive (No Texture)
    static void DrawQuad(const Vector2 &position, const Vector2 &size, const Vector4 &color);

    // Draw a textured quad with an optional color tint (Defaults to white/no tint)
    static void DrawQuad(const Vector2 &position, const Vector2 &size, const Texture *texture,
                         const Vector4 &tintColor = Vector4(1.0f));

  private:
    static unsigned int s_VAO, s_VBO, s_ShaderProgram;
};
} // namespace TigerEngine