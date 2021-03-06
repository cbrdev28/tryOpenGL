/**
 * Window manager for glfw
 */
#include "cbrpch.h"

#include "WindowManager.h"

WindowManager::WindowManager() { pressedDownKeysMap_.reserve(WindowManager::kKeyMapCount); }

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
  glfwSwapInterval(0);  // Set to 1 to enabled VSync
}

void WindowManager::updateWindowStats() {
  auto nowTime = std::chrono::steady_clock::now();
  windowStats_.frameDeltaTime =
      std::chrono::duration_cast<std::chrono::duration<float>>(nowTime - windowStats_.endTime);
  windowStats_.endTime = nowTime;
  ++windowStats_.frameCount;
}

void WindowManager::processKeyInput() {
  // As long as a key is not "released", we mark it as "pressed down"
  pressedDownKeysMap_[GLFW_KEY_W] = glfwGetKey(window_, GLFW_KEY_W) != GLFW_RELEASE;
  pressedDownKeysMap_[GLFW_KEY_A] = glfwGetKey(window_, GLFW_KEY_A) != GLFW_RELEASE;
  pressedDownKeysMap_[GLFW_KEY_S] = glfwGetKey(window_, GLFW_KEY_S) != GLFW_RELEASE;
  pressedDownKeysMap_[GLFW_KEY_D] = glfwGetKey(window_, GLFW_KEY_D) != GLFW_RELEASE;
}

void WindowManager::setWindowShouldClose() { glfwSetWindowShouldClose(window_, 1 /* true */); }

void WindowManager::setCurrentContext(GLFWwindow* window) {
  ASSERT(window == window_);
  glfwMakeContextCurrent(window_);
}

void WindowManager::framebufferSizeCallback(int width, int height) {
  width_ = width;
  height_ = height;
  GLCall(glViewport(0, 0, static_cast<GLsizei>(width_), static_cast<GLsizei>(height_)));

  for (auto* listener : listeners_) {
    listener->onResize(width_, height_);
  }
}

void WindowManager::keyCallback(int key, int scancode, int action, int mods) {
  if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS) && ((mods & WindowManager::defaultKeyModMask) == 0)) {
    this->setWindowShouldClose();
  }

  for (auto* listener : listeners_) {
    listener->onKeyCallback(key, scancode, action, mods);
  }
  // Debug:
  // fmt::print("Key: {}, scancode: {}, action: {}, mods: {}\n", key, scancode, action, mods);
}

/**
 * Callback
 */
void WindowManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  auto* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
  windowManager->framebufferSizeCallback(width, height);
  // Debug
  // fmt::print("framebufferSizeCallback w/ = {}, h = {}", width, height);
}

void WindowManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
  windowManager->keyCallback(key, scancode, action, mods);
  // Debug
  // fmt::print("Key: {}, scancode: {}, action: {}, mods: {}\n", key, scancode, action, mods);
  // if ((key == GLFW_KEY_ENTER) && ((mods & GLFW_MOD_ALT) != 0) && (action == GLFW_PRESS)) {
  //   // Pressed Alt + Enter
  // }
}

void WindowManager::errorCallback(int code, const char* description) {
  fmt::print("WindowManager::errorCallback(...):\ncode = {}\ndescription = {}\n", code, description);
}
