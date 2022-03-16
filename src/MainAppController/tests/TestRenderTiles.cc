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
      currentCameraTileIdx_(this->findTileBaseIdxForPos(cameraPosX_, cameraPosY_, tileVertices_)) {
  // TODO(cbr): try to store in array?
  tileVertices_ = TileVertex::buildTilesGrid(kDefaultGridRowColumnCount);
  std::vector<unsigned int> allTileIndices = this->makeTilesIndices(tileVertices_.size());
  std::vector<float> serializedVertices = TileVertex::serialize(tileVertices_);

  va1_ = std::make_unique<VertexArray>();
  vb1_ = std::make_unique<VertexBuffer>(serializedVertices.data(), serializedVertices.size() * sizeof(float));

  VertexBufferLayout layout;
  layout.pushFloat(TileVertex::kPosCount);
  layout.pushFloat(TileVertex::kTextureCoordCount);
  layout.pushFloat(TileVertex::kTextureIdCount);
  va1_->addBuffer(*vb1_, layout);

  ib1_ = std::make_unique<IndexBuffer>(allTileIndices.data(), allTileIndices.size());

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

  va3_ = std::make_unique<VertexArray>();
  vb3_ = std::make_unique<VertexBuffer>(nullptr, sizeof(float) * TestRenderTiles::maxDynamicTriangleVertexValues,
                                        GL_DYNAMIC_DRAW);

  VertexBufferLayout layout3;
  layout3.pushFloat(2);  // Each vertex has 2 float values
  va3_->addBuffer(*vb3_, layout3);

  // Index buffer is pre-filled with max indices values
  // TODO(cbr): try to use array instead?
  dynamicTriangleIndices_.reserve(TestRenderTiles::maxDynamicTriangleIndiceValues);
  for (int i = 0; i < TestRenderTiles::maxDynamicTriangles; i++) {
    dynamicTriangleIndices_.emplace_back(i * 3 + 0);
    dynamicTriangleIndices_.emplace_back(i * 3 + 1);
    dynamicTriangleIndices_.emplace_back(i * 3 + 2);
  }

  ib3_ = std::make_unique<IndexBuffer>(dynamicTriangleIndices_.data(), dynamicTriangleIndices_.size());

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

TestRenderTiles::~TestRenderTiles() { this->getTestContext().inputManager->removeKeyboardListener(this); }

void TestRenderTiles::onUpdate(float deltaTime) { frameDeltaTime_ = deltaTime; }

void TestRenderTiles::onRender() {
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));
  renderer_.draw(*shader1_, *va1_, *ib1_);
  renderer_.draw(*shader2_, *va2_, *ib2_);
  renderer_.draw(*shader3_, *va3_, *ib3_);
}

void TestRenderTiles::onImGuiRender() {
  ImGui::Text("Window width: %d", this->getTestContext().windowManager->getWidth());
  ImGui::Text("Window height: %d", this->getTestContext().windowManager->getHeight());
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
  ImGui::Text("Dynamic triangles count: %zu", dynamicTriangles_.size());
  ImGui::Text("Dynamic indices count: %zu", dynamicTriangleIndices_.size());

  for (int i = 0; i < dynamicTriangles_.size() / 6; i++) {
    ImGui::Text("#: %.d", i);
    ImGui::Text("Triangle v1: %.2f, %.2f", dynamicTriangles_[i * 6 + 0], dynamicTriangles_[i * 6 + 1]);
    ImGui::Text("Triangle v2: %.2f, %.2f", dynamicTriangles_[i * 6 + 2], dynamicTriangles_[i * 6 + 2 + 1]);
    ImGui::Text("Triangle v3: %.2f, %.2f", dynamicTriangles_[i * 6 + 4], dynamicTriangles_[i * 6 + 4 + 1]);
    ImGui::Text("---- ---- ---- ----");
  }

  for (int i = 0; i < dynamicTriangleIndices_.size() / 3; i++) {
    ImGui::Text("#: %.d", i);
    ImGui::Text("Indice 1: %.d", dynamicTriangleIndices_[i * 3 + 0]);
    ImGui::Text("Indice 2: %.d", dynamicTriangleIndices_[i * 3 + 1]);
    ImGui::Text("Indice 3: %.d", dynamicTriangleIndices_[i * 3 + 2]);
    ImGui::Text("---- ---- ---- ----");
  }
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

  this->setViewProjection(usePerspective_, *shader3_);
  shader3_->unBind();
}

void TestRenderTiles::onMoveForward() {
  const auto nextCameraPosY = cameraPosY_ + (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  const auto nextPosTileIdx = this->findTileBaseIdxForPos(cameraPosX_, nextCameraPosY, tileVertices_);
  if (nextPosTileIdx == -1 || tileVertices_[nextPosTileIdx].textureIdx == 1.0F) {
    // Collision with "wall" tile (or out of grid)
  } else {
    currentCameraTileIdx_ = nextPosTileIdx;
    cameraPosY_ = nextCameraPosY;
    this->updateModelViewProjection();
  }
}

void TestRenderTiles::onMoveBackward() {
  const auto nextCameraPosY = cameraPosY_ - (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  const auto nextPosTileIdx = this->findTileBaseIdxForPos(cameraPosX_, nextCameraPosY, tileVertices_);
  if (nextPosTileIdx == -1 || tileVertices_[nextPosTileIdx].textureIdx == 1.0F) {
    // Collision with "wall" tile (or out of grid)
  } else {
    currentCameraTileIdx_ = nextPosTileIdx;
    cameraPosY_ = nextCameraPosY;
    this->updateModelViewProjection();
  }
}

void TestRenderTiles::onMoveLeft() {
  const auto nextCameraPosX = cameraPosX_ - (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  const auto nextPosTileIdx = this->findTileBaseIdxForPos(nextCameraPosX, cameraPosY_, tileVertices_);
  if (nextPosTileIdx == -1 || tileVertices_[nextPosTileIdx].textureIdx == 1.0F) {
    // Collision with "wall" tile (or out of grid)
  } else {
    currentCameraTileIdx_ = nextPosTileIdx;
    cameraPosX_ = nextCameraPosX;
    this->updateModelViewProjection();
  }
}

void TestRenderTiles::onMoveRight() {
  const auto nextCameraPosX = cameraPosX_ + (TestRenderTiles::defaultCameraSpeed * frameDeltaTime_);
  const auto nextPosTileIdx = this->findTileBaseIdxForPos(nextCameraPosX, cameraPosY_, tileVertices_);
  if (nextPosTileIdx == -1 || tileVertices_[nextPosTileIdx].textureIdx == 1.0F) {
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

auto TestRenderTiles::makeTilesIndices(unsigned int tileVerticesCount) -> std::vector<unsigned int> {
  const auto tilesCount = tileVerticesCount / TestRenderTiles::verticesPerTile;
  const auto indicesCount = tilesCount * TestRenderTiles::indicesPerTile;

  std::vector<unsigned int> allIndices = {};
  allIndices.reserve(indicesCount);

  for (int i = 0; i < tilesCount; i++) {
    const auto indiceIdx = i * TestRenderTiles::verticesPerTile;
    ASSERT(indiceIdx + 3 < indicesCount);
    allIndices.emplace_back(indiceIdx + 0);
    allIndices.emplace_back(indiceIdx + 1);
    allIndices.emplace_back(indiceIdx + 2);
    allIndices.emplace_back(indiceIdx + 2);
    allIndices.emplace_back(indiceIdx + 3);
    allIndices.emplace_back(indiceIdx + 0);
  }

  return allIndices;
}

auto TestRenderTiles::findTileBaseIdxForPos(float posX, float posY, const std::vector<TileVertex>& vertices) -> int {
  const unsigned int tilesCount = vertices.size() / TestRenderTiles::verticesPerTile;
  unsigned int maxCount = tilesCount;
  unsigned int startCount = 0;
  // We only try to optimize if there is a "large" amount of tiles
  const bool optimizeThreshold = tilesCount > 16;

  if (optimizeThreshold) {
    ASSERT(tilesCount == kDefaultGridSize);
    // We know/hope our grid is centered at coordinate 0/0
    // So if the position of the camera is negative on Y axis,
    // it means we "should" only search in the bottom rows of our tile grid
    if (posY < 0) {
      // Search only for half the count (plus the number for one more row/column)
      maxCount = (tilesCount / 2) + kDefaultGridRowColumnCount;
    } else if (posY >= 0) {
      // Skip first half (minus the number for one row/column)
      startCount = (tilesCount / 2) - kDefaultGridRowColumnCount;
    }
  }
  ASSERT(startCount >= 0);
  ASSERT(maxCount <= tilesCount);

  for (unsigned int i = startCount; i < maxCount; i++) {
    if (!optimizeThreshold) {
      continue;
    } else {
      const auto idxLine = i % kDefaultGridRowColumnCount;
      if (posX < 0.0F) {
        // Skip tiles at the end of lines
        // If we passed at least half of the line
        if (idxLine > kDefaultGridRowColumnCount / 2) {
          // Bump current "i" index to finish the line
          const auto offset = kDefaultGridRowColumnCount - idxLine - 1;
          i = i + offset;
        }
      } else if (posX >= 0.0F) {
        // Skip tiles from the beginning of lines
        // If we are still at least in the first half
        if (idxLine < kDefaultGridRowColumnCount / 2) {
          // Bump current "i" index to the middle of line
          const auto offset = (kDefaultGridRowColumnCount / 2) - 1;
          i = i + offset;
        }
      }
    }
    const auto vertex1Idx = i * TestRenderTiles::verticesPerTile + 0;
    const auto vertex3Idx = i * TestRenderTiles::verticesPerTile + 2;
    ASSERT(vertex1Idx < vertices.size());
    ASSERT(vertex3Idx < vertices.size());

    const auto& vertex1 = vertices[vertex1Idx];
    const auto& vertex3 = vertices[vertex3Idx];

    if (posX >= (vertex1.positions[0] - TileVertex::kTileSpacing) && posX <= vertex3.positions[0]) {
      if (posY >= (vertex1.positions[1] - TileVertex::kTileSpacing) && posY <= vertex3.positions[1]) {
        return static_cast<int>(vertex1Idx);
      }
    }
  }
  return -1;
}

auto TestRenderTiles::makeDynamicTriangle() -> std::vector<float> {
  const float gridRightEdgePos =
      (kDefaultGridRowColumnCount / 2.0F) * (TileVertex::kTileSize + TileVertex::kTileSpacing);
  const float gridLeftEdgePos = -gridRightEdgePos;
  const float gridTopPos = gridRightEdgePos;
  const float gridBottomPos = -gridTopPos;

  std::uniform_real_distribution<float> dist(0.0F, 1.0F);
  const float randomPosX = (dist(gen) * gridRightEdgePos * 2) - gridRightEdgePos;
  const float randomPosY = (dist(gen) * gridTopPos * 2) - gridTopPos;

  return std::vector<float>{
      // clang-format off
    randomPosX, randomPosY, // Bottom left: indice = 0
    randomPosX + 0.2F, randomPosY, // Bottom right: indice = 1
    randomPosX, randomPosY + 0.2F, // Top right: indice = 2
      // clang-format on
  };
}

void TestRenderTiles::addDynamicTriangle() {
  auto const tempTriangle = this->makeDynamicTriangle();
  dynamicTriangles_.insert(dynamicTriangles_.end(), tempTriangle.begin(), tempTriangle.end());

  ASSERT(dynamicTriangles_.size() < TestRenderTiles::maxDynamicTriangleVertexValues);

  // Make new indices
  // unsigned int currentSize = dynamicTriangleIndices_.size();
  // const auto tempIndice = std::vector<unsigned int>{currentSize, currentSize + 1, currentSize + 2};
  // dynamicTriangleIndices_.insert(dynamicTriangleIndices_.end(), tempIndice.begin(), tempIndice.end());

  // Potential refactor: not sending ALL data each time
  vb3_->bind();
  vb3_->setData(dynamicTriangles_.data(), sizeof(float) * dynamicTriangles_.size());
  vb3_->unBind();
}

}  // namespace test
