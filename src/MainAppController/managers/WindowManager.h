#ifndef WINDOW_MANAGER_H_
#define WINDOW_MANAGER_H_

#include "AspectRatio.h"
#include "WindowListener.h"
#include "WindowStats.h"

/**
 * Window manager for glfw
 */
class WindowManager {
 public:
  WindowManager();
  ~WindowManager();

  WindowManager(const WindowManager& other) = delete;
  WindowManager(WindowManager&& other) = delete;
  auto operator=(const WindowManager& other) -> WindowManager& = delete;
  auto operator=(WindowManager&& other) -> WindowManager& = delete;

  static constexpr int defaultWidth = 1280;
  static constexpr int defaultKeyModMask = GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT | GLFW_MOD_SUPER;

  /**
   * Initialize window manager: glfw, glad.
   * Create a window & set callbacks: framebuffer size & error
   * @throw -1
   */
  void init();
  void updateWindowStats();
  void processKeyInput();
  void setWindowShouldClose();
  void setCurrentContext(GLFWwindow* window);

  [[nodiscard]] inline auto getWindow() const -> GLFWwindow* { return window_; };
  [[nodiscard]] inline auto getWidth() const -> int { return width_; };
  [[nodiscard]] inline auto getHeight() const -> int { return height_; };
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

 private:
  GLFWwindow* window_{nullptr};
  AspectRatio aspectRatio_{16, 9, 16.0F / 9.0F};
  WindowStats windowStats_;

  // Update this count when supporting new keys (used to reserve memory)
  static constexpr unsigned int kKeyMapCount = 4;
  // We depend directly on GLFW key code (to keep track of keys pressed down)
  std::unordered_map<int /* keyCode */, bool /* pressedDown */> pressedDownKeysMap_;

  int width_{WindowManager::defaultWidth};
  int height_{static_cast<int>(static_cast<float>(width_) / aspectRatio_.ratio)};
  std::vector<WindowListener*> listeners_{};
  void framebufferSizeCallback(int width, int height);

  /**
   * Callback
   */
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void errorCallback(int code, const char* description);
};

#endif
