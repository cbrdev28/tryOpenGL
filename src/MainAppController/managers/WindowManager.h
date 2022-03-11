#ifndef WINDOW_MANAGER_H_
#define WINDOW_MANAGER_H_

#include <WindowListener.h>
#include <openGLHeaders.h>

#include <algorithm>
#include <vector>

/**
 * Window manager for glfw
 */
class WindowManager {
 public:
  WindowManager() = default;
  ~WindowManager();

  WindowManager(const WindowManager& other) = delete;
  WindowManager(WindowManager&& other) = delete;
  auto operator=(const WindowManager& other) -> WindowManager& = delete;
  auto operator=(WindowManager&& other) -> WindowManager& = delete;

  /**
   * Constants
   */
  static const int defaultWidth = 1024;
  static const int defaultHeight = 768;

  /**
   * Initialize window manager: glfw, glad.
   * Create a window & set callbacks: framebuffer size & error
   * @throw -1
   * @return WindowManager&
   */
  auto init() -> WindowManager&;

  [[nodiscard]] auto getWindow() const -> GLFWwindow*;

  [[nodiscard]] inline auto getWidth() const -> int { return WindowManager::width; };
  [[nodiscard]] inline auto getHeight() const -> int { return WindowManager::height; };

  inline auto addWindowListener(WindowListener* listener) -> WindowManager& {
    WindowManager::listeners_.emplace_back(listener);
    return *this;
  }

  inline auto removeWindowListener(WindowListener* listener) -> WindowManager& {
    auto iterator = std::find(listeners_.begin(), listeners_.end(), listener);
    if (iterator != listeners_.end()) {
      listeners_.erase(iterator);
    }
    return *this;
  }

 private:
  GLFWwindow* window_{nullptr};

  static int width;
  static int height;
  static std::vector<WindowListener*> listeners_;

  /**
   * Callback
   */
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void errorCallback(int code, const char* description);
};

#endif