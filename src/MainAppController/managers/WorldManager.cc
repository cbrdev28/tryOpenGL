/**
 * World manager to define what to draw & how to interact.
 */
#include "WorldManager.h"

#include <VertexBufferLayout.h>
#include <basicCamera.h>
#include <basicSquare.h>

auto WorldManager::init(const float windowWidth, const float windowHeight) -> WorldManager& {
  // GLCall(glEnable(GL_DEPTH_TEST));
  shaderManager_.init();

  vao_ = std::make_unique<VertexArray>();
  vbo_ = std::make_unique<VertexBuffer>(basicSquareIndicedVertices.data(),
                                        basicSquareVerticesSizeOf * basicSquareIndicedVertices.size());
  ibo_ = std::make_unique<IndexBuffer>(basicSquareIndices.data(), basicSquareIndices.size());

  VertexBufferLayout layout;
  layout.pushFloat(basicSquareVertexSize);
  vao_->addBuffer(*vbo_, layout);

  // Set our default "look at" camera in the view matrix
  matrixHelper_.updateView(cameraPosition_ + basicCameraPositionOffset, cameraPosition_ + basicCameraTarget,
                           basicCameraUp);
  // Set perspective in the projection matrix based on screen size
  matrixHelper_.updateProjection(windowWidth, windowHeight);
  shaderManager_.bind();
  shaderManager_.setModelMatrix(matrixHelper_.model)
      .setViewMatrix(matrixHelper_.view)
      .setProjectionMatrix(matrixHelper_.projection);

  vao_->unBind();
  vbo_->unBind();
  ibo_->unBind();
  shaderManager_.unBind();
  return *this;
}

// NOTE: this function is called during the render loop!
auto WorldManager::render() -> WorldManager& {
  updateDeltaTimeFrame_(glfwGetTime());
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));

  shaderManager_.bind();
  // Update view matrix for camera movement
  shaderManager_.setViewMatrix(matrixHelper_.view);
  // Update projection matrix for window size change & aspect ratio
  shaderManager_.setProjectionMatrix(matrixHelper_.projection);

  renderer_.draw(shaderManager_, *vao_, *ibo_);
  return *this;
}

/**
 * Overrides
 */
void WorldManager::onMoveForward() {
  cameraPosition_ = cameraPosition_ + basicCameraYAxis * getCameraSpeedByFrame_();
  matrixHelper_.updateView(cameraPosition_ + basicCameraPositionOffset, cameraPosition_ + basicCameraTarget,
                           basicCameraUp);
}

void WorldManager::onMoveBackward() {
  cameraPosition_ = cameraPosition_ - basicCameraYAxis * getCameraSpeedByFrame_();
  matrixHelper_.updateView(cameraPosition_ + basicCameraPositionOffset, cameraPosition_ + basicCameraTarget,
                           basicCameraUp);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void WorldManager::onResize(int width, int height) {
  matrixHelper_.updateProjection(static_cast<float>(width), static_cast<float>(height));
}

// Called during render
auto WorldManager::updateDeltaTimeFrame_(double currentTimeFrame) -> WorldManager& {
  deltaTimeFrame_ = currentTimeFrame - lastTimeFrame_;
  lastTimeFrame_ = currentTimeFrame;
  return *this;
}

// Helper function called when moving camera
[[nodiscard]] auto WorldManager::getCameraSpeedByFrame_() const -> float {
  const double speedByFrame = basicCameraSpeed * deltaTimeFrame_;
  return static_cast<float>(speedByFrame);
}
