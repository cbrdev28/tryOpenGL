/**
 * Shader manager helper
 */

#include "ShaderManager.h"

#include <fmt/core.h>

ShaderManager::ShaderManager(const char* vertexShaderSrc, const char* fragmentShaderSrc)
    : _vertexShaderSrc(vertexShaderSrc), _fragmentShaderSrc(fragmentShaderSrc) {
  fmt::print("ShaderManager::ShaderManager(...)\n");
};

ShaderManager::~ShaderManager() {
  fmt::print("ShaderManager::~ShaderManager()\n");
  // Handle deleting shader resources
}

ShaderManager& ShaderManager::init() {
  fmt::print("ShaderManager::init()\n");
  // Compile & link shaders...
  this->compileVertex();
  this->compileFragment();
  this->link();
  return *this;
}

ShaderManager& ShaderManager::compileVertex() {
  fmt::print("ShaderManager::compileVertex()\n");
  this->compile();
  return *this;
}

ShaderManager& ShaderManager::compileFragment() {
  fmt::print("ShaderManager::compileFragment()\n");
  this->compile();
  return *this;
}

ShaderManager& ShaderManager::compile() {
  fmt::print("ShaderManager::compile()\n");
  return *this;
}

ShaderManager& ShaderManager::link() {
  fmt::print("ShaderManager::link()\n");
  return *this;
}