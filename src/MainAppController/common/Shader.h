#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "glmHeaders.h"
#include "openGLHeaders.h"

struct ShaderProgramSource {
  std::string vertexSource;
  std::string fragmentSource;
};

/**
 * Shader manager helper
 */
class Shader {
 public:
  explicit Shader(std::string filepath);
  ~Shader();

  Shader(const Shader& other) = delete;
  Shader(Shader&& other) = delete;
  auto operator=(const Shader& other) -> Shader& = delete;
  auto operator=(Shader&& other) -> Shader& = delete;

  void bind() const;
  void unBind() const;

  auto getUniformLocation(const std::string& name) -> GLint;
  void setUniformMat4(const std::string& name, const glm::mat4& value);
  void setUniform1i(const std::string& name, GLint value);
  void setUniform1iv(const std::string& name, const std::vector<GLint>& values);

 private:
  std::string shaderFilePath_;
  std::unordered_map<std::string, GLint> uniformLocationCache_;

  GLuint vertexShaderID_{0};
  GLuint fragmentShaderID_{0};
  GLuint shaderProgramID_{0};

  /**
   * Initialize shaders: create, compile, link
   * @return Shader&
   * @throw -1
   */
  auto init() -> Shader&;

  /**
   * Parse shader from a file
   * @return ShaderProgramSource
   * @throw -1
   */
  auto parseShader() -> ShaderProgramSource;

  /**
   * Compile vertex shader
   * @return Shader&
   * @throw -1
   */
  auto compileVertex(const std::string& source) -> Shader&;

  /**
   * Compile fragment shader
   * @return Shader&
   * @throw -1
   */
  auto compileFragment(const std::string& source) -> Shader&;

  /**
   * Compile a shader & check for errors
   * @return Shader&
   * @throw -1
   */
  auto compile(GLuint shaderID) -> Shader&;

  /**
   * Link shaders & check for errors.
   * Delete unused resources.
   * @return Shader&
   * @throw -1
   */
  auto link() -> Shader&;
};

#endif
