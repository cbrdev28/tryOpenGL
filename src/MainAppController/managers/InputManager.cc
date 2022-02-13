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
  return this;
}