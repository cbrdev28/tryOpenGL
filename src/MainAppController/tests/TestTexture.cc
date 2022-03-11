#include "TestTexture.h"

#include <imgui.h>
#include <openGLErrorHelpers.h>

namespace test {

TestTexture::TestTexture(const TestContext& ctx) : Test(ctx) {}
TestTexture::~TestTexture() = default;

void TestTexture::onUpdate(float deltaTime) {}
void TestTexture::onRender() {
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));
}
void TestTexture::onImGuiRender() { ImGui::ColorEdit4("Color", backgroundColor_.data()); }

}  // namespace test