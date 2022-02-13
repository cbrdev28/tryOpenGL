#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__
/**
 * Window manager for glfw
 */

// Disable clang-format because we must include glad before GLFW
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

class WindowManager {
 public:
  WindowManager();

  /**
   * Initialize window manager: glfw, glad.
   * Create a window
   * @throw -1
   * @return WindowManager*
   */
  WindowManager* init();

 private:
  GLFWwindow* _window;
};

/**
 * Callback
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif