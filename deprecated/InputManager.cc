/**
 * Input manager for glfw to handle keyboard & mouse
 */
#include "InputManager.h"

#include <fmt/core.h>
#include <fmt/format.h>

InputManager::InputManager(WindowManager& windowManager) : windowManager_(windowManager) {
  const auto* formattedRef = fmt::ptr(&windowManager_);
  fmt::print("InputManager::InputManager(...): windowManager_ = {}\n", formattedRef);
};

InputManager::~InputManager() {
  if (!listeners_.empty()) {
    fmt::print("Warning: ~InputManager(): listeners are not empty!\n");
  }
}

auto InputManager::init() -> InputManager& {
  // NOTE: re-visit this when mouse is actually needed
  // GLFWwindow* window = windowManager_.getWindow();
  // glfwSetCursorPosCallback(window, InputManager::mouseCallback);

  // NOTE: this may need to be moved in some function
  // Uncomment to tell GLFW to capture our mouse
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return *this;
}

void InputManager::processKeyboardInput() {
  GLFWwindow* window = windowManager_.getWindow();
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
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    for (KeyboardListener* listener : listeners_) {
      listener->onMoveLeft();
    }
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    for (KeyboardListener* listener : listeners_) {
      listener->onMoveRight();
    }
  }
  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
    for (KeyboardListener* listener : listeners_) {
      listener->onZoomOut();
    }
  }
  if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
    for (KeyboardListener* listener : listeners_) {
      listener->onZoomIn();
    }
  }
}

void InputManager::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
  // Debug
  // fmt::print("InputManager::mouseCallback(...): x = {} / y = {}\n", xPos, yPos);
}