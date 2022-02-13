/**
 * World manager to define what to draw & how to interact.
 */

// Disable clang-format because we must include glad before GLFW
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "WorldManager.h"

#include <fmt/core.h>

WorldManager& WorldManager::init() {
  fmt::print("WorldManager::init()\n");
  glEnable(GL_DEPTH_TEST);
  return *this;
}
