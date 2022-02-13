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

WorldManager& WorldManager::render() {
  // fmt::print("WorldManager::render()\n");
  const float neonPinkR = 255 / 255.0f;
  const float neonPinkG = 68 / 255.0f;
  const float neonPinkB = 205 / 255.0f;
  glClearColor(neonPinkR, neonPinkG, neonPinkB, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return *this;
}