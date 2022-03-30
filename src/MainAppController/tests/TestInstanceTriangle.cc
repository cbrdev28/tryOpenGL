#include "TestInstanceTriangle.h"

#include <imgui.h>

#include <chrono>

#include "VertexBufferLayout.h"


namespace test {

TestInstanceTriangle::TestInstanceTriangle(const TestContext& ctx) : Test(ctx) {
  va_ = std::make_unique<VertexArray>();

  vbModelVertex0_ =
      std::make_unique<VertexBuffer>(instancedTriangle_.vertices.data(), instancedTriangle_.verticesGLSize());
  vbModelPositions2_ = std::make_unique<VertexBuffer>(nullptr, instancedTriangle_.maxPositionsGLSize(), GL_STREAM_DRAW);
  vbModelZRotationAngle3_ =
      std::make_unique<VertexBuffer>(nullptr, instancedTriangle_.maxZRotationAnglesGLSize(), GL_STREAM_DRAW);

  vbModelVertex0_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbModelPositions2_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbModelZRotationAngle3_->setDivisor(VertexBufferDivisor::FOR_EACH);

  VertexBufferLayout layoutModel;
  layoutModel.pushFloat(2);
  VertexBufferLayout layoutPosition;
  layoutPosition.pushFloat(2);
  VertexBufferLayout layoutRotationAngle;
  layoutRotationAngle.pushFloat(1);

  const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>> vectorOfPairs = {
      {*vbModelVertex0_, layoutModel},
      {*vbModelPositions2_, layoutPosition},
      {*vbModelZRotationAngle3_, layoutRotationAngle},
  };
  va_->setInstanceBufferLayout(vectorOfPairs);

  shader_ = std::make_unique<Shader>("test_instance_triangle.shader");
  shader_->bind();
  shader_->setUniformMat4("u_view", glm::mat4(1.0F));

  const auto aspectRatio = ctx.windowManager->getAspectRatio().ratio;
  shader_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  va_->unBind();
  vbModelZRotationAngle3_->unBind();
  vbModelPositions2_->unBind();
  vbModelVertex0_->unBind();
  shader_->unBind();
}

TestInstanceTriangle::~TestInstanceTriangle() = default;

void TestInstanceTriangle::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;

  if (!useThreads_) {
    debugUpdateStatus_ = "Main";
    instancedTriangle_.onUpdateRotationAngle(deltaTime);
    vbModelZRotationAngle3_->setInstanceData(instancedTriangle_.zRotationAngles.data(),
                                             instancedTriangle_.zRotationAnglesGLSize(),
                                             instancedTriangle_.maxZRotationAnglesGLSize());
    vbModelZRotationAngle3_->unBind();
  } else {
    debugUpdateStatus_ = "Thread";
    onThreadedUpdate(deltaTime);
  }
}

void TestInstanceTriangle::onRender() {
  renderer_.clearColorBackground(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2),
                                 backgroundColor_.at(3));
  renderer_.drawInstance(*shader_, *va_, static_cast<GLsizei>(instancedTriangle_.vertices.size()),
                         static_cast<GLsizei>(instancedTriangle_.positions.size()));
}

void TestInstanceTriangle::onImGuiRender() {
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

  ImGui::Text("Width: %d", this->getTestContext().windowManager->getWidth());
  ImGui::Text("Height: %d", this->getTestContext().windowManager->getHeight());
  ImGui::ColorEdit4("Color", backgroundColor_.data());

  ImGui::NewLine();
  ImGui::Text("Positions count: %.zu", instancedTriangle_.positions.size());
  if (ImGui::Button("Add instance")) {
    this->addTriangleInstance();
  }

  ImGui::NewLine();
  ImGui::Text("Update status: %s", debugUpdateStatus_.c_str());
  ImGui::Checkbox("Use threads", &useThreads_);

  ImGui::NewLine();
  ImGui::SliderInt("Spawn count", &newInstancesCount_, 1, 100, "count = %d");
  if (ImGui::Button("Spawn MANY")) {
    this->addTriangleInstance(newInstancesCount_);
  }
}

void TestInstanceTriangle::addTriangleInstance(int count) {
  for (int i = 0; i < count; ++i) {
    instancedTriangle_.addTriangle();
  }

  vbModelPositions2_->setInstanceData(instancedTriangle_.positions.data(), instancedTriangle_.positionsGLSize(),
                                      instancedTriangle_.maxPositionsGLSize());
  vbModelPositions2_->unBind();
  vbModelZRotationAngle3_->setInstanceData(instancedTriangle_.zRotationAngles.data(),
                                           instancedTriangle_.zRotationAnglesGLSize(),
                                           instancedTriangle_.maxZRotationAnglesGLSize());
  vbModelZRotationAngle3_->unBind();
}

void TestInstanceTriangle::onThreadedUpdate(float dt) {
  unsigned int instancesCount = instancedTriangle_.zRotationAngles.size();
  // Start using thread if we have a "large" amount of instances
  if (instancesCount >= 200) {
    debugUpdateStatus_ = "Threading...";
    unsigned int instancesPerBatch = 100;
    unsigned int currentBatchStartIndex = 0;
    std::vector<std::future<void>> futures = {};
    futures.reserve((instancesCount / instancesPerBatch) + 1);

    do {
      debugUpdateStatus_ = "Queuing...";
      unsigned int batchEndIndex = currentBatchStartIndex + instancesPerBatch;
      if (batchEndIndex > instancesCount) {
        batchEndIndex = instancesCount;
      }

      futures.emplace_back(threadPool.queueTask([&, currentBatchStartIndex, batchEndIndex](std::promise<void> promise) {
        for (unsigned int i = currentBatchStartIndex; i < batchEndIndex; ++i) {
          auto& angle = instancedTriangle_.zRotationAngles.at(i);
          angle += 1.0F * InstancedTriangle::kRotationSpeed * dt;
        }
        promise.set_value();
      }));

      currentBatchStartIndex += instancesPerBatch;
    } while (currentBatchStartIndex < instancesCount);

    for (auto& future : futures) {
      debugUpdateStatus_ = "Waiting...";
      future.wait();
    }

    debugUpdateStatus_ = "Sending...";
    vbModelZRotationAngle3_->setInstanceData(instancedTriangle_.zRotationAngles.data(),
                                             instancedTriangle_.zRotationAnglesGLSize(),
                                             instancedTriangle_.maxZRotationAnglesGLSize());
    vbModelZRotationAngle3_->unBind();
  } else {
    debugUpdateStatus_ = "NOT Threading...";
    // Uncomment to update all instances
    // instancedTriangle_.onUpdateRotationAngle(dt);
    // vbModelZRotationAngle3_->setInstanceData(instancedTriangle_.zRotationAngles.data(),
    //                                          instancedTriangle_.zRotationAnglesGLSize(),
    //                                          instancedTriangle_.maxZRotationAnglesGLSize());
    // vbModelZRotationAngle3_->unBind();
  }
}

}  // namespace test