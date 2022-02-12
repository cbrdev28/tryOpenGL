#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__
/**
 * Window manager for glfw
 */

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