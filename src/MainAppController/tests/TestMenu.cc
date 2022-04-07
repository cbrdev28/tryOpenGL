#include "cbrpch.h"

#include "TestMenu.h"

#include "tryOpenGLConfig.h"

namespace test {

TestMenu::~TestMenu() {
  if (currentTest_ != nullptr) {
    delete currentTest_;
    currentTest_ = nullptr;
  }
}

void TestMenu::onUpdate(float deltaTime) {
  if (currentTest_ != nullptr) {
    currentTest_->onUpdate(deltaTime);
  }
}

void TestMenu::onRender() {
  if (currentTest_ != nullptr) {
    currentTest_->onRender();
  } else {
    renderer_.clearColorBackground(0.1F, 0.1F, 0.1F, 1.0F);
  }
}

void TestMenu::onImGuiRender() {
  ImGui::Begin("Test Menu");
  if (currentTest_ != nullptr) {
    if (ImGui::Button("Back")) {
      delete currentTest_;
      currentTest_ = nullptr;
      ImGui::End();
      return;
    }
    currentTest_->onImGuiRender();
  } else {
    ImGui::Text(
        "Good evening: %s",
        fmt::format("v{}.{}.{}", tryOpenGL_VERSION_MAJOR, tryOpenGL_VERSION_MINOR, tryOpenGL_VERSION_PATCH).c_str());

    if (ImGui::Button("Quit")) {
      this->getTestContext().windowManager->setWindowShouldClose();
    }
    for (auto& test : tests_) {
      if (ImGui::Button(test.first.c_str())) {
        currentTest_ = test.second(this->getTestContext());
      }
    }
  }
  ImGui::End();
}

}  // namespace test