/**
 * World manager to define what to draw & how to interact.
 */
#include "WorldManager.h"

#include <fmt/core.h>
#include <fmt/format.h>

WorldManager::WorldManager(WindowManager& windowManager, InputManager& inputManager)
    : windowManager_(windowManager), inputManager_(inputManager) {
  fmt::print("WorldManager::WorldManager(...)\n");
  const auto* formattedRef = fmt::ptr(&windowManager_);
  fmt::print("WorldManager::WorldManager(...): windowManager_ = {}\n", formattedRef);

  formattedRef = fmt::ptr(&inputManager_);
  fmt::print("WorldManager::WorldManager(...): inputManager_ = {}\n", formattedRef);
}

auto WorldManager::init() -> WorldManager& {
  fmt::print("WorldManager::init()\n");
  glEnable(GL_DEPTH_TEST);
  // Init shader manager to load, compile & link default shaders
  shaderManager_.init();

  // Init VAO & VBO
  const int numberOfVertexObject = 1;
  glGenVertexArrays(numberOfVertexObject, &VAO_);
  const int numberOfBuffer = 1;
  glGenBuffers(numberOfBuffer, &VBO_);
  glBindVertexArray(VAO_);

  // Setup verticles to be drawn
  const auto verticles =
      WorldManager::tileVerticles(WorldManager::tileDeltaX, WorldManager::tileDeltaY, WorldManager::tileHeightZ);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * verticles.size()), verticles.data(),
               GL_STATIC_DRAW);

  const GLuint vertexIndex = 0;
  const GLint vertexSize = 3;
  GLCall(glVertexAttribPointer(vertexIndex, vertexSize, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), nullptr));
  glEnableVertexAttribArray(vertexIndex);

  // Enable shader to init camera matrix
  glUseProgram(shaderManager_.getShaderProgramID());
  // Set default "look at" camera
  matrixHelper_.updateView(cameraPosition_ + cameraPositionOffset_, cameraPosition_ + cameraTarget_, up_);
  // Set perspective
  const auto windowWidth = static_cast<float>(windowManager_.getWidth());
  const auto windowHeight = static_cast<float>(windowManager_.getHeight());
  matrixHelper_.updateProjection(windowWidth, windowHeight);
  // Set matrix in shader
  shaderManager_.setModelMatrix(matrixHelper_.model)
      .setViewMatrix(matrixHelper_.view)
      .setProjectionMatrix(matrixHelper_.projection);

  // TODO(cbr): re-work when learning batch rendering
  // Preload/store vertices positions to re-use during render
  // const unsigned int numberOfTiles = 9;
  // positionVectors_ = WorldManager::tilePositions(numberOfTiles);

  // Subscribe as listener
  windowManager_.addWindowListener(this);
  inputManager_.addKeyboardListener(this);
  return *this;
}

// NOTE: this function is called during the render loop!
auto WorldManager::render() -> WorldManager& {
  // fmt::print("WorldManager::render()\n");
  updateDeltaTimeFrame_(glfwGetTime());

  glClearColor(WorldManager::neonPinkR, WorldManager::neonPinkG, WorldManager::neonPinkB, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderManager_.getShaderProgramID());
  // Update view matrix for camera movement
  shaderManager_.setViewMatrix(matrixHelper_.view);
  // Update projection matrix for window size change & aspect ratio
  shaderManager_.setProjectionMatrix(matrixHelper_.projection);

  // TODO(cbr): re-work when learning batch rendering
  // Loop over positions and update model matrix
  // for (auto aPosition : positionVectors_) {
  //   // Positioning objects using the model matrix
  //   glm::mat4 model = matrixHelper_.model;
  //   model = glm::translate(model, aPosition);
  //   shaderManager_.setModelMatrix(model);

  //   // Render
  //   glBindVertexArray(VAO_);
  //   // Draw static tile made of 5 panels (each panel is made of 2 triangles, so 6 coordinates)
  //   const GLint startingIndex = 0;
  //   const GLsizei numberOfPanel = 5;
  //   const GLsizei numberOfCoordinatesForPanel = 6;
  //   GLCall(glDrawArrays(GL_TRIANGLES, startingIndex, numberOfPanel * numberOfCoordinatesForPanel));
  // }

  // Render
  glBindVertexArray(VAO_);
  // Draw static tile made of 5 panels (each panel is made of 2 triangles, so 6 coordinates)
  const GLint startingIndex = 0;
  const GLsizei numberOfPanel = 5;
  const GLsizei numberOfCoordinatesForPanel = 6;
  GLCall(glDrawArrays(GL_TRIANGLES, startingIndex, numberOfPanel * numberOfCoordinatesForPanel));
  return *this;
}

