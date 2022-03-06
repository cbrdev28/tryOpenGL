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
#include "basicCamera.h"
#include "glmHeaders.h"
#include "openGLHeaders.h"

/**
 * World manager to define what to draw & how to interact.
 */
class WorldManager : public WindowListener, KeyboardListener {
 public:
  explicit WorldManager(WindowManager& windowManager, InputManager& inputManager);

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

  // The position of the camera itself is by default a bit away from the z axis.
  glm::vec3 cameraPosition_ = glm::vec3(0.0F, 0.0F, basicCameraZDelta);

  unsigned int VBO_{0};
  unsigned int IBO_{0};
};

#endif
