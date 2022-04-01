#include "TestSquareWithTriangles.h"

#include <imgui.h>

#include <chrono>

#include "VertexBufferLayout.h"

namespace test {

TestSquareWithTriangles::TestSquareWithTriangles(const TestContext& ctx) : Test(ctx) {
  this->initTriangles();
  this->initSquare();
}

TestSquareWithTriangles::~TestSquareWithTriangles() = default;

void TestSquareWithTriangles::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;

  if (!useThreads_) {
    debugUpdateStatus_ = "Main";
    instancedTriangle_->onUpdateRotationAngle(deltaTime);
    vbTrianglesZAngles_->setInstanceData(instancedTriangle_->zRotationAngles.data(),
                                         instancedTriangle_->zRotationAnglesGLSize(),
                                         instancedTriangle_->maxZRotationAnglesGLSize());
    vbTrianglesZAngles_->unBind();
  } else {
    debugUpdateStatus_ = "Thread";
    onThreadedUpdate(deltaTime);
  }
}

void TestSquareWithTriangles::onRender() {
  renderer_.clearColorBackground(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2),
                                 backgroundColor_.at(3));
  renderer_.drawInstance(*shaderTriangles_, *vaTriangles_, static_cast<GLsizei>(instancedTriangle_->vertices.size()),
                         static_cast<GLsizei>(instancedTriangle_->positions.size()));

  renderer_.drawInstance(*shaderSquare_, *vaSquare_, static_cast<GLsizei>(targetSquare_->vertices.size()),
                         static_cast<GLsizei>(1));
}

void TestSquareWithTriangles::onImGuiRender() {
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
  ImGui::Text("Positions count: %.zu", instancedTriangle_->positions.size());
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

void TestSquareWithTriangles::addTriangleInstance(int count) {
  for (int i = 0; i < count; ++i) {
    // instancedTriangle_->addTriangle();
    instancedTriangle_->spawnTriangle({0.0F, 0.0F});
  }

  vbTrianglesPositions_->setInstanceData(instancedTriangle_->positions.data(), instancedTriangle_->positionsGLSize(),
                                         instancedTriangle_->maxPositionsGLSize());
  vbTrianglesPositions_->unBind();
  vbTrianglesZAngles_->setInstanceData(instancedTriangle_->zRotationAngles.data(),
                                       instancedTriangle_->zRotationAnglesGLSize(),
                                       instancedTriangle_->maxZRotationAnglesGLSize());
  vbTrianglesZAngles_->unBind();
}

void TestSquareWithTriangles::onThreadedUpdate(float dt) {
  auto threadPool = this->getTestContext().threadPoolManager;
  unsigned int instancesCount = instancedTriangle_->zRotationAngles.size();
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

      futures.emplace_back(
          threadPool->queueTask([&, currentBatchStartIndex, batchEndIndex](std::promise<void> promise) {
            for (unsigned int i = currentBatchStartIndex; i < batchEndIndex; ++i) {
              auto& angle = instancedTriangle_->zRotationAngles.at(i);
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
    vbTrianglesZAngles_->setInstanceData(instancedTriangle_->zRotationAngles.data(),
                                         instancedTriangle_->zRotationAnglesGLSize(),
                                         instancedTriangle_->maxZRotationAnglesGLSize());
    vbTrianglesZAngles_->unBind();
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

void TestSquareWithTriangles::initTriangles() {
  vaTriangles_ = std::make_unique<VertexArray>();

  vbTrianglesVertices_ =
      std::make_unique<VertexBuffer>(instancedTriangle_->vertices.data(), instancedTriangle_->verticesGLSize());
  vbTrianglesPositions_ =
      std::make_unique<VertexBuffer>(nullptr, instancedTriangle_->maxPositionsGLSize(), GL_STREAM_DRAW);
  vbTrianglesZAngles_ =
      std::make_unique<VertexBuffer>(nullptr, instancedTriangle_->maxZRotationAnglesGLSize(), GL_STREAM_DRAW);

  vbTrianglesVertices_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbTrianglesPositions_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbTrianglesZAngles_->setDivisor(VertexBufferDivisor::FOR_EACH);

  VertexBufferLayout layoutModel;
  layoutModel.pushFloat(2);
  VertexBufferLayout layoutPosition;
  layoutPosition.pushFloat(2);
  VertexBufferLayout layoutRotationAngle;
  layoutRotationAngle.pushFloat(1);

  const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>> vectorOfPairs = {
      {*vbTrianglesVertices_, layoutModel},
      {*vbTrianglesPositions_, layoutPosition},
      {*vbTrianglesZAngles_, layoutRotationAngle},
  };
  vaTriangles_->setInstanceBufferLayout(vectorOfPairs);

  shaderTriangles_ = std::make_unique<Shader>("test_instance_triangle.shader");
  shaderTriangles_->bind();
  shaderTriangles_->setUniformMat4("u_view", glm::mat4(1.0F));

  const auto aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shaderTriangles_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  vaTriangles_->unBind();
  vbTrianglesZAngles_->unBind();
  vbTrianglesPositions_->unBind();
  vbTrianglesVertices_->unBind();
  shaderTriangles_->unBind();
}

void TestSquareWithTriangles::initSquare() {
  vaSquare_ = std::make_unique<VertexArray>();

  vbSquareVertices_ = std::make_unique<VertexBuffer>(targetSquare_->vertices.data(), targetSquare_->verticesGLSize());
  vbSquarePosition_ =
      std::make_unique<VertexBuffer>(&targetSquare_->position, sizeof(targetSquare_->position), GL_STREAM_DRAW);
  vbSquareZAngle_ =
      std::make_unique<VertexBuffer>(&targetSquare_->zAngle, sizeof(targetSquare_->zAngle), GL_STREAM_DRAW);

  vbSquareVertices_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbSquarePosition_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbSquareZAngle_->setDivisor(VertexBufferDivisor::FOR_EACH);

  VertexBufferLayout layoutModel;
  layoutModel.pushFloat(2);
  VertexBufferLayout layoutPosition;
  layoutPosition.pushFloat(2);
  VertexBufferLayout layoutRotationAngle;
  layoutRotationAngle.pushFloat(1);

  const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>> vectorOfPairs = {
      {*vbSquareVertices_, layoutModel},
      {*vbSquarePosition_, layoutPosition},
      {*vbSquareZAngle_, layoutRotationAngle},
  };
  vaSquare_->setInstanceBufferLayout(vectorOfPairs);

  shaderSquare_ = std::make_unique<Shader>("test_square_triangle.shader");
  shaderSquare_->bind();
  shaderSquare_->setUniformMat4("u_view", glm::mat4(1.0F));

  const auto aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shaderSquare_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  vaSquare_->unBind();
  vbSquareZAngle_->unBind();
  vbSquarePosition_->unBind();
  vbSquareVertices_->unBind();
  shaderSquare_->unBind();
}

}  // namespace test