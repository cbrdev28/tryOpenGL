/**
 * World manager to define what to draw & how to interact.
 */
#include "WorldManager.h"

#include <VertexBufferLayout.h>

#include "basicFirstForms.h"

auto WorldManager::init(const float windowWidth, const float windowHeight) -> WorldManager& {
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
  shaderManager_.setUniformMat4("u_model", matrixHelper_.model);
  shaderManager_.setUniformMat4("u_view", matrixHelper_.view);
  shaderManager_.setUniformMat4("u_projection", matrixHelper_.projection);

  vao_->unBind();
  vbo_->unBind();
  ibo_->unBind();
  shaderManager_.unBind();
  return *this;
}

// NOTE: this function is called during the render loop!
auto WorldManager::render() -> WorldManager& {
  renderer_.clearColorBackground(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]);

  shaderManager_.bind();
  // Update view matrix for camera movement
  shaderManager_.setUniformMat4("u_view", matrixHelper_.view);
  // Update projection matrix for window size change & aspect ratio
  shaderManager_.setUniformMat4("u_projection", matrixHelper_.projection);

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

void WorldManager::onResize(int width, int height) {
  matrixHelper_.updateProjection(static_cast<float>(width), static_cast<float>(height));
}

// Helper function called when moving camera
[[nodiscard]] auto WorldManager::getCameraSpeedByFrame_() const -> float {
  const double speedByFrame = basicCameraSpeed * deltaTimeFrame_;
  return static_cast<float>(speedByFrame);
}
