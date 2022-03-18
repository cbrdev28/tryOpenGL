#include "TestRenderTiles.h"

#include <MatrixHelper.h>
#include <VertexBufferLayout.h>
#include <basicTriangle.h>
#include <imgui.h>
#include <openGLErrorHelpers.h>

#include <vector>

namespace test {

TestRenderTiles::TestRenderTiles(const TestContext& ctx)
    : Test(ctx),
      aspectRatio_(ctx.windowManager->getAspectRatio()),
      reversedAspectRatio_(aspectRatio_.reversed()),
      gridVertices_(TileVertex::buildTilesGrid<kDefaultGridVerticesCount>(kDefaultGridRowColumnCount)),
      currentCameraTileIdx_(TileVertex::findTileBaseIdxForPos<kDefaultGridVerticesCount>(
          cameraPosX_, cameraPosY_, gridVertices_, kDefaultGridSize, kDefaultGridRowColumnCount)) {
  std::array<float, kDefaultGridVerticesFloatCount> serializedVertices =
      TileVertex::serialize<kDefaultGridVerticesCount>(gridVertices_);
  std::array<unsigned int, kDefaultGridIndicesCount> gridIndices =
      TileVertex::makeGridIndices<kDefaultGridIndicesCount>(gridVertices_.size());

  va1_ = std::make_unique<VertexArray>();
  vb1_ = std::make_unique<VertexBuffer>(serializedVertices.data(), serializedVertices.size() * sizeof(float));

  VertexBufferLayout layout;
  layout.pushFloat(TileVertex::kPosCount);
  layout.pushFloat(TileVertex::kTextureCoordCount);
  layout.pushFloat(TileVertex::kTextureIdCount);
  va1_->addBuffer(*vb1_, layout);

  ib1_ = std::make_unique<IndexBuffer>(gridIndices.data(), gridIndices.size());

  shader1_ = std::make_unique<ShaderManager>("test_render_tiles.shader");
  shader1_->init();
  shader1_->bind();
  shader1_->setUniformMat4("u_model", MatrixHelper::identityMatrix);
  this->setViewProjection(usePerspective_, *shader1_);

  textureGrass_ = std::make_unique<Texture>("grass.png");
  textureWall_ = std::make_unique<Texture>("wall.png");
  textureGrass_->bind(0);
  textureWall_->bind(1);
  // Set an array of samplers in our shader with values: 0, 1 (respectively matching the texture bind(...) function)
  shader1_->setUniform1iv("u_textureSamplers", {0, 1});

  va1_->unBind();
  vb1_->unBind();
  ib1_->unBind();
  shader1_->unBind();

  va2_ = std::make_unique<VertexArray>();
  vb2_ = std::make_unique<VertexBuffer>(basicTriangleIndicedVertices.data(),
                                        basicTriangleVerticesSizeOf * basicTriangleIndicedVertices.size());

  VertexBufferLayout layout2;
  layout2.pushFloat(basicTriangleVertexSize);
  va2_->addBuffer(*vb2_, layout2);

  ib2_ = std::make_unique<IndexBuffer>(basicTriangleIndices.data(), basicTriangleIndices.size());

  shader2_ = std::make_unique<ShaderManager>("basic.shader");
  shader2_->init();
  shader2_->bind();
  shader2_->setUniformMat4("u_model", MatrixHelper::identityMatrix);
  this->setViewProjection(usePerspective_, *shader2_);

  va2_->unBind();
  vb2_->unBind();
  ib2_->unBind();
  shader2_->unBind();

  ctx.inputManager->addKeyboardListener(this);
}

TestRenderTiles::~TestRenderTiles() { this->getTestContext().inputManager->removeKeyboardListener(this); }

void TestRenderTiles::onUpdate(float deltaTime) { frameDeltaTime_ = deltaTime; }

void TestRenderTiles::onRender() {
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));
  renderer_.draw(*shader1_, *va1_, *ib1_);
  renderer_.draw(*shader2_, *va2_, *ib2_);
}

void TestRenderTiles::onImGuiRender() {
  ImGui::Text("Delta frame: %.3f", frameDeltaTime_);
  ImGui::Text("FPS: %.2f", 1.0F / frameDeltaTime_);
  ImGui::ColorEdit4("Color", backgroundColor_.data());

  ImGui::Checkbox("Use perspective", &usePerspective_);
  ImGui::Text("Camera pos X: %.2f", cameraPosX_);
  ImGui::Text("Camera pos Y: %.2f", cameraPosY_);
  ImGui::Text("Camera current tile base index: %d", currentCameraTileIdx_);
}

void TestRenderTiles::setViewProjection(bool usePerspective, ShaderManager& shader) {
  shader.bind();

  if (usePerspective) {
    glm::vec3 pos = {cameraPosX_, cameraPosY_, 0.0F};
    shader.setUniformMat4(
        "u_view", glm::lookAt(pos + TestRenderTiles::perspectiveLookAtPositionOffset,
                              TestRenderTiles::perspectiveLookAtTarget + pos, TestRenderTiles::perspectiveLookAtUp));

    const auto perspective = glm::perspective(glm::radians(fov_), reversedAspectRatio_, 0.0F, 10.0F);
    shader.setUniformMat4("u_projection", perspective);
  } else {
    shader.setUniformMat4("u_view", MatrixHelper::identityMatrix);

    const auto orthoX = 1.0F * zoom_;
    const auto orthoY = reversedAspectRatio_ * zoom_;
    shader.setUniformMat4("u_projection", glm::ortho((-orthoX) + cameraPosX_, orthoX + cameraPosX_,
                                                     (-orthoY) + cameraPosY_, orthoY + cameraPosY_, -1.0F, 1.0F));
  }
}

void TestRenderTiles::setModel(ShaderManager& shader) {
  shader.bind();
  const auto model = glm::translate(MatrixHelper::identityMatrix, glm::vec3(cameraPosX_, cameraPosY_, 0.0F));
  shader.setUniformMat4("u_model", model);
}

void TestRenderTiles::updateModelViewProjection() {
  this->setViewProjection(usePerspective_, *shader1_);
  shader1_->unBind();

  this->setViewProjection(usePerspective_, *shader2_);
  this->setModel(*shader2_);
  shader2_->unBind();
}

void TestRenderTiles::onMoveForward() {
  const auto nextCameraPosY = cameraPosY_ + (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  const auto nextPosTileIdx = TileVertex::findTileBaseIdxForPos<kDefaultGridVerticesCount>(
      cameraPosX_, nextCameraPosY, gridVertices_, kDefaultGridSize, kDefaultGridRowColumnCount);

  if (nextPosTileIdx == -1 || gridVertices_.at(nextPosTileIdx).textureIdx == 1.0F) {
    // Collision with "wall" tile (or out of grid)
  } else {
    currentCameraTileIdx_ = nextPosTileIdx;
    cameraPosY_ = nextCameraPosY;
    this->updateModelViewProjection();
  }
}

void TestRenderTiles::onMoveBackward() {
  const auto nextCameraPosY = cameraPosY_ - (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  const auto nextPosTileIdx = TileVertex::findTileBaseIdxForPos<kDefaultGridVerticesCount>(
      cameraPosX_, nextCameraPosY, gridVertices_, kDefaultGridSize, kDefaultGridRowColumnCount);

  if (nextPosTileIdx == -1 || gridVertices_.at(nextPosTileIdx).textureIdx == 1.0F) {
    // Collision with "wall" tile (or out of grid)
  } else {
    currentCameraTileIdx_ = nextPosTileIdx;
    cameraPosY_ = nextCameraPosY;
    this->updateModelViewProjection();
  }
}

void TestRenderTiles::onMoveLeft() {
  const auto nextCameraPosX = cameraPosX_ - (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  const auto nextPosTileIdx = TileVertex::findTileBaseIdxForPos<kDefaultGridVerticesCount>(
      nextCameraPosX, cameraPosY_, gridVertices_, kDefaultGridSize, kDefaultGridRowColumnCount);

  if (nextPosTileIdx == -1 || gridVertices_.at(nextPosTileIdx).textureIdx == 1.0F) {
    // Collision with "wall" tile (or out of grid)
  } else {
    currentCameraTileIdx_ = nextPosTileIdx;
    cameraPosX_ = nextCameraPosX;
    this->updateModelViewProjection();
  }
}

void TestRenderTiles::onMoveRight() {
  const auto nextCameraPosX = cameraPosX_ + (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  const auto nextPosTileIdx = TileVertex::findTileBaseIdxForPos<kDefaultGridVerticesCount>(
      nextCameraPosX, cameraPosY_, gridVertices_, kDefaultGridSize, kDefaultGridRowColumnCount);

  if (nextPosTileIdx == -1 || gridVertices_.at(nextPosTileIdx).textureIdx == 1.0F) {
    // Collision with "wall" tile (or out of grid)
  } else {
    currentCameraTileIdx_ = nextPosTileIdx;
    cameraPosX_ = nextCameraPosX;
    this->updateModelViewProjection();
  }
}

void TestRenderTiles::onZoomIn() {
  zoom_ = zoom_ + (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  fov_ = fov_ + (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  this->updateModelViewProjection();
}

void TestRenderTiles::onZoomOut() {
  zoom_ = zoom_ - (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  fov_ = fov_ - (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  this->updateModelViewProjection();
}

}  // namespace test
