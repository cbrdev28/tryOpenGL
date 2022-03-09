#include "ImGuiManager.h"

ImGuiManager::ImGuiManager(const WindowManager& windowManager) : windowManager_(windowManager) {}

ImGuiManager::~ImGuiManager() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiManager::init() {
  IMGUI_CHECKVERSION();
  imGuiContext_ = ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(windowManager_.getWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiManager::renderFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiManager::render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::renderExample() {
  ImGui::Begin("Example");
  const float frameRatePerSecond = 1000.0F;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", frameRatePerSecond / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
  ImGui::End();
}