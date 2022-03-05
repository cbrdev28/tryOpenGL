/**
 * Input manager for glfw to handle keyboard & mouse
 */
#include "InputManager.h"

InputManager::InputManager(WindowManager& windowManager) : windowManager_(windowManager) {
  fmt::print("InputManager::InputManager(...)\n");
  const auto* formattedRef = fmt::ptr(&windowManager_);
  fmt::print("InputManager::InputManager(...): windowManager_ = {}\n", formattedRef);
};

auto InputManager::init() -> InputManager& {
  fmt::print("InputManager::init()\n");

  GLFWwindow* window = windowManager_.getWindow();
  GLFWcursorposfun cpCallback = glfwSetCursorPosCallback(window, InputManager::mouseCallback);
  if (cpCallback == nullptr) {
    // Only warning
    const auto* formattedPointer = fmt::ptr(cpCallback);
    fmt::print("Warning glfwSetCursorPosCallback(...): {}\n", formattedPointer);
  }

  // NOTE: this may need to be moved in some function
  // Uncomment to tell GLFW to capture our mouse
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return *this;
}

void InputManager::processKeyboardInput() {
  GLFWwindow* window = windowManager_.getWindow();
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1 /* true */);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    for (KeyboardListener* listener : listeners_) {
      listener->onMoveForward();
    }
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    for (KeyboardListener* listener : listeners_) {
      listener->onMoveBackward();
    }
  }
}

void InputManager::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
  // Debug
  // fmt::print("InputManager::mouseCallback(...): x = {} / y = {}\n", xPos, yPos);
}