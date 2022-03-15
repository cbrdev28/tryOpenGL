#ifndef WINDOW_MANAGER_H_
#define WINDOW_MANAGER_H_

#include <AspectRatio.h>
#include <KeyboardListener.h>
#include <WindowListener.h>
#include <WindowStats.h>
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
  static const int defaultWidth = 2048;

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
  [[nodiscard]] inline auto getAspectRatio() -> AspectRatio& { return aspectRatio_; };
  [[nodiscard]] inline auto getWindowStats() -> WindowStats& { return windowStats_; };

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

  void updateWindowStats();

 private:
  GLFWwindow* window_{nullptr};
  AspectRatio aspectRatio_{16, 10, 16.0F / 10.0F};
  WindowStats windowStats_;

  static int width;
  static int height;
  static std::vector<WindowListener*> listeners_;
  static std::vector<KeyboardListener*> keyboardListeners_;

  /**
   * Callback
   */
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void errorCallback(int code, const char* description);
};

#endif