#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

#include <glmHeaders.h>
#include <openGLHeaders.h>

#include <string>

struct ShaderProgramSource {
  std::string vertexSource;
  std::string fragmentSource;
};

/**
 * Shader manager helper
 */
class ShaderManager {
 public:
  explicit ShaderManager(const std::string& filepath);
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

  /**
   * Helper to directly set the model matrix in the vertex shader
   * @param modelMatrix the model matrix
   * @return ShaderManager&
   * @throw -1
   */
  auto setModelMatrix(glm::mat4 modelMatrix) -> ShaderManager&;

  /**
   * Helper to directly set the view matrix in the vertex shader
   * @param viewMatrix the view matrix
   * @return ShaderManager&
   * @throw -1
   */
  auto setViewMatrix(glm::mat4 viewMatrix) -> ShaderManager&;

  /**
   * Helper to directly set the projection matrix in the vertex shader
   * @param projectionMatrix the projection matrix
   * @return ShaderManager&
   * @throw -1
   */
  auto setProjectionMatrix(glm::mat4 projectionMatrix) -> ShaderManager&;

 private:
  std::string shaderFilePath_;

  unsigned int vertexShaderID_{0};
  unsigned int fragmentShaderID_{0};
  unsigned int shaderProgramID_{0};

  GLint modelMatrixUniformLocation_{-1};
  GLint viewMatrixUniformLocation_{-1};
  GLint projectionMatrixUniformLocation_{-1};

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

  /**
   * Load matrix uniform locations from shader
   * @return ShaderManager&
   * @throw -1
   */
  auto loadMatrixUniformLocations() -> ShaderManager&;
};

#endif
