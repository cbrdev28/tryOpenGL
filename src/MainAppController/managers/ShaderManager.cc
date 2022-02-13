/**
 * Shader manager helper
 */

// Disable clang-format because we must include glad before GLFW
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "ShaderManager.h"

#include <fmt/core.h>

ShaderManager::ShaderManager(const char* vertexShaderSrc, const char* fragmentShaderSrc)
    : _vertexShaderSrc(vertexShaderSrc),
      _fragmentShaderSrc(fragmentShaderSrc),
      _vertexShaderID(0),
      _fragmentShaderID(0) {
  fmt::print("ShaderManager::ShaderManager(...)\n");
};

ShaderManager::~ShaderManager() {
  fmt::print("ShaderManager::~ShaderManager()\n");

  if (_vertexShaderID != 0) {
    glDeleteShader(_vertexShaderID);
  }
  if (_fragmentShaderID != 0) {
    glDeleteShader(_fragmentShaderID);
  }
}

ShaderManager& ShaderManager::init() {
  fmt::print("ShaderManager::init()\n");

  this->compileVertex();
  this->compileFragment();
  this->link();
  return *this;
}

ShaderManager& ShaderManager::compileVertex() {
  fmt::print("ShaderManager::compileVertex()\n");

  _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  if (_vertexShaderID == 0) {
    fmt::print("Failed to glCreateShader(GL_VERTEX_SHADER)\n");
    throw -1;
  }
  glShaderSource(_vertexShaderID, 1, &_vertexShaderSrc, NULL);
  this->compile(_vertexShaderID);
  return *this;
}

ShaderManager& ShaderManager::compileFragment() {
  fmt::print("ShaderManager::compileFragment()\n");

  _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  if (_fragmentShaderID == 0) {
    fmt::print("Failed to glCreateShader(GL_FRAGMENT_SHADER)\n");
    throw -1;
  }
  glShaderSource(_fragmentShaderID, 1, &_fragmentShaderSrc, NULL);
  this->compile(_fragmentShaderID);
  return *this;
}

ShaderManager& ShaderManager::compile(const unsigned int shaderID) {
  fmt::print("ShaderManager::compile()\n");

  if (shaderID == 0) {
    fmt::print("Invalid shaderID\n");
    throw -1;
  }

  glCompileShader(shaderID);

  int success;
  char infoLog[512];
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    fmt::print("Failed to compile shader:\n{}\n", infoLog);
    throw -1;
  }
  return *this;
}

ShaderManager& ShaderManager::link() {
  fmt::print("ShaderManager::link()\n");
  return *this;
}