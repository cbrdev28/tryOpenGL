/**
 * World manager to define what to draw & how to interact.
 */
#include "WorldManager.h"

#include <Renderer.h>
#include <VertexBufferLayout.h>
#include <basicBackgroundColor.h>
#include <basicSquare.h>

WorldManager::WorldManager(WindowManager& windowManager, InputManager& inputManager)
    : windowManager_(windowManager), inputManager_(inputManager) {}

auto WorldManager::init() -> WorldManager& {
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
  matrixHelper_.updateProjection(static_cast<float>(windowManager_.getWidth()),
                                 static_cast<float>(windowManager_.getHeight()));
  shaderManager_.bind();
  shaderManager_.setModelMatrix(matrixHelper_.model)
      .setViewMatrix(matrixHelper_.view)
      .setProjectionMatrix(matrixHelper_.projection);

  windowManager_.addWindowListener(this);
  inputManager_.addKeyboardListener(this);

  vao_->unBind();
  vbo_->unBind();
  ibo_->unBind();
  shaderManager_.unBind();
  return *this;
}

// NOTE: this function is called during the render loop!
auto WorldManager::render() -> WorldManager& {
  updateDeltaTimeFrame_(glfwGetTime());
  GLCall(glClearColor(basicBackgroundNeonPinkR, basicBackgroundNeonPinkG, basicBackgroundNeonPinkB, 1.0F));
  GLCall(glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/));

  shaderManager_.bind();
  // Update view matrix for camera movement
  shaderManager_.setViewMatrix(matrixHelper_.view);
  // Update projection matrix for window size change & aspect ratio
  shaderManager_.setProjectionMatrix(matrixHelper_.projection);

  vao_->bind();
  ibo_->bind();
  // Draw one tile from indices
  GLCall(glDrawElements(GL_TRIANGLES, basicSquareIndices.size(), GL_UNSIGNED_INT, nullptr));
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
  const auto windowWidth = static_cast<float>(width);
  const auto windowHeight = static_cast<float>(height);
  matrixHelper_.updateProjection(windowWidth, windowHeight);
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
