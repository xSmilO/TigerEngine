#include "Graphics/Renderer2D.h"
#include "Graphics/Texture.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace TigerEngine {

unsigned int Renderer2D::s_VAO = 0, Renderer2D::s_VBO = 0, Renderer2D::s_ShaderProgram = 0;

unsigned int Renderer2D::s_TriangleVAO = 0, Renderer2D::s_TriangleVBO = 0;
unsigned int Renderer2D::s_PolygonVAO = 0, Renderer2D::s_PolygonVBO = 0;
unsigned int Renderer2D::s_LineVAO = 0, Renderer2D::s_LineVBO = 0;
unsigned int Renderer2D::s_EBO = 0;

const char *vertexShaderSrc = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 u_Transform; 

void main() {
    gl_Position = u_Transform * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
})";

const char *fragmentShaderSrc = R"(#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_HasTexture; 

void main() {
    if (u_HasTexture == 1) {
        FragColor = texture(u_Texture, TexCoord) * u_Color;
    } else {
        FragColor = u_Color;
    }
})";

void Renderer2D::Init() {
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

    // 2. Compile and Link Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    s_ShaderProgram = glCreateProgram();
    glAttachShader(s_ShaderProgram, vertexShader);
    glAttachShader(s_ShaderProgram, fragmentShader);
    glLinkProgram(s_ShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // NOTE: You will eventually want to initialize s_TriangleVAO, s_PolygonVAO,
    // and s_LineVAO here as well so those drawing functions don't crash!
}

void Renderer2D::Shutdown() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
    glDeleteBuffers(1, &s_EBO);
    glDeleteProgram(s_ShaderProgram);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Vector4 &color) {
    glUseProgram(s_ShaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(s_ShaderProgram, "u_Transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), color.r, color.g, color.b, color.a);
    glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0);

    glBindVertexArray(s_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Texture *texture, const Vector4 &tintColor) {
    glUseProgram(s_ShaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(s_ShaderProgram, "u_Transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), tintColor.r, tintColor.g, tintColor.b, tintColor.a);

    if(texture) {
        texture->Bind(0);
        glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_Texture"), 0);
        glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 1);
    } else {
        glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0);
    }

    glBindVertexArray(s_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer2D::DrawTriangle(const Vector2 &position, const Vector2 &size, const Vector4 &tintColor, float rotation) {
    glUseProgram(s_ShaderProgram);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(s_ShaderProgram, "u_Transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), tintColor.r, tintColor.g, tintColor.b, tintColor.a);
    glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0);

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
    glUseProgram(s_ShaderProgram);
    int loc = glGetUniformLocation(s_ShaderProgram, "u_ViewProjection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));
}

void Renderer2D::EndScene() {
    // Ready for batch rendering later
}

} // namespace TigerEngine