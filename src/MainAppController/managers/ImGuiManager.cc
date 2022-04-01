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

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplGlfw_InitForOpenGL(windowManager_.getWindow(), true);
#ifdef CBR_APPLE
  ImGui_ImplOpenGL3_Init("#version 150");
#else
  ImGui_ImplOpenGL3_Init("#version 130");
#endif
}

void ImGuiManager::renderFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

  ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
  dockspace_flags |= ImGuiDockNodeFlags_NoDockingInCentralNode;
  ImGui::DockSpaceOverViewport(main_viewport, dockspace_flags);
}

void ImGuiManager::render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
    GLFWwindow* main_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(main_context);
  }
}

void ImGuiManager::renderExample() {
  ImGui::Begin("Example");
  const float frameRatePerSecond = 1000.0F;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", frameRatePerSecond / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
  ImGui::End();
}
