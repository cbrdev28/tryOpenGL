#ifndef WORLD_MANAGER_H_
#define WORLD_MANAGER_H_

// TODO(cbr): re-work when learning batch rendering
// #include <vector>

#include "InputManager.h"
#include "KeyboardListener.h"
#include "MatrixHelper.h"
#include "ShaderManager.h"
#include "WindowListener.h"
#include "WindowManager.h"
#include "glmHeaders.h"
#include "openGLHeaders.h"

/**
 * World manager to define what to draw & how to interact.
 */
class WorldManager : public WindowListener, KeyboardListener {
 public:
  explicit WorldManager(WindowManager& windowManager, InputManager& inputManager);

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

  unsigned int VBO_{0};
  // unsigned int IBO_{0};
};

#endif
