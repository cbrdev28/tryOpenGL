#ifndef WORLD_MANAGER_H_
#define WORLD_MANAGER_H_

#include <fmt/core.h>
#include <fmt/format.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "InputManager.h"
#include "KeyboardListener.h"
#include "MatrixHelper.h"
#include "ShaderManager.h"
#include "WindowListener.h"
#include "WindowManager.h"
#include "openGLHeaders.h"


/**
 * World manager to define what to draw & how to interact.
 */
class WorldManager : public WindowListener, KeyboardListener {
 public:
  explicit WorldManager(WindowManager& windowManager, InputManager& inputManager);

  /**
   * The delta from the origin on the X axis for a tile.
   * Meaning, the width of a tile is this value mutliplied by 2.
   */
  static constexpr const float tileDeltaX = 0.5F;
  /**
   * The delta from the origin on the Y axis for a tile.
   * Meaning, the length of a tile is this value mutliplied by 2.
   */
  static constexpr const float tileDeltaY = 0.5F;
  /**
   * The height on the Z axis for a tile.
   * Meaning, the height of a tile is this value.
   * Because each of our tile will always start from 0 on Z axis.
   */
  static constexpr const float tileHeightZ = 0.1F;
  /**
   * The angle of each tiles to rotate them around the X axis.
   * The idea is to build a floor of tiles.
   * Used with the model matrix.
   */
  static constexpr const float tileAngle = glm::radians(-55.0F);
  /**
   * How far the camera will look from the Z axis.
   * Meaning, "how high" the camera will be.
   * Used with the view matrix.
   */
  static constexpr const float cameraZDelta = 2.0F;
  /**
   * How far the camera will look from the Y axis.
   * Meaning, "how behind" the camera will be.
   * Used with the view matrix.
   */
  static constexpr const float cameraYDelta = 4.0F;
  /**
   * Basically the width of a tile, based on its delta X.
   * Used to position each tile to build a floor.
   */
  static constexpr const float tileWidth = WorldManager::tileDeltaX * 2.0F;
  /**
   * Basically the length of a tile, based on its delta Y.
   * Used to position each tile to build a floor.
   */
  static constexpr const float tileLength = WorldManager::tileDeltaY * 2.0F;

  static constexpr const float neonPinkR = 255.0F / 255.0F;
  static constexpr const float neonPinkG = 68.0F / 255.0F;
  static constexpr const float neonPinkB = 205.0F / 255.0F;

  /**
   * Initialize world manager: shader...
   * @return WorldManager&
   * @throw -1
   */
  auto init() -> WorldManager&;

  /**
   * Render world
   * @return WorldManager&
   * @throw -1
   */
  auto render() -> WorldManager&;

  /**
   * Overrides
   */
  void onMoveForward() override;
  void onMoveBackward() override;
  void onResize(int width, int height) override;

 private:
  WindowManager& windowManager_;
  InputManager& inputManager_;
  ShaderManager shaderManager_{ShaderManager::vertexShaderSource, ShaderManager::fragmentShaderSource};
  MatrixHelper matrixHelper_;

  /**
   * Frame timestamps
   */
  // Keep track of last frame timestamp
  double lastTimeFrame_ = 0.0F;
  // Time between last frame & current frame
  double deltaTimeFrame_ = 0.0F;
  // Called during render
  auto updateDeltaTimeFrame_(double currentTimeFrame) -> WorldManager& {
    deltaTimeFrame_ = currentTimeFrame - lastTimeFrame_;
    lastTimeFrame_ = currentTimeFrame;
    return *this;
  }
  // Helper for camera speed
  const double defaultCameraSpeed = 1.0F;
  [[nodiscard]] auto getCameraSpeedByFrame_() const -> float {
    const double speedByFrame = defaultCameraSpeed * deltaTimeFrame_;
    return static_cast<float>(speedByFrame);
  }

  /**
   * Camera vectors
   */
  // The position of the camera itself is by default a bit away from the z axis.
  glm::vec3 cameraPosition_ = glm::vec3(0.0F, 0.0F, WorldManager::cameraZDelta);
  // By default, we look at things from higher (Z axis) and from behind (Y axis)
  // Each time we apply this offset when setting the camera position
  const glm::vec3 cameraPositionOffset_ = glm::vec3(0.0F, -WorldManager::cameraYDelta, WorldManager::cameraZDelta);
  // The position of where the camera looks at (its target), by default the origin-ish.
  glm::vec3 cameraTarget_ = glm::vec3(0.0F, 0.0F, -1.0F);
  // Define a vector which points up
  glm::vec3 up_ = glm::vec3(0.0F, 1.0F, 0.0F);

  // For now we only have one VBO & VAO, otherwise these should be arrays instead
  unsigned int VBO_{0};
  unsigned int VAO_{0};

  // Store position vectors
  std::vector<glm::vec3> positionVectors_{glm::vec3(0.0F, 0.0F, 0.0F)};

  // Our first helper function to build verticles for a tile
  static auto tileVerticles(float deltaX, float deltaY, float height) -> std::vector<float>;

  // Our first helper function to position tiles
  static auto tilePositions(unsigned int numberOfTiles, float width = WorldManager::tileWidth,
                            float length = WorldManager::tileLength) -> std::vector<glm::vec3>;
};

#endif
