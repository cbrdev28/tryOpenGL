/**
 * World manager to define what to draw & how to interact.
 */
#include "WorldManager.h"

#include <fmt/core.h>
#include <fmt/format.h>

#include "basicBackgroundColor.h"
#include "basicSquare.h"

WorldManager::WorldManager(WindowManager& windowManager, InputManager& inputManager)
    : windowManager_(windowManager), inputManager_(inputManager) {
  const auto* formattedRef = fmt::ptr(&windowManager_);
  fmt::print("WorldManager::WorldManager(...): windowManager_ = {}\n", formattedRef);
  formattedRef = fmt::ptr(&inputManager_);
  fmt::print("WorldManager::WorldManager(...): inputManager_ = {}\n", formattedRef);
}

auto WorldManager::init() -> WorldManager& {
  GLCall(glEnable(GL_DEPTH_TEST));
  // Init shader manager to load, compile & link default shaders
  shaderManager_.init();

  // Setup VBO: Vertex Buffer Object
  const int numberOfBuffer = 1;
  GLCall(glGenBuffers(numberOfBuffer, &VBO_));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO_));
  GLCall(glBufferData(GL_ARRAY_BUFFER, basicSquareVerticesSizeOf * basicSquareIndicedVertices.size(),
                      basicSquareIndicedVertices.data(), GL_STATIC_DRAW));

  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, basicSquareVertexSize, GL_FLOAT, GL_FALSE,
                               basicSquareVertexSize * basicSquareVerticesSizeOf, nullptr));

  // Setup IBO: Index Buffer Object
  GLCall(glGenBuffers(numberOfBuffer, &IBO_));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, basicSquareIndicesSizeOf * basicSquareIndices.size(),
                      basicSquareIndices.data(), GL_STATIC_DRAW));

  // Set our default "look at" camera in the view matrix
  matrixHelper_.updateView(cameraPosition_ + basicCameraPositionOffset, cameraPosition_ + basicCameraTarget,
                           basicCameraUp);
  // Set perspective in the projection matrix
  matrixHelper_.updateProjection(static_cast<float>(windowManager_.getWidth()),
                                 static_cast<float>(windowManager_.getHeight()));
  // Set matrix in shader
  GLCall(glUseProgram(shaderManager_.getShaderProgramID()));
  shaderManager_.setModelMatrix(matrixHelper_.model)
      .setViewMatrix(matrixHelper_.view)
      .setProjectionMatrix(matrixHelper_.projection);

  // Subscribe as listener
  windowManager_.addWindowListener(this);
  inputManager_.addKeyboardListener(this);
  return *this;
}

// NOTE: this function is called during the render loop!
auto WorldManager::render() -> WorldManager& {
  updateDeltaTimeFrame_(glfwGetTime());
  GLCall(glClearColor(basicBackgroundNeonPinkR, basicBackgroundNeonPinkG, basicBackgroundNeonPinkB, 1.0F));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  GLCall(glUseProgram(shaderManager_.getShaderProgramID()));
  // Update view matrix for camera movement
  shaderManager_.setViewMatrix(matrixHelper_.view);
  // Update projection matrix for window size change & aspect ratio
  shaderManager_.setProjectionMatrix(matrixHelper_.projection);

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
