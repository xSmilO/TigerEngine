#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace TigerEngine {

class Shader {
  public:
    // Takes the file paths to your vertex and fragment shaders
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Easy uniform setters
    void SetInt(const std::string &name, int value);
    void SetFloat(const std::string &name, float value);
    void SetVec4(const std::string &name, const glm::vec4 &value);
    void SetMat4(const std::string &name, const glm::mat4 &matrix);
    void SetFloat4(const std::string &name, float f1, float f2, float f3, float f4);

  private:
    unsigned int m_RendererID; // The OpenGL Program ID

    std::string ReadFile(const std::string &filepath);
    void CheckCompileErrors(unsigned int shader, const std::string &type);
};

} // namespace TigerEngine