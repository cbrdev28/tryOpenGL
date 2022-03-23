#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

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
class ShaderManager {
 public:
  explicit ShaderManager(std::string filepath);
  ~ShaderManager();

  ShaderManager(const ShaderManager& other) = delete;
  ShaderManager(ShaderManager&& other) = delete;
  auto operator=(const ShaderManager& other) -> ShaderManager& = delete;
  auto operator=(ShaderManager&& other) -> ShaderManager& = delete;

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
   * @return ShaderManager&
   * @throw -1
   */
  auto init() -> ShaderManager&;

  /**
   * Parse shader from a file
   * @return ShaderProgramSource
   * @throw -1
   */
  auto parseShader() -> ShaderProgramSource;

  /**
   * Compile vertex shader
   * @return ShaderManager&
   * @throw -1
   */
  auto compileVertex(const std::string& source) -> ShaderManager&;

  /**
   * Compile fragment shader
   * @return ShaderManager&
   * @throw -1
   */
  auto compileFragment(const std::string& source) -> ShaderManager&;

  /**
   * Compile a shader & check for errors
   * @return ShaderManager&
   * @throw -1
   */
  auto compile(GLuint shaderID) -> ShaderManager&;

  /**
   * Link shaders & check for errors.
   * Delete unused resources.
   * @return ShaderManager&
   * @throw -1
   */
  auto link() -> ShaderManager&;
};

#endif
