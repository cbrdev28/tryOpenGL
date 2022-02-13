#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "WindowManager.h"

/**
 * Input manager for glfw to handle keyboard & mouse.
 * It requires a WindowManager previously initialized.
 */
class InputManager {
 public:
  InputManager(WindowManager& windowManager);

  /**
   * Initialize input manager.
   * Set mouse callback...
   * @throw -1
   * @return InputManager*
   */
  InputManager* init();

  /**
   * Simple keyboard input process
   */
  void processKeyboardInput();

 private:
  WindowManager& _windowManager;

  static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
};

#endif