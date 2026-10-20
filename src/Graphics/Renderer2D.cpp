#include "Graphics/Renderer2D.h"
#include "Graphics/Texture.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace TigerEngine {
unsigned int Renderer2D::s_VAO = 0, Renderer2D::s_VBO = 0, Renderer2D::s_ShaderProgram = 0;

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
}

void Renderer2D::Shutdown() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
    glDeleteProgram(s_ShaderProgram);
}

void Renderer2D::DrawQuad(float x, float y, float width, float height, const Texture *texture, float r, float g,
                          float b, float a) {
    glUseProgram(s_ShaderProgram);
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Position"), x, y);
    glUniform2f(glGetUniformLocation(s_ShaderProgram, "u_Scale"), width, height);
    glUniform4f(glGetUniformLocation(s_ShaderProgram, "u_Color"), r, g, b, a);

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

#include <glm/gtc/type_ptr.hpp> // Add this include at the top

// ... Init() and Shutdown() stay exactly the same ...

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