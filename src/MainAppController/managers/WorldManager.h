#ifndef WORLD_MANAGER_H_
#define WORLD_MANAGER_H_

#include <IndexBuffer.h>
#include <KeyboardListener.h>
#include <MatrixHelper.h>
#include <Renderer.h>
#include <ShaderManager.h>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <WindowListener.h>
#include <basicBackgroundColor.h>
#include <glmHeaders.h>

#include <memory>
#include <vector>

/**
 * World manager to define what to draw & how to interact.
 */
class WorldManager : public WindowListener, public KeyboardListener {
 public:
  /**
   * Initialize world manager: shader...
   * @return WorldManager&
   * @throw -1
   */
  auto init(const float windowWidth, const float windowHeight) -> WorldManager&;

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

  inline auto getBackGroundColor() -> std::vector<float>* { return &backgroundColor_; };

 private:
  Renderer renderer_;
  ShaderManager shaderManager_{"../res/shaders/basic.shader"};
  MatrixHelper matrixHelper_;

  std::unique_ptr<VertexBuffer> vbo_;
  std::unique_ptr<IndexBuffer> ibo_;
  std::unique_ptr<VertexArray> vao_;

  // Keep track of last frame timestamp
  double lastTimeFrame_ = 0.0F;
  // Time between last frame & current frame
  double deltaTimeFrame_ = 0.0F;
  // Called during render
  auto updateDeltaTimeFrame_(double currentTimeFrame) -> WorldManager&;
  [[nodiscard]] auto getCameraSpeedByFrame_() const -> float;

  // Keep track of camera position
  glm::vec3 cameraPosition_ = glm::vec3(0.0F, 0.0F, 0.0F);
  std::vector<float> backgroundColor_ = {basicBackgroundNeonPinkR, basicBackgroundNeonPinkG, basicBackgroundNeonPinkB,
                                         1.0F};
};

#endif
