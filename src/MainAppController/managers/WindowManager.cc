/**
 * Window manager for glfw
 */
#include "WindowManager.h"

#include <fmt/core.h>

#include <chrono>

#include "openGLErrorHelpers.h"

WindowManager::~WindowManager() {
  if (!listeners_.empty()) {
    fmt::print("WARN: ~WindowManager(): listeners are not empty!\n");
  }

  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
    glfwTerminate();
    window_ = nullptr;
  }
}

void WindowManager::init() {
  int initialized = glfwInit();
  if (initialized != GLFW_TRUE) {
    fmt::print("ERROR: failed to glfwInit()\n");
    throw -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef CBR_APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  glfwSetErrorCallback(WindowManager::errorCallback);

  GLFWwindow* window = glfwCreateWindow(width_, height_, "WindowManager", nullptr, nullptr);
  if (window == nullptr) {
    fmt::print("ERROR: failed to glfwCreateWindow(...)\n");
    throw -1;
  }
  window_ = window;
  glfwSetWindowAspectRatio(window, aspectRatio_.numerator, aspectRatio_.denominator);
  glfwMakeContextCurrent(window_);

  // NOLINTNEXTLINE(google-readability-casting, cppcoreguidelines-pro-type-cstyle-cast)
  if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
    fmt::print("ERROR: failed to gladLoadGLLoader(...)\n");
    throw -1;
  }
  fmt::print("OpenGL version: {}\n", glGetString(GL_VERSION));

  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, WindowManager::framebufferSizeCallback);
  glfwSetKeyCallback(window_, WindowManager::keyCallback);
  glfwSwapInterval(0);
}

void WindowManager::updateWindowStats() {
  auto nowTime = std::chrono::steady_clock::now();
  windowStats_.frameDeltaTime =
      std::chrono::duration_cast<std::chrono::duration<float>>(nowTime - windowStats_.endTime);
  windowStats_.endTime = nowTime;
  ++windowStats_.frameCount;
}

void WindowManager::processKeyInput() {
  if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
    for (auto& listener : listeners_) {
      listener->onKeyADown();
    }
  }
  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
    for (auto& listener : listeners_) {
      listener->onKeyWDown();
    }
  }
  if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
    for (auto& listener : listeners_) {
      listener->onKeySDown();
    }
  }
  if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
    for (auto& listener : listeners_) {
      listener->onKeyDDown();
    }
  }
}

void WindowManager::framebufferSizeCallback(int width, int height) {
  width_ = width;
  height_ = height;
  GLCall(glViewport(0, 0, static_cast<GLsizei>(width_), static_cast<GLsizei>(height_)));

  for (auto* listener : listeners_) {
    listener->onResize(width_, height_);
  }
}

/**
 * Callback
 */
void WindowManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  auto* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
  windowManager->framebufferSizeCallback(width, height);
  // Debug
  // fmt::print("framebufferSizeCallback w/ = {}, h = {}", width, height);
  // const auto testSize = WindowManager::listeners_.size();
  // fmt::print("framebufferSizeCallback testSize = {}", testSize);
}

void WindowManager::keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int mods) {
  if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS) && ((mods & WindowManager::defaultKeyModMask) == 0)) {
    glfwSetWindowShouldClose(window, 1 /* true */);
  }
  // fmt::print("Key: {}, scancode: {}, action: {}, mods: {}\n", key, scancode, action, mods);
  // if ((key == GLFW_KEY_ENTER) && ((mods & GLFW_MOD_ALT) != 0) && (action == GLFW_PRESS)) {
  //   // Pressed Alt + Enter
  // }
}

void WindowManager::errorCallback(int code, const char* description) {
  fmt::print("WindowManager::errorCallback(...):\ncode = {}\ndescription = {}\n", code, description);
}
