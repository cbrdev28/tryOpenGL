#include "TestDynamicRender.h"

#include <MatrixHelper.h>
#include <VertexBufferLayout.h>
#include <basicTriangle.h>
#include <imgui.h>
#include <openGLErrorHelpers.h>

#include <vector>

#include "DynamicTriangle.h"
#include "TileVertex.h"

namespace test {

TestDynamicRender::TestDynamicRender(const TestContext& ctx)
    : Test(ctx),
      aspectRatio_(ctx.windowManager->getAspectRatio()),
      reversedAspectRatio_(aspectRatio_.reversed()),
      gridVertices_(TileVertex::buildTilesGrid<kDefaultGridVerticesCount>(kDefaultGridRowColumnCount)),
      currentCameraTileIdx_(TileVertex::findTileBaseIdxForPos<kDefaultGridVerticesCount>(
          cameraPosX_, cameraPosY_, gridVertices_, kDefaultGridSize, kDefaultGridRowColumnCount)),
      dynamicIndices_(DynamicTriangle::makeIndices<kDefaultMaxDynamicTriangleIndices>()) {
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

  textureWall_ = std::make_unique<Texture>("wall.png");
  textureGrass_ = std::make_unique<Texture>("grass.png");
  textureWall_->bind(0);
  textureGrass_->bind(1);
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

  va3_ = std::make_unique<VertexArray>();
  vb3_ = std::make_unique<VertexBuffer>(nullptr, sizeof(float) * kDefaultMaxDynamicTriangleVertices, GL_DYNAMIC_DRAW);

  VertexBufferLayout layout3;
  layout3.pushFloat(2);  // Each vertex has 2 float values
  va3_->addBuffer(*vb3_, layout3);

  // Index buffer is pre-filled with max indices values
  ib3_ = std::make_unique<IndexBuffer>(dynamicIndices_.data(), dynamicIndices_.size());

  shader3_ = std::make_unique<ShaderManager>("basic.shader");
  shader3_->init();
  shader3_->bind();
  shader3_->setUniformMat4("u_model", MatrixHelper::identityMatrix);
  this->setViewProjection(usePerspective_, *shader3_);

  va3_->unBind();
  vb3_->unBind();
  ib3_->unBind();
  shader3_->unBind();

  ctx.inputManager->addKeyboardListener(this);
}

TestDynamicRender::~TestDynamicRender() { this->getTestContext().inputManager->removeKeyboardListener(this); }

void TestDynamicRender::onUpdate(float deltaTime) { frameDeltaTime_ = deltaTime; }

void TestDynamicRender::onRender() {
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));
  renderer_.draw(*shader1_, *va1_, *ib1_);
  renderer_.draw(*shader2_, *va2_, *ib2_);
  renderer_.draw(*shader3_, *va3_, *ib3_);
}

void TestDynamicRender::onImGuiRender() {
  ImGui::Text("Delta frame: %.3f", frameDeltaTime_);
  ImGui::Text("FPS: %.2f", 1.0F / frameDeltaTime_);
  ImGui::Text("%s", aspectRatio_.formattedValue().c_str());
  ImGui::ColorEdit4("Color", backgroundColor_.data());

  ImGui::Checkbox("Use perspective", &usePerspective_);
  ImGui::Text("Camera pos X: %.2f", cameraPosX_);
  ImGui::Text("Camera pos Y: %.2f", cameraPosY_);
  ImGui::Text("Base tile index: %d", currentCameraTileIdx_);

  if (ImGui::Button("Add dynamic triangle")) {
    this->addDynamicTriangle();
  }
  ImGui::Text("Dynamic triangles count: %zu", dynamicTriangleVertices_.size());
  ImGui::Text("Dynamic indices count: %zu", dynamicIndices_.size());

  // for (int i = 0; i < dynamicTriangles_.size() / 6; i++) {
  //   ImGui::Text("#: %.d", i);
  //   ImGui::Text("Triangle v1: %.2f, %.2f", dynamicTriangles_[i * 6 + 0], dynamicTriangles_[i * 6 + 1]);
  //   ImGui::Text("Triangle v2: %.2f, %.2f", dynamicTriangles_[i * 6 + 2], dynamicTriangles_[i * 6 + 2 + 1]);
  //   ImGui::Text("Triangle v3: %.2f, %.2f", dynamicTriangles_[i * 6 + 4], dynamicTriangles_[i * 6 + 4 + 1]);
  //   ImGui::Text("---- ---- ---- ----");
  // }

  // for (int i = 0; i < dynamicIndices_.size() / 3; i++) {
  //   ImGui::Text("#: %.d", i);
  //   ImGui::Text("Indice 1: %.d", dynamicIndices_.at(i * 3 + 0));
  //   ImGui::Text("Indice 2: %.d", dynamicIndices_.at(i * 3 + 1));
  //   ImGui::Text("Indice 3: %.d", dynamicIndices_.at(i * 3 + 2));
  //   ImGui::Text("---- ---- ---- ----");
  // }
}

void TestDynamicRender::setViewProjection(bool usePerspective, ShaderManager& shader) {
  shader.bind();

  if (usePerspective) {
    glm::vec3 pos = {cameraPosX_, cameraPosY_, 0.0F};
    shader.setUniformMat4("u_view", glm::lookAt(pos + TestDynamicRender::perspectiveLookAtPositionOffset,
                                                TestDynamicRender::perspectiveLookAtTarget + pos,
                                                TestDynamicRender::perspectiveLookAtUp));

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

void TestDynamicRender::setModel(ShaderManager& shader) {
  shader.bind();
  const auto model = glm::translate(MatrixHelper::identityMatrix, glm::vec3(cameraPosX_, cameraPosY_, 0.0F));
  shader.setUniformMat4("u_model", model);
}

void TestDynamicRender::updateModelViewProjection() {
  this->setViewProjection(usePerspective_, *shader1_);
  shader1_->unBind();

  this->setViewProjection(usePerspective_, *shader2_);
  this->setModel(*shader2_);
  shader2_->unBind();

  this->setViewProjection(usePerspective_, *shader3_);
  shader3_->unBind();
}

void TestDynamicRender::onMoveForward() {
  const auto nextCameraPosY = cameraPosY_ + (TestDynamicRender::defaultCameraSpeed * frameDeltaTime_);
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

void TestDynamicRender::onMoveBackward() {
  const auto nextCameraPosY = cameraPosY_ - (TestDynamicRender::defaultCameraSpeed * frameDeltaTime_);
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

void TestDynamicRender::onMoveLeft() {
  const auto nextCameraPosX = cameraPosX_ - (TestDynamicRender::defaultCameraSpeed * frameDeltaTime_);
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

void TestDynamicRender::onMoveRight() {
  const auto nextCameraPosX = cameraPosX_ + (TestDynamicRender::defaultCameraSpeed * frameDeltaTime_);
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

void TestDynamicRender::onZoomIn() {
  zoom_ = zoom_ + (TestDynamicRender::defaultCameraSpeed * frameDeltaTime_);
  fov_ = fov_ + (TestDynamicRender::defaultCameraSpeed * frameDeltaTime_);
  this->updateModelViewProjection();
}

void TestDynamicRender::onZoomOut() {
  zoom_ = zoom_ - (TestDynamicRender::defaultCameraSpeed * frameDeltaTime_);
  fov_ = fov_ - (TestDynamicRender::defaultCameraSpeed * frameDeltaTime_);
  this->updateModelViewProjection();
}

void TestDynamicRender::addDynamicTriangle() {
  auto const tempTriangle = DynamicTriangle::makeTriangleVertices(kDefaultGridRowColumnCount, TileVertex::kTileSize,
                                                                  TileVertex::kTileSpacing);
  dynamicTriangleVertices_.insert(dynamicTriangleVertices_.end(), tempTriangle.begin(), tempTriangle.end());
  ASSERT(dynamicTriangleVertices_.size() <= kDefaultMaxDynamicTriangleVertices);

  // Potential refactor: not sending ALL data each time
  vb3_->bind();
  vb3_->setData(dynamicTriangleVertices_.data(), sizeof(float) * dynamicTriangleVertices_.size());
  vb3_->unBind();
}

}  // namespace test
