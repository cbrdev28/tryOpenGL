/**
 * Window manager for glfw
 */
#include "WindowManager.h"

// Initialize static class variables
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables, cppcoreguidelines-interfaces-global-init)
int WindowManager::width = WindowManager::defaultWidth;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables, cppcoreguidelines-interfaces-global-init)
int WindowManager::height = WindowManager::defaultHeight;

// Init to empty
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::vector<WindowListener*> WindowManager::listeners_ = {};

WindowManager::WindowManager() { fmt::print("WindowManager::WindowManager()\n"); }

WindowManager::~WindowManager() {
  fmt::print("WindowManager::~WindowManager()\n");
  if (window_ != nullptr) {
    fmt::print("WindowManager::~WindowManager(): glfwTerminate()...\n");
    glfwTerminate();
    window_ = nullptr;
  }
}

/**
 * Public
 */
auto WindowManager::init() -> WindowManager& {
  fmt::print("WindowManager::init()\n");

  int initialized = glfwInit();
  if (initialized != GLFW_TRUE) {
    fmt::print("Failed to glfwInit()\n");
    throw -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // For Apple
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWerrorfun errorCallback = glfwSetErrorCallback(WindowManager::errorCallback);
  if (errorCallback == nullptr) {
    // Only warning
    const auto* formattedPointer = fmt::ptr(errorCallback);
    fmt::print("Warning glfwSetErrorCallback(...): {}\n", formattedPointer);
  }

  GLFWwindow* window =
      glfwCreateWindow(WindowManager::defaultWidth, WindowManager::defaultHeight, "WindowManager", nullptr, nullptr);
  if (window == nullptr) {
    fmt::print("Failed to glfwCreateWindow(...)\n");
    throw -1;
  }
  window_ = window;
  glfwMakeContextCurrent(window_);

  // NOLINTNEXTLINE(google-readability-casting, cppcoreguidelines-pro-type-cstyle-cast)
  if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
    fmt::print("Failed to gladLoadGLLoader(...)\n");
    throw -1;
  }

  GLFWframebuffersizefun fsCallback = glfwSetFramebufferSizeCallback(window_, WindowManager::framebufferSizeCallback);
  if (fsCallback == nullptr) {
    // Only warning for now, since it seems to be working anyway
    const auto* formattedPointer = fmt::ptr(fsCallback);
    fmt::print("Warning glfwSetFramebufferSizeCallback(...): {}\n", formattedPointer);
  }
  return *this;
}

auto WindowManager::getWindow() const -> GLFWwindow* { return window_; }

/**
 * Private
 */

/**
 * Callback
 */
void WindowManager::framebufferSizeCallback(GLFWwindow* /* window */, int width, int height) {
  glViewport(0, 0, width, height);

  WindowManager::width = width;
  WindowManager::height = height;

  for (WindowListener* listener : WindowManager::listeners_) {
    listener->onResize(width, height);
  }

  // Debug
  // fmt::print("framebufferSizeCallback w/ = {}, h = {}", width, height);
  // const auto testSize = WindowManager::listeners_.size();
  // fmt::print("framebufferSizeCallback testSize = {}", testSize);
}

void WindowManager::errorCallback(int code, const char* description) {
  fmt::print("WindowManager::errorCallback(...):\ncode = {}\ndescription = {}\n", code, description);
}
