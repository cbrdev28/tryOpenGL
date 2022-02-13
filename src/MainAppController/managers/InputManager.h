#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "WindowManager.h"

/**
 * Input manager for glfw to handle keyboard & mouse
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

 private:
  WindowManager& _windowManager;

  static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
};

#endif