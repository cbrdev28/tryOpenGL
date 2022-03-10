#ifndef IM_GUI_MANAGER_H_
#define IM_GUI_MANAGER_H_

#include <WindowManager.h>
#include <openGLHeaders.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGuiManager {
 public:
  explicit ImGuiManager(const WindowManager& windowManager);
  ~ImGuiManager();

  ImGuiManager(const ImGuiManager& other) = delete;
  ImGuiManager(ImGuiManager&& other) = delete;
  auto operator=(const ImGuiManager& other) -> ImGuiManager& = delete;
  auto operator=(ImGuiManager&& other) -> ImGuiManager& = delete;

  void init();
  void renderFrame();
  void render();

  void renderExample();

 private:
  const WindowManager& windowManager_;
  ImGuiContext* imGuiContext_{nullptr};
};

#endif