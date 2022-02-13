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
  ~WindowManager();

  /**
   * Constants
   */
  static const int defaultWidth = 1024;
  static const int defaultHeight = 768;

  /**
   * Do not access these directly. Internal usage only.
   */
  static int width;
  static int height;

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