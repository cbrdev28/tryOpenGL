#include "TestDynamicRender.h"

#include <imgui.h>

#include <vector>

#include "DynamicTriangle.h"
#include "MatrixHelper.h"
#include "TileVertex.h"
#include "VertexBufferLayout.h"
#include "basicFirstForms.h"

namespace test {

TestDynamicRender::TestDynamicRender(const TestContext& ctx)
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

  shader1_ = std::make_unique<ShaderManager>("test_dynamic_render.shader");
  shader1_->setUniformMat4("u_model", MatrixHelper::identityMatrix);
  this->setViewProjection(usePerspective_, *shader1_);

  textureWall_ = std::make_unique<Texture>("wall.png");
  textureGrass_ = std::make_unique<Texture>("grass.png");
  textureWall_->bind(0);
  textureGrass_->bind(1);
  // Set an array of samplers in our shader with values: 0, 1 (respectively matching the texture bind(...) function)
  shader1_->setUniform1i("u_textureSamplerA", 0);
  shader1_->setUniform1i("u_textureSamplerB", 1);

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
  shader2_->setUniformMat4("u_model", MatrixHelper::identityMatrix);
  this->setViewProjection(usePerspective_, *shader2_);

  va2_->unBind();
  vb2_->unBind();
  ib2_->unBind();
  shader2_->unBind();

  va3_ = std::make_unique<VertexArray>();
  // Dynamic usage of vertex buffer
  vb3_ = std::make_unique<VertexBuffer>(nullptr, sizeof(float) * kDefaultMaxDynamicTriangleVertices, GL_DYNAMIC_DRAW);
  // Send empty data to initialize it
  vb3_->setData(dynamicTriangleVertices_.data(),
                static_cast<GLsizeiptr>(sizeof(float) * dynamicTriangleVertices_.size()));

  VertexBufferLayout layout3;
  layout3.pushFloat(2);  // Each vertex has 2 float values
  va3_->addBuffer(*vb3_, layout3);

  // Dynamic usage of index buffer
  ib3_ = std::make_unique<IndexBuffer>(nullptr, kDefaultMaxDynamicTriangleIndices, GL_DYNAMIC_DRAW);
  // Send empty data to initialize it
  ib3_->setData(dynamicIndicesVector_.data(), static_cast<GLsizei>(dynamicIndicesVector_.size()));

  shader3_ = std::make_unique<ShaderManager>("basic.shader");
  shader3_->setUniformMat4("u_model", MatrixHelper::identityMatrix);
  this->setViewProjection(usePerspective_, *shader3_);

  va3_->unBind();
  vb3_->unBind();
  ib3_->unBind();
  shader3_->unBind();

  ctx.inputManager->addKeyboardListener(this);
}

TestDynamicRender::~TestDynamicRender() { this->getTestContext().inputManager->removeKeyboardListener(this); }

/**
 * onUpdate, onRender, onImGuiRender
 */
void TestDynamicRender::onUpdate(float deltaTime) {
  frameDeltaTime_ = deltaTime;
  this->onUpdateDynamicTriangles(deltaTime);
}

void TestDynamicRender::onRender() {
  renderer_.clearColorBackground(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]);
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
  ImGui::Text("Dynamic vertices count: %zu", dynamicTriangleVertices_.size());
  ImGui::Text("Dynamic indices vector count: %zu", dynamicIndicesVector_.size());
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

  unsigned int currentSize = dynamicIndicesVector_.size();
  const auto tempIndice = std::vector<unsigned int>{currentSize, currentSize + 1, currentSize + 2};
  dynamicIndicesVector_.insert(dynamicIndicesVector_.end(), tempIndice.begin(), tempIndice.end());

  // Potential refactor: not sending ALL data each time
  vb3_->bind();
  vb3_->setData(dynamicTriangleVertices_.data(),
                static_cast<GLsizeiptr>(sizeof(float) * dynamicTriangleVertices_.size()));
  vb3_->unBind();
  // Update indices in index buffer
  ib3_->bind();
  ib3_->setData(dynamicIndicesVector_.data(), static_cast<GLsizei>(dynamicIndicesVector_.size()));
  ib3_->unBind();
}

void TestDynamicRender::onUpdateDynamicTriangles(float deltaTime) { this->onMoveTriangleTowardCamera(deltaTime); }

void TestDynamicRender::onMoveTriangleTowardCamera(float deltaTime) {
  // For each 6 vertices (since a triangle is made of 3 vector of 2 positions)
  for (int i = 0; i < dynamicTriangleVertices_.size(); i += 6) {
    // Only once per loop, trying to rotate a triangle
    // if (i == 0) {
    //   const float triangleMidPosX = dynamicTriangleVertices_.at(0) + (DynamicTriangle::kTriangleSize / 2);
    //   const float triangleMidPosY = dynamicTriangleVertices_.at(1) + (DynamicTriangle::kTriangleSize / 2);
    //   auto const directionVec = glm::vec2(cameraPosX_ - triangleMidPosX, cameraPosY_ - triangleMidPosY);
    //   const auto angle = glm::atan(directionVec.y, directionVec.x);
    //   const auto modelMatrix = glm::rotate(MatrixHelper::identityMatrix, cbr, glm::vec3(0.0F, 0.0F, 1.0F));
    //   shader3_->bind();
    //   shader3_->setUniformMat4("u_model", modelMatrix);
    //   shader3_->unBind();
    // }

    const float triangleX = dynamicTriangleVertices_.at(i);
    const float triangleY = dynamicTriangleVertices_.at(i + 1);
    const float deltaPos = 0.2F * deltaTime;

    ASSERT(i + 5 < dynamicTriangleVertices_.size());

    if (triangleX < cameraPosX_) {
      dynamicTriangleVertices_.at(i + 0) += deltaPos;
      dynamicTriangleVertices_.at(i + 2) += deltaPos;
      dynamicTriangleVertices_.at(i + 4) += deltaPos;
    } else if (triangleX > cameraPosX_) {
      dynamicTriangleVertices_.at(i + 0) -= deltaPos;
      dynamicTriangleVertices_.at(i + 2) -= deltaPos;
      dynamicTriangleVertices_.at(i + 4) -= deltaPos;
    }
    if (triangleY < cameraPosY_) {
      dynamicTriangleVertices_.at(i + 1) += deltaPos;
      dynamicTriangleVertices_.at(i + 3) += deltaPos;
      dynamicTriangleVertices_.at(i + 5) += deltaPos;
    } else if (triangleY > cameraPosY_) {
      dynamicTriangleVertices_.at(i + 1) -= deltaPos;
      dynamicTriangleVertices_.at(i + 3) -= deltaPos;
      dynamicTriangleVertices_.at(i + 5) -= deltaPos;
    }
  }
  vb3_->bind();
  vb3_->setData(dynamicTriangleVertices_.data(),
                static_cast<GLsizeiptr>(sizeof(float) * dynamicTriangleVertices_.size()));
  vb3_->unBind();
}

}  // namespace test
