/**
 * Shader manager helper
 */
constexpr int SHADER_INFO_LOG = 512;

#include "ShaderManager.h"

#include <fmt/core.h>

#include "Renderer.h"

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ShaderManager::ShaderManager(const char* vertexShaderSrc, const char* fragmentShaderSrc)
    : vertexShaderSrc_(vertexShaderSrc), fragmentShaderSrc_(fragmentShaderSrc){};

ShaderManager::~ShaderManager() {
  if (vertexShaderID_ != 0) {
    GLCall(glDeleteShader(vertexShaderID_));
  }
  if (fragmentShaderID_ != 0) {
    GLCall(glDeleteShader(fragmentShaderID_));
  }
  if (shaderProgramID_ != 0) {
    GLCall(glDeleteProgram(shaderProgramID_));
  }
}

auto ShaderManager::init() -> ShaderManager& {
  this->compileVertex().compileFragment().link().loadMatrixUniformLocations();
  return *this;
}

void ShaderManager::bind() const { GLCall(glUseProgram(shaderProgramID_)); }

void ShaderManager::unBind() const {
  // TODO(cbr): implement
}

// NOLINTNEXTLINE(readability-make-member-function-const)
auto ShaderManager::setModelMatrix(glm::mat4 modelMatrix) -> ShaderManager& {
  GLsizei matrixCount = 1;
  GLboolean transpose = GL_FALSE;
  GLCall(glUniformMatrix4fv(modelMatrixUniformLocation_, matrixCount, transpose, glm::value_ptr(modelMatrix)));
  return *this;
}

// NOLINTNEXTLINE(readability-make-member-function-const)
auto ShaderManager::setViewMatrix(glm::mat4 viewMatrix) -> ShaderManager& {
  GLsizei matrixCount = 1;
  GLboolean transpose = GL_FALSE;
  GLCall(glUniformMatrix4fv(viewMatrixUniformLocation_, matrixCount, transpose, glm::value_ptr(viewMatrix)));
  return *this;
}

// NOLINTNEXTLINE(readability-make-member-function-const)
auto ShaderManager::setProjectionMatrix(glm::mat4 projectionMatrix) -> ShaderManager& {
  GLsizei matrixCount = 1;
  GLboolean transpose = GL_FALSE;
  GLCall(
      glUniformMatrix4fv(projectionMatrixUniformLocation_, matrixCount, transpose, glm::value_ptr(projectionMatrix)));
  return *this;
}

auto ShaderManager::compileVertex() -> ShaderManager& {
  vertexShaderID_ = glCreateShader(GL_VERTEX_SHADER);
  if (vertexShaderID_ == 0) {
    fmt::print("Failed to glCreateShader(GL_VERTEX_SHADER)\n");
    throw -1;
  }
  GLCall(glShaderSource(vertexShaderID_, 1, &vertexShaderSrc_, nullptr));
  this->compile(vertexShaderID_);
  return *this;
}

auto ShaderManager::compileFragment() -> ShaderManager& {
  fragmentShaderID_ = glCreateShader(GL_FRAGMENT_SHADER);
  if (fragmentShaderID_ == 0) {
    fmt::print("Failed to glCreateShader(GL_FRAGMENT_SHADER)\n");
    throw -1;
  }
  GLCall(glShaderSource(fragmentShaderID_, 1, &fragmentShaderSrc_, nullptr));
  this->compile(fragmentShaderID_);
  return *this;
}

auto ShaderManager::compile(const unsigned int shaderID) -> ShaderManager& {
  if (shaderID == 0) {
    fmt::print("Invalid shaderID\n");
    throw -1;
  }
  GLCall(glCompileShader(shaderID));

  int success = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, modernize-avoid-c-arrays)
  char infoLog[SHADER_INFO_LOG];

  GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success));
  if (success == 0) {
    GLCall(glGetShaderInfoLog(shaderID, SHADER_INFO_LOG, nullptr, static_cast<GLchar*>(infoLog)));
    fmt::print("Failed to compile shader:\n{}\n", infoLog);
    throw -1;
  }
  return *this;
}

auto ShaderManager::link() -> ShaderManager& {
  shaderProgramID_ = glCreateProgram();
  if (shaderProgramID_ == 0) {
    fmt::print("Failed to glCreateProgram()\n");
    throw -1;
  }
  GLCall(glAttachShader(shaderProgramID_, vertexShaderID_));
  GLCall(glAttachShader(shaderProgramID_, fragmentShaderID_));
  GLCall(glLinkProgram(shaderProgramID_));

  int success = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, modernize-avoid-c-arrays)
  char infoLog[SHADER_INFO_LOG];

  GLCall(glGetProgramiv(shaderProgramID_, GL_LINK_STATUS, &success));
  if (success == 0) {
    GLCall(glGetProgramInfoLog(shaderProgramID_, SHADER_INFO_LOG, nullptr, static_cast<GLchar*>(infoLog)));
    fmt::print("Failed to link shaders:\n{}\n", infoLog);
    throw -1;
  }
  GLCall(glDeleteShader(vertexShaderID_));
  GLCall(glDeleteShader(fragmentShaderID_));
  return *this;
}

auto ShaderManager::loadMatrixUniformLocations() -> ShaderManager& {
  modelMatrixUniformLocation_ = glGetUniformLocation(shaderProgramID_, "model");
  if (modelMatrixUniformLocation_ == GL_INVALID_VALUE || modelMatrixUniformLocation_ == GL_INVALID_OPERATION) {
    fmt::print("Failed to get uniform location for model matrix\n");
    throw -1;
  }

  viewMatrixUniformLocation_ = glGetUniformLocation(shaderProgramID_, "view");
  if (viewMatrixUniformLocation_ == GL_INVALID_VALUE || viewMatrixUniformLocation_ == GL_INVALID_OPERATION) {
    fmt::print("Failed to get uniform location for view matrix\n");
    throw -1;
  }

  projectionMatrixUniformLocation_ = glGetUniformLocation(shaderProgramID_, "projection");
  if (projectionMatrixUniformLocation_ == GL_INVALID_VALUE ||
      projectionMatrixUniformLocation_ == GL_INVALID_OPERATION) {
    fmt::print("Failed to get uniform location for projection matrix\n");
    throw -1;
  }
  return *this;
}
