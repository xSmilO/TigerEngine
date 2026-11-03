#pragma once
#include "Core/Math.h"
#include "Graphics/OrthographicCamera.h"
#include "Graphics/Shader.h"
#include <glm/glm.hpp>

namespace TigerEngine {
class Texture;

class Renderer2D {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const OrthographicCamera &camera);
    static void EndScene();

    // Update these signatures:
    static void DrawQuad(const glm::mat4 &transform, const Vector4 &color);

    // ADD THIS: Draws a textured quad using a matrix
    static void DrawQuad(const glm::mat4 &transform, const Texture *texture, const Vector4 &tintColor);

    static void DrawQuad(const glm::mat4 &transform, const Texture *texture,
                         const Vector4 &tintColor, const Vector2 &uvMin, const Vector2 &uvMax);

    // Matrix overloads use the same world transforms as sprites (including parents).
    static void DrawTriangle(const glm::mat4 &transform, const Vector4 &color);
    static void DrawPolygon(const glm::mat4 &transform, int sides, const Vector4 &color);

    static void DrawTriangle(const Vector2 &position, const Vector2 &size, const Vector4 &color, float rotation = 0.0f);

    static void DrawPolygon(const Vector2 &position, const Vector2 &size, int sides, const Vector4 &color,
                            float rotation = 0.0f);

    // World-space endpoints, viewed through the current BeginScene camera.
    static void DrawLine(const Vector2 &p0, const Vector2 &p1, const Vector4 &color);

  private:
    static Shader *s_Shader;

    static unsigned int s_VAO, s_VBO;
    static unsigned int s_TriangleVAO, s_TriangleVBO;
    static unsigned int s_PolygonVAO, s_PolygonVBO;
    static unsigned int s_EBO; // <-- Add this line!

    // NEW: Buffers for the Line Renderer
    static unsigned int s_LineVAO, s_LineVBO;
};
} // namespace TigerEngine