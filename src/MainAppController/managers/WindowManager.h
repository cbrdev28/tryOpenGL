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
   * Initialize window manager
   * @return -1 if an error occurred, 0 otherwise
   */
  int init();
};

#endif