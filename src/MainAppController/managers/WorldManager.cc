/**
 * World manager to define what to draw & how to interact.
 */
#include "WorldManager.h"

#include <fmt/core.h>
#include <fmt/format.h>

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
  GLCall(glBufferData(GL_ARRAY_BUFFER, basicSquareSizeOf * basicSquareVertices.size(), basicSquareVertices.data(),
                      GL_STATIC_DRAW));

  const GLuint vertexIndex = 0;  // Match with layout = 0 in our shader
  GLCall(glEnableVertexAttribArray(vertexIndex));
  GLCall(glVertexAttribPointer(vertexIndex, basicSquareVertexSize, GL_FLOAT, GL_FALSE,
                               basicSquareVertexSize * basicSquareSizeOf, nullptr));

  // Setup IBO: Index Buffer Object
  // GLCall(glGenBuffers(numberOfBuffer, &IBO_));
  // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_));
  // GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW));

  // Enable shader to init camera matrix
  GLCall(glUseProgram(shaderManager_.getShaderProgramID()));
  // Set default "look at" camera
  matrixHelper_.updateView(cameraPosition_ + basicCameraPositionOffset, cameraPosition_ + basicCameraTarget,
                           basicCameraUp);
  // Set perspective
  matrixHelper_.updateProjection(static_cast<float>(windowManager_.getWidth()),
                                 static_cast<float>(windowManager_.getHeight()));
  // Set matrix in shader
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

  GLCall(glClearColor(WorldManager::neonPinkR, WorldManager::neonPinkG, WorldManager::neonPinkB, 1.0F));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  GLCall(glUseProgram(shaderManager_.getShaderProgramID()));
  // Update view matrix for camera movement
  shaderManager_.setViewMatrix(matrixHelper_.view);
  // Update projection matrix for window size change & aspect ratio
  shaderManager_.setProjectionMatrix(matrixHelper_.projection);

  // Draw one tile from indices
  // GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

  // Draw one tile/rectangle
  GLCall(glDrawArrays(GL_TRIANGLES, 0, basicSquareVertices.size()));

  return *this;
}

/**
 * Overrides
 */
void WorldManager::onMoveForward() {
  const auto yAxis = glm::vec3(0.0F, 1.0F, 0.0F);
  cameraPosition_ = cameraPosition_ + yAxis * getCameraSpeedByFrame_();
  matrixHelper_.updateView(cameraPosition_ + basicCameraPositionOffset, cameraPosition_ + basicCameraTarget,
                           basicCameraUp);
}

void WorldManager::onMoveBackward() {
  const auto yAxis = glm::vec3(0.0F, 1.0F, 0.0F);
  cameraPosition_ = cameraPosition_ - yAxis * getCameraSpeedByFrame_();
  matrixHelper_.updateView(cameraPosition_ + basicCameraPositionOffset, cameraPosition_ + basicCameraTarget,
                           basicCameraUp);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void WorldManager::onResize(int width, int height) {
  const auto windowWidth = static_cast<float>(width);
  const auto windowHeight = static_cast<float>(height);
  matrixHelper_.updateProjection(windowWidth, windowHeight);
}
