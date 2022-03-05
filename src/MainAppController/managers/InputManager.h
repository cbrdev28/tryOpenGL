#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <vector>

#include "KeyboardListener.h"
#include "WindowManager.h"

/**
 * Input manager for glfw to handle keyboard & mouse.
 * It requires a WindowManager previously initialized.
 */
class InputManager {
 public:
  explicit InputManager(WindowManager& windowManager);

  /**
   * Initialize input manager.
   * Set mouse callback...
   * @throw -1
   * @return InputManager&
   */
  auto init() -> InputManager&;

  /**
   * Simple keyboard input process
   */
  void processKeyboardInput();

  /**
   * Add keyboard listener
   * @return InputManager&
   */
  auto addKeyboardListener(KeyboardListener* listener) -> InputManager& {
    listeners_.emplace_back(listener);
    return *this;
  }

 private:
  WindowManager& windowManager_;
  std::vector<KeyboardListener*> listeners_{};

  static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
};

#endif