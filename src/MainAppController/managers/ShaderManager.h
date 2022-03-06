#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

#include <glmHeaders.h>
#include <openGLHeaders.h>

/**
 * Shader manager helper
 */
class ShaderManager {
 public:
  ShaderManager(const char* vertexShaderSrc, const char* fragmentShaderSrc);
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
  const char* vertexShaderSrc_{vertexShaderSource};
  const char* fragmentShaderSrc_{fragmentShaderSource};

  unsigned int vertexShaderID_{0};
  unsigned int fragmentShaderID_{0};
  unsigned int shaderProgramID_{0};

  GLint modelMatrixUniformLocation_{-1};
  GLint viewMatrixUniformLocation_{-1};
  GLint projectionMatrixUniformLocation_{-1};

  /**
   * Compile vertex shader
   * @return ShaderManager&
   * @throw -1
   */
  auto compileVertex() -> ShaderManager&;

  /**
   * Compile fragment shader
   * @return ShaderManager&
   * @throw -1
   */
  auto compileFragment() -> ShaderManager&;

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

  // For now we hardcode our shader sources & make them available
 public:
  /**
   * Vertex shader source
   */
  static constexpr const char* vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform mat4 model;\n"
      "uniform mat4 view;\n"
      "uniform mat4 projection;\n"
      "out vec3 posVec;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
      "   posVec = aPos;\n"
      "}\0";

  /**
   * Fragment shader source
   */
  static constexpr const char* fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "in vec3 posVec;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) + vec4(posVec, 1.0f);\n"
      "}\n\0";
};

#endif
