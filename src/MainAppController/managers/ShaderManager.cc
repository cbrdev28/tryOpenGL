/**
 * Shader manager helper
 */
#include "ShaderManager.h"

#include <fmt/core.h>
#include <openGLErrorHelpers.h>
#include <openGLHeaders.h>

#include <array>
#include <fstream>
#include <sstream>

constexpr int SHADER_INFO_LOG = 512;

// NOLINTNEXTLINE(modernize-pass-by-value)
ShaderManager::ShaderManager(const std::string& filepath) : shaderFilePath_(filepath){};

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
  ShaderProgramSource shaderSources = parseShader();
  this->compileVertex(shaderSources.vertexSource)
      .compileFragment(shaderSources.fragmentSource)
      .link()
      .loadMatrixUniformLocations();
  return *this;
}

void ShaderManager::bind() const { GLCall(glUseProgram(shaderProgramID_)); }

// NOLINTNEXTLINE clang-tidy(readability-convert-member-functions-to-static)
void ShaderManager::unBind() const { GLCall(glUseProgram(0)); }

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

auto ShaderManager::parseShader() -> ShaderProgramSource {
  enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1,
  };

  std::ifstream fileStream(shaderFilePath_);
  std::string line;
  std::array<std::stringstream, 2> streams;
  ShaderType type = ShaderType::NONE;
  while (std::getline(fileStream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
    } else {
      const auto streamIndex = static_cast<std::size_t>(type);
      streams.at(streamIndex) << line << "\n";
    }
  }
  return {streams[0].str(), streams[1].str()};
}

auto ShaderManager::compileVertex(const std::string& source) -> ShaderManager& {
  vertexShaderID_ = glCreateShader(GL_VERTEX_SHADER);
  if (vertexShaderID_ == 0) {
    fmt::print("Failed to glCreateShader(GL_VERTEX_SHADER)\n");
    throw -1;
  }
  const char* src = source.c_str();
  GLCall(glShaderSource(vertexShaderID_, 1, &src, nullptr));
  this->compile(vertexShaderID_);
  return *this;
}

auto ShaderManager::compileFragment(const std::string& source) -> ShaderManager& {
  fragmentShaderID_ = glCreateShader(GL_FRAGMENT_SHADER);
  if (fragmentShaderID_ == 0) {
    fmt::print("Failed to glCreateShader(GL_FRAGMENT_SHADER)\n");
    throw -1;
  }
  const char* src = source.c_str();
  GLCall(glShaderSource(fragmentShaderID_, 1, &src, nullptr));
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
