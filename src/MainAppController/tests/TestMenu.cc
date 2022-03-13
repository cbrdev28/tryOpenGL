#include "TestMenu.h"

#include <imgui.h>
#include <openGLErrorHelpers.h>
#include <openGLHeaders.h>

namespace test {

void TestMenu::onUpdate(float deltaTime) {
  if (currentTest_ != nullptr) {
    currentTest_->onUpdate(deltaTime);
  }
}

void TestMenu::onRender() {
  if (currentTest_ != nullptr) {
    currentTest_->onRender();
  } else {
    GLCall(glClearColor(0.0F, 0.0F, 0.0F, 1.0F));
  }
}

void TestMenu::onImGuiRender() {
  if (currentTest_ != nullptr) {
    if (ImGui::Button("Back")) {
      delete currentTest_;
      currentTest_ = nullptr;
      return;
    }
    currentTest_->onImGuiRender();
  } else {
    for (auto& test : tests_) {
      if (ImGui::Button(test.first.c_str())) {
        currentTest_ = test.second(this->getTestContext());
      }
    }
  }
}

}  // namespace test