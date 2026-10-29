#include "Graphics/Renderer2D.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace TigerEngine {

unsigned int Renderer2D::s_VAO = 0, Renderer2D::s_VBO = 0, Renderer2D::s_ShaderProgram = 0;

unsigned int Renderer2D::s_TriangleVAO = 0, Renderer2D::s_TriangleVBO = 0;
unsigned int Renderer2D::s_PolygonVAO = 0, Renderer2D::s_PolygonVBO = 0;
unsigned int Renderer2D::s_LineVAO = 0, Renderer2D::s_LineVBO = 0;
unsigned int Renderer2D::s_EBO = 0;

Shader *Renderer2D::s_Shader = nullptr;

void Renderer2D::Init() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 1. Setup Quad VAO/VBO/EBO
    glGenVertexArrays(1, &s_VAO);
    glBindVertexArray(s_VAO);

    float vertices[] = {
        // Positions   // TexCoords
        -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
        0.5f,  -0.5f, 1.0f, 0.0f, // Bottom Right
        0.5f,  0.5f,  1.0f, 1.0f, // Top Right
        -0.5f, 0.5f,  0.0f, 1.0f  // Top Left
    };

    glGenBuffers(1, &s_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    glGenBuffers(1, &s_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Set default shader
    s_Shader = new Shader("assets/shaders/default.vert", "assets/shaders/default.frag");

    float triangleVertices[] = {
        // X, Y,       U, V
        0.0f,  0.5f,  0.5f, 1.0f, // Top center
        -0.5f, -0.5f, 0.0f, 0.0f, // Bottom left
        0.5f,  -0.5f, 1.0f, 0.0f  // Bottom right
    };
    glGenVertexArrays(1, &s_TriangleVAO);
    glGenBuffers(1, &s_TriangleVBO);
    glBindVertexArray(s_TriangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_TriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // --- 2. SETUP DYNAMIC POLYGON ---
    glGenVertexArrays(1, &s_PolygonVAO);
    glGenBuffers(1, &s_PolygonVBO);
    glBindVertexArray(s_PolygonVAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_PolygonVBO);
    // We don't fill this with data yet, we just allocate enough empty memory (GL_DYNAMIC_DRAW)
    // 4 floats per vertex, max 100 vertices = 400 floats
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 400, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &s_LineVAO);
    glGenBuffers(1, &s_LineVBO);
    glBindVertexArray(s_LineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_LineVBO);
    // Allocate space for 2 vertices (X, Y, U, V) = 8 floats total. GL_DYNAMIC_DRAW because lines change every frame.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Renderer2D::Shutdown() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
    glDeleteBuffers(1, &s_EBO);
    delete s_Shader;
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Vector4 &color) {
    s_Shader->Bind();

    s_Shader->SetMat4("u_Transform", transform);
    s_Shader->SetFloat4("u_Color", color.r, color.g, color.b, color.a);
    s_Shader->SetInt("u_HasTexture", 0);

    glBindVertexArray(s_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Texture *texture, const Vector4 &tintColor) {
    s_Shader->Bind();

    s_Shader->SetMat4("u_Transform", transform);

    s_Shader->SetFloat4("u_Color", tintColor.r, tintColor.g, tintColor.b, tintColor.a);

    if(texture) {
        texture->Bind(0);
        s_Shader->SetInt("u_Texture", 0);
        s_Shader->SetInt("u_HasTexture", 1);
    } else {
        s_Shader->SetInt("u_HasTexture", 0);
    }

    glBindVertexArray(s_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer2D::DrawTriangle(const Vector2 &position, const Vector2 &size, const Vector4 &tintColor, float rotation) {
    s_Shader->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

    s_Shader->SetMat4("u_Transform", transform);
    s_Shader->SetFloat4("u_Color", tintColor.r, tintColor.g, tintColor.b, tintColor.a);
    s_Shader->SetInt("u_HasTexture", 0);

    glBindVertexArray(s_TriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer2D::DrawPolygon(const Vector2 &position, const Vector2 &size, int sides, const Vector4 &color,
                             const float rotation) {
    if(sides < 3)
        sides = 3;
    if(sides > 98)
        sides = 98;

    std::vector<float> vertices;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f); // X, Y
    vertices.push_back(0.5f);
    vertices.push_back(0.5f); // U, V

    float angleStep = (2.0f * 3.14159265f) / sides;
    for(int i = 0; i <= sides; i++) {
        float angle = i * angleStep;
        vertices.push_back(cos(angle) * 0.5f);        // X
        vertices.push_back(sin(angle) * 0.5f);        // Y
        vertices.push_back(cos(angle) * 0.5f + 0.5f); // U
        vertices.push_back(sin(angle) * 0.5f + 0.5f); // V
    }

    glBindBuffer(GL_ARRAY_BUFFER, s_PolygonVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    glUseProgram(s_ShaderProgram);
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(s_ShaderProgram, "u_Transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), color.r, color.g, color.b, color.a);
    glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0);

    glBindVertexArray(s_PolygonVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);
}

void Renderer2D::DrawLine(const Vector2 &p0, const Vector2 &p1, const Vector4 &color) {
    float lineVertices[8] = {p0.x, p0.y, 0.0f, 0.0f, p1.x, p1.y, 1.0f, 1.0f};

    glBindBuffer(GL_ARRAY_BUFFER, s_LineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVertices), lineVertices);

    glUseProgram(s_ShaderProgram);
    glm::mat4 identity = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(s_ShaderProgram, "u_Transform"), 1, GL_FALSE, glm::value_ptr(identity));
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), color.r, color.g, color.b, color.a);
    glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0);

    glBindVertexArray(s_LineVAO);
    glDrawArrays(GL_LINES, 0, 2);
}

void Renderer2D::BeginScene(const OrthographicCamera &camera) {
    s_Shader->Bind();
    s_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer2D::EndScene() {
    // Ready for batch rendering later
}

} // namespace TigerEngine