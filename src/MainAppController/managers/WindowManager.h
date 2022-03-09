#ifndef WINDOW_MANAGER_H_
#define WINDOW_MANAGER_H_

#include <WindowListener.h>
#include <openGLHeaders.h>

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

  [[nodiscard]] auto getWidth() const -> int { return WindowManager::width; };
  [[nodiscard]] auto getHeight() const -> int { return WindowManager::height; };

  auto addWindowListener(WindowListener* listener) -> WindowManager& {
    WindowManager::listeners_.emplace_back(listener);
    return *this;
  }

 private:
  GLFWwindow* window_{nullptr};

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static int width;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static int height;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static std::vector<WindowListener*> listeners_;

  /**
   * Callback
   */
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void errorCallback(int code, const char* description);
};

#endif