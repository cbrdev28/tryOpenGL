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
   * Initialize window manager: glfw, glad.
   * Create a window & set callbacks: framebuffer size & error
   * @throw -1
   * @return WindowManager*
   */
  WindowManager* init();

 private:
  GLFWwindow* _window;
  static int width;
  static int height;

  /**
   * Callback
   */
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void errorCallback(int code, const char* description);
};

#endif