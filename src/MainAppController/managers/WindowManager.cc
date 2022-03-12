/**
 * Window manager for glfw
 */
#include "WindowManager.h"

#include <fmt/core.h>
#include <openGLErrorHelpers.h>
#include <openGLHeaders.h>

// Initialize static class variables
int WindowManager::width = WindowManager::defaultWidth;
int WindowManager::height = WindowManager::defaultHeight;
// Init to empty
std::vector<WindowListener*> WindowManager::listeners_ = {};

WindowManager::~WindowManager() {
  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
    glfwTerminate();
    window_ = nullptr;
  }
}

/**
 * Public
 */
auto WindowManager::init() -> WindowManager& {
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

  glfwSetErrorCallback(WindowManager::errorCallback);

  const auto aspectRatioHeight = WindowManager::defaultWidth / aspectRatio_.ratio;
  GLFWwindow* window = glfwCreateWindow(WindowManager::defaultWidth, static_cast<int>(aspectRatioHeight),
                                        "WindowManager", nullptr, nullptr);
  if (window == nullptr) {
    fmt::print("Failed to glfwCreateWindow(...)\n");
    throw -1;
  }
  window_ = window;
  glfwSetWindowAspectRatio(window, aspectRatio_.numerator, aspectRatio_.denominator);
  glfwMakeContextCurrent(window_);

  // NOLINTNEXTLINE(google-readability-casting, cppcoreguidelines-pro-type-cstyle-cast)
  if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
    fmt::print("Failed to gladLoadGLLoader(...)\n");
    throw -1;
  }

  glfwSetFramebufferSizeCallback(window_, WindowManager::framebufferSizeCallback);
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
  GLCall(glViewport(0, 0, width, height));

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
