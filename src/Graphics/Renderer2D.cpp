#include "Graphics/Renderer2D.h"
#include "Graphics/Texture.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace TigerEngine {
unsigned int Renderer2D::s_VAO = 0, Renderer2D::s_VBO = 0, Renderer2D::s_ShaderProgram = 0;

unsigned int Renderer2D::s_TriangleVAO = 0, Renderer2D::s_TriangleVBO = 0;
unsigned int Renderer2D::s_PolygonVAO = 0, Renderer2D::s_PolygonVBO = 0;
unsigned int Renderer2D::s_LineVAO = 0, Renderer2D::s_LineVBO = 0;

const char *vertexShaderSrc = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;
    out vec2 TexCoord;
    
    uniform mat4 u_ViewProjection; // NEW
    uniform vec2 u_Position;
    uniform vec2 u_Scale;
    
    void main() {
        // Multiply the world position by the camera matrix
        vec4 worldPos = vec4((aPos * u_Scale) + u_Position, 0.0, 1.0);
        gl_Position = u_ViewProjection * worldPos;
        TexCoord = aTexCoord;
    })";

const char *fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoord;
    uniform sampler2D u_Texture;
    uniform vec4 u_Color;
    uniform int u_HasTexture;
    void main() {
        if(u_HasTexture == 1) FragColor = texture(u_Texture, TexCoord) * u_Color;
        else FragColor = u_Color;
    })";

void Renderer2D::Init() {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fs);

    s_ShaderProgram = glCreateProgram();
    glAttachShader(s_ShaderProgram, vs);
    glAttachShader(s_ShaderProgram, fs);
    glLinkProgram(s_ShaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    float quadVertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  1.0f, 1.0f,
                            0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.0f, 0.0f};

    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);
    glBindVertexArray(s_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    glDeleteProgram(s_ShaderProgram);
}

// Draw primitive with pure color
void Renderer2D::DrawQuad(const Vector2 &position, const Vector2 &size, const Vector4 &color) {
    DrawQuad(position, size, nullptr, color);
}

// Draw texture with color tint
void Renderer2D::DrawQuad(const Vector2 &position, const Vector2 &size, const Texture *texture,
                          const Vector4 &tintColor) {
    glUseProgram(s_ShaderProgram);

    // Send Vectors to the shader
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Position"), position.x, position.y);
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Scale"), size.x, size.y);
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), tintColor.r, tintColor.g, tintColor.b, tintColor.a);

    if(texture) {
        texture->Bind(0);
        glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_Texture"), 0);
        glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 1);
    } else {
        glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0);
    }

    glBindVertexArray(s_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer2D::DrawTriangle(const Vector2 &position, const Vector2 &size, const Vector4 &color) {
    glUseProgram(s_ShaderProgram);

    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Position"), position.x, position.y);
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Scale"), size.x, size.y);
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), color.r, color.g, color.b, color.a);
    glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0); // No texture

    glBindVertexArray(s_TriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer2D::DrawPolygon(const Vector2 &position, const Vector2 &size, int sides, const Vector4 &color) {
    if(sides < 3)
        sides = 3;
    if(sides > 98)
        sides = 98; // Prevent overflowing our buffer

    std::vector<float> vertices;
    // Central vertex for the triangle fan
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

    // Upload the new vertices to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, s_PolygonVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    // Draw
    glUseProgram(s_ShaderProgram);
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Position"), position.x, position.y);
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Scale"), size.x, size.y);
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), color.r, color.g, color.b, color.a);
    glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0);

    glBindVertexArray(s_PolygonVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);
}

void Renderer2D::DrawLine(const Vector2 &p0, const Vector2 &p1, const Vector4 &color) {
    // Construct the array of 2 vertices. (U, V coords don't matter for pure colored lines)
    float lineVertices[8] = {p0.x, p0.y, 0.0f, 0.0f, p1.x, p1.y, 1.0f, 1.0f};

    // Upload the new points to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, s_LineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVertices), lineVertices);

    glUseProgram(s_ShaderProgram);

    // Because our vertices are already exact world coordinates, we tell the shader not to move or scale them!
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Position"), 0.0f, 0.0f);
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Scale"), 1.0f, 1.0f);

    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), color.r, color.g, color.b, color.a);
    glUniform1i(glGetUniformLocation(s_ShaderProgram, "u_HasTexture"), 0);

    glBindVertexArray(s_LineVAO);

    // Draw the line!
    glDrawArrays(GL_LINES, 0, 2);
}

void Renderer2D::BeginScene(const OrthographicCamera &camera) {
    glUseProgram(s_ShaderProgram);
    // Send the camera matrix to the GPU once per frame
    int loc = glGetUniformLocation(s_ShaderProgram, "u_ViewProjection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));
}

void Renderer2D::EndScene() {
    // Nothing needed here right now, but good for future batch rendering
}
} // namespace TigerEngine