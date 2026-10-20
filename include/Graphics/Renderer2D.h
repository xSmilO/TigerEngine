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

    static void DrawQuad(const Vector2 &position, const Vector2 &size, const Vector4 &color);
    static void DrawQuad(const Vector2 &position, const Vector2 &size, const Texture *texture,
                         const Vector4 &tintColor = Vector4(1.0f));

    // NEW: Draw a Triangle
    static void DrawTriangle(const Vector2 &position, const Vector2 &size, const Vector4 &color);

    // NEW: Draw a Polygon (sides = 3 for triangle, 5 for pentagon, 30 for circle)
    static void DrawPolygon(const Vector2 &position, const Vector2 &size, int sides, const Vector4 &color);

    // NEW: Draw a line between two points
    static void DrawLine(const Vector2 &p0, const Vector2 &p1, const Vector4 &color);

  private:
    static unsigned int s_VAO, s_VBO, s_ShaderProgram;
    static unsigned int s_TriangleVAO, s_TriangleVBO;
    static unsigned int s_PolygonVAO, s_PolygonVBO;

    // NEW: Buffers for the Line Renderer
    static unsigned int s_LineVAO, s_LineVBO;
};
} // namespace TigerEngine