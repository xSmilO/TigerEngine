#include "Graphics/Shader.h"
#include "Core/AssetPath.h"
#include <fstream>
#include <glad/glad.h> // Or wherever your OpenGL headers are
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace TigerEngine {

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    // 1. Read the files
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex = 0, fragment = 0;
    m_RendererID = 0;
    try {
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX: " + vertexPath);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT: " + fragmentPath);
        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertex);
        glAttachShader(m_RendererID, fragment);
        glLinkProgram(m_RendererID);
        CheckCompileErrors(m_RendererID, "PROGRAM");
    } catch (...) {
        if (vertex) glDeleteShader(vertex);
        if (fragment) glDeleteShader(fragment);
        if (m_RendererID) glDeleteProgram(m_RendererID);
        throw;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}
void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetFloat4(const std::string &name, float f1, float f2, float f3, float f4) {
    glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), f1, f2, f3, f4);
}

std::string Shader::ReadFile(const std::string &filepath) {
    const auto resolved = AssetPath::Resolve(filepath).string();
    std::ifstream file;
    std::stringstream stream;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(resolved);
        stream << file.rdbuf();
        file.close();
    } catch(std::ifstream::failure &e) {
        throw std::runtime_error("Failed to read shader file: " + resolved);
    }
    return stream.str();
}

void Shader::CheckCompileErrors(unsigned int shader, const std::string &type) {
    int success = GL_FALSE;
    char infoLog[1024]{};
    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
    }
    if (!success) throw std::runtime_error("Shader " + type + " failed: " + infoLog);
}

} // namespace TigerEngine
