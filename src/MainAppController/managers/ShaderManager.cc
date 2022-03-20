/**
 * Shader manager helper
 */
#include "ShaderManager.h"

#include <fmt/core.h>

#include <array>
#include <fstream>
#include <sstream>
#include <utility>

#include "openGLErrorHelpers.h"

constexpr int SHADER_INFO_LOG = 512;

ShaderManager::ShaderManager(std::string filepath) : shaderFilePath_(std::move(filepath)) { init(); };

ShaderManager::~ShaderManager() {
  unBind();
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

void ShaderManager::bind() const { GLCall(glUseProgram(shaderProgramID_)); }

void ShaderManager::unBind() const { GLCall(glUseProgram(0)); }

auto ShaderManager::getUniformLocation(const std::string& name) -> GLint {
  if (uniformLocationCache_.find(name) != uniformLocationCache_.end()) {
    return uniformLocationCache_[name];
  }

  GLCall(GLint uniformLocation = glGetUniformLocation(shaderProgramID_, name.c_str()));
  if (uniformLocation == -1) {
    fmt::print("ShaderManager error with file: {}\n", shaderFilePath_);
    fmt::print("Failed to get uniform location for: {}\n", name);
    throw -1;
  }
  uniformLocationCache_[name] = uniformLocation;
  return uniformLocation;
}

void ShaderManager::setUniformMat4(const std::string& name, const glm::mat4& value) {
  GLsizei count = 1;
  GLboolean transpose = GL_FALSE;
  GLCall(glUniformMatrix4fv(this->getUniformLocation(name), count, transpose, glm::value_ptr(value)));
}

void ShaderManager::setUniform1i(const std::string& name, const GLint value) {
  GLCall(glUniform1i(this->getUniformLocation(name), value));
}

void ShaderManager::setUniform1iv(const std::string& name, const std::vector<GLint>& values) {
  GLCall(glUniform1iv(this->getUniformLocation(name), values.size(), values.data()));
}

auto ShaderManager::init() -> ShaderManager& {
  ShaderProgramSource shaderSources = parseShader();
  this->compileVertex(shaderSources.vertexSource).compileFragment(shaderSources.fragmentSource).link();
  bind();
  return *this;
}

auto ShaderManager::parseShader() -> ShaderProgramSource {
  enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1,
  };

  std::ifstream fileStream(shaderFilePath_);
  if (!fileStream.is_open()) {
    fmt::print("Failed to open shader file: {}\n", shaderFilePath_);
    throw -1;
  }

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
    fmt::print("ShaderManager error with file: {}\n", shaderFilePath_);
    fmt::print("Failed to glCreateShader(GL_VERTEX_SHADER): {}\n", source);
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
    fmt::print("ShaderManager error with file: {}\n", shaderFilePath_);
    fmt::print("Failed to glCreateShader(GL_FRAGMENT_SHADER): {}\n", source);
    throw -1;
  }
  const char* src = source.c_str();
  GLCall(glShaderSource(fragmentShaderID_, 1, &src, nullptr));
  this->compile(fragmentShaderID_);
  return *this;
}

auto ShaderManager::compile(const GLuint shaderID) -> ShaderManager& {
  if (shaderID == 0) {
    fmt::print("ShaderManager error with file: {}\n", shaderFilePath_);
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
    fmt::print("ShaderManager error with file: {}\n", shaderFilePath_);
    fmt::print("Failed to compile shader:\n{}\n", infoLog);
    throw -1;
  }
  return *this;
}

auto ShaderManager::link() -> ShaderManager& {
  shaderProgramID_ = glCreateProgram();
  if (shaderProgramID_ == 0) {
    fmt::print("ShaderManager error with file: {}\n", shaderFilePath_);
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
    fmt::print("ShaderManager error with file: {}\n", shaderFilePath_);
    fmt::print("Failed to link shaders:\n{}\n", infoLog);
    throw -1;
  }
  GLCall(glDeleteShader(vertexShaderID_));
  GLCall(glDeleteShader(fragmentShaderID_));
  return *this;
}
