#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

/**
 * Shader manager helper
 */
class ShaderManager {
 public:
  ShaderManager(const char* vertexShaderSrc, const char* fragmentShaderSrc);
  ~ShaderManager();

  /**
   * Initialize shaders: create, compile, link
   * @return ShaderManager&
   * @throw -1
   */
  ShaderManager& init();

 private:
  const char* _vertexShaderSrc;
  const char* _fragmentShaderSrc;

  unsigned int _vertexShaderID;
  unsigned int _fragmentShaderID;
  unsigned int _shaderProgramID;

  /**
   * Compile vertex shader
   * @return ShaderManager&
   * @throw -1
   */
  ShaderManager& compileVertex();

  /**
   * Compile fragment shader
   * @return ShaderManager&
   * @throw -1
   */
  ShaderManager& compileFragment();

  /**
   * Compile a shader & check for errors
   * @return ShaderManager&
   * @throw -1
   */
  ShaderManager& compile(const unsigned int shaderID);

  /**
   * Link shaders & check for errors.
   * Delete unused resources.
   * @return ShaderManager&
   * @throw -1
   */
  ShaderManager& link();

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
      "   FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) * vec4(posVec, 1.0f);\n"
      "}\n\0";
};

#endif