/**
 * Overrides
 */
void WorldManager::onMoveForward() {
  const auto yAxis = glm::vec3(0.0F, 1.0F, 0.0F);
  cameraPosition_ = cameraPosition_ + yAxis * getCameraSpeedByFrame_();
  matrixHelper_.updateView(cameraPosition_ + cameraPositionOffset_, cameraPosition_ + cameraTarget_, up_);
}

void WorldManager::onMoveBackward() {
  const auto yAxis = glm::vec3(0.0F, 1.0F, 0.0F);
  cameraPosition_ = cameraPosition_ - yAxis * getCameraSpeedByFrame_();
  matrixHelper_.updateView(cameraPosition_ + cameraPositionOffset_, cameraPosition_ + cameraTarget_, up_);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void WorldManager::onResize(int width, int height) {
  const auto windowWidth = static_cast<float>(width);
  const auto windowHeight = static_cast<float>(height);
  matrixHelper_.updateProjection(windowWidth, windowHeight);
}

/**
 * A basic first function to return the verticles for a tile.
 * The z of the tile will always be based from 0, and will be the value of its height.
 */
auto WorldManager::tileVerticles(float deltaX, float deltaY, float height) -> std::vector<float> {
  const float floorLevel = 0.0F;

  std::vector<float> verticles = {
      // clang-format off

    // Top rectangle
    // first triangle
    deltaX, deltaY, height,   // top right
    deltaX, -deltaY, height,  // bottom right
    -deltaX, deltaY, height,  // top left
    // second triangle
    deltaX, -deltaY, height,   // bottom right
    -deltaX, -deltaY, height,  // bottom left
    -deltaX, deltaY, height,    // top left

    // Front rectangle
    -deltaX, -deltaY, height,
    -deltaX, -deltaY, floorLevel,
    deltaX, -deltaY, height,

    -deltaX, -deltaY, floorLevel,
    deltaX, -deltaY, floorLevel,
    deltaX, -deltaY, height,

    // Left side rectangle
    -deltaX, -deltaY, height,
    -deltaX, -deltaY, floorLevel,
    -deltaX, deltaY, height,

    -deltaX, -deltaY, floorLevel,
    -deltaX, deltaY, floorLevel,
    -deltaX, deltaY, height,

    // Right side rectangle
    deltaX, -deltaY, height,
    deltaX, -deltaY, floorLevel,
    deltaX, deltaY, height,

    deltaX, -deltaY, floorLevel,
    deltaX, deltaY, floorLevel,
    deltaX, deltaY, height,

    // Back rectangle
    deltaX, deltaY, height,
    deltaX, deltaY, floorLevel,
    -deltaX, deltaY, height,

    deltaX, deltaY, floorLevel,
    -deltaX, deltaY, floorLevel,
    -deltaX, deltaY, height,

    // No Bottom rectangle
      // clang-format on
  };
  return verticles;
}

// TODO(cbr): re-work when learning batch rendering
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
// auto WorldManager::tilePositions(unsigned int numberOfTiles, float width, float length) -> std::vector<glm::vec3> {
//   auto positions = std::vector<glm::vec3>{};
//   const auto mid = static_cast<int>(numberOfTiles / 2);
//   for (int i = 0; i < numberOfTiles; i++) {
//     for (int j = 0; j < numberOfTiles; j++) {
//       const auto posX = static_cast<float>(i - mid) * width;
//       const auto posY = static_cast<float>(j - mid) * length;
//       positions.emplace_back(glm::vec3(posX, posY, 0.0F));
//     }
//   }
//   return positions;
// }