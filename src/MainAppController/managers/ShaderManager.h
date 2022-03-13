#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

#include <glmHeaders.h>
#include <openGLHeaders.h>

#include <string>
#include <unordered_map>
#include <vector>

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

  /**
   * Initialize shaders: create, compile, link
   * @return ShaderManager&
   * @throw -1
   */
  auto init() -> ShaderManager&;

  void bind() const;
  void unBind() const;

  auto getUniformLocation(const std::string& name) -> GLint;
  void setUniformMat4(const std::string& name, const glm::mat4& value);
  void setUniform1i(const std::string& name, const int value);
  void setUniform1iv(const std::string& name, const std::vector<int>& values);

 private:
  std::string shaderFilePath_;
  std::unordered_map<std::string, GLint> uniformLocationCache_;

  unsigned int vertexShaderID_{0};
  unsigned int fragmentShaderID_{0};
  unsigned int shaderProgramID_{0};

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
  auto compile(unsigned int shaderID) -> ShaderManager&;

  /**
   * Link shaders & check for errors.
   * Delete unused resources.
   * @return ShaderManager&
   * @throw -1
   */
  auto link() -> ShaderManager&;
};

#endif
