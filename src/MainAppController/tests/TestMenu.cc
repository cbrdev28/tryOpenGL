#include "TestMenu.h"

#include <imgui.h>

namespace test {

TestMenu::TestMenu(Test*& currentTest) : currentTest_(currentTest) {}

void TestMenu::onUpdate(float deltaTime) {
  if (currentTest_ != nullptr) {
    currentTest_->onUpdate(deltaTime);
  }
}

void TestMenu::onRender() {
  if (currentTest_ != nullptr) {
    currentTest_->onRender();
  }
}

void TestMenu::onImGuiRender() {
  if (currentTest_ != nullptr) {
    if (ImGui::Button("Back")) {
      currentTest_ = nullptr;
      return;
    }
    currentTest_->onImGuiRender();
  } else {
    for (auto& test : tests_) {
      if (ImGui::Button(test.first.c_str())) {
        currentTest_ = test.second();
      }
    }
  }
}

}  // namespace test