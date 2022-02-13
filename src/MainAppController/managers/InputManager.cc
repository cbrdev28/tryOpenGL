/**
 * Input manager for glfw to handle keyboard & mouse
 */

#include "InputManager.h"

#include <fmt/core.h>
#include <fmt/format.h>

InputManager::InputManager(WindowManager& windowManager) : _windowManager(windowManager) {
  fmt::print("InputManager::InputManager(...)\n");
  auto formattedRef = fmt::ptr(&_windowManager);
  fmt::print("InputManager::InputManager(...): _windowManager = {}\n", formattedRef);
};

InputManager* InputManager::init() {
  fmt::print("InputManager::init()\n");

  GLFWwindow* window = _windowManager.getWindow();
  GLFWcursorposfun cpCallback = glfwSetCursorPosCallback(window, InputManager::mouseCallback);
  if (cpCallback == NULL) {
    // Only warning
    auto formattedPointer = fmt::ptr(cpCallback);
    fmt::print("Warning glfwSetCursorPosCallback(...): {}\n", formattedPointer);
  }

  // NOTE: this may need to be moved in some function
  // Tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return this;
}

void InputManager::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
  fmt::print("InputManager::mouseCallback(...)\n");
  // Debug
  fmt::print("InputManager::mouseCallback(...): x = {} / y = {}\n", xPos, yPos);
}