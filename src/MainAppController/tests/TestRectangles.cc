#include "TestRectangles.h"

#include <imgui.h>

#include <chrono>

#include "VertexBufferLayout.h"

namespace test {

TestRectangles::TestRectangles(const TestContext& ctx) : Test(ctx) { ctx.windowManager->addWindowListener(this); }

TestRectangles::~TestRectangles() { this->getTestContext().windowManager->removeWindowListener(this); }

void TestRectangles::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;

  if (!useThreads_) {
    // Some update in the main thread
  } else {
    onThreadedUpdate(deltaTime);
  }
}

void TestRectangles::onRender() {
  renderer_.clearColorBackground(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2),
                                 backgroundColor_.at(3));
  // Add renderer draw call
}

void TestRectangles::onImGuiRender() {
  ImGui::NewLine();
  ImGui::Text("FPS: %.2f", 1.0F / deltaTime_);
  auto frameCount = this->getTestContext().windowManager->getWindowStats().frameCount;
  auto startTime = this->getTestContext().windowManager->getWindowStats().startTime;
  auto endTime = this->getTestContext().windowManager->getWindowStats().endTime;
  ImGui::Text("Avg FPS: %.2f",
              static_cast<float>(frameCount) /
                  std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count());
  ImGui::Text("Avg rendering: %.2f ms",
              std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(endTime - startTime).count() /
                  static_cast<float>(frameCount));

  ImGui::NewLine();
  ImGui::Text("Width: %d", this->getTestContext().windowManager->getWidth());
  ImGui::Text("Height: %d", this->getTestContext().windowManager->getHeight());
  ImGui::ColorEdit4("Color", backgroundColor_.data());

  ImGui::NewLine();
  ImGui::Checkbox("Use threads", &useThreads_);
}

void TestRectangles::onKeyADown() {}

void TestRectangles::onKeyWDown() {}

void TestRectangles::onKeySDown() {}

void TestRectangles::onKeyDDown() {}

void TestRectangles::onThreadedUpdate(float dt) {
  // auto threadPool = this->getTestContext().threadPoolManager;
  // Do something with threads
}

}  // namespace test