#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "WindowManager.h"

/**
 * Input manager for glfw to handle keyboard & mouse
 */
class InputManager {
 public:
  InputManager(WindowManager& windowManager);

 private:
  WindowManager& _windowManager;
};

#endif