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
  tileVertices_ = this->makeTilesVertices(TestRenderTiles::gridRowColumnCount);
  std::vector<unsigned int> allTileIndices = this->makeTilesIndices(tileVertices_.size());
  std::vector<float> serializedVertices = TileVertex::serialize(tileVertices_);

  va1_ = std::make_unique<VertexArray>();
  vb1_ = std::make_unique<VertexBuffer>(serializedVertices.data(), serializedVertices.size() * sizeof(float));

  VertexBufferLayout layout;
  layout.pushFloat(TileVertex::posCount);
  layout.pushFloat(TileVertex::textureCoordCount);
  layout.pushFloat(1);  // TileVertex textureIdx
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
}

TestRenderTiles::~TestRenderTiles() = default;

void TestRenderTiles::onUpdate(float /*deltaTime*/) {
  const auto nextCameraPosX = deltaX_ * 0.1F;
  const auto nextCameraPosY = deltaY_ * 0.1F;
  const auto nextPosTileIdx = this->findTileBaseIdxForPos(nextCameraPosX, nextCameraPosY, tileVertices_);
  if (nextPosTileIdx == -1 || tileVertices_[nextPosTileIdx].textureIdx == 1.0F) {
    // Collision with "wall" tile (or out of grid)
  } else {
    cameraPosX_ = nextCameraPosX;
    cameraPosY_ = nextCameraPosY;
    currentCameraTileIdx_ = nextPosTileIdx;
  }

  this->setViewProjection(usePerspective_, *shader1_);
  shader1_->unBind();
  this->setViewProjection(usePerspective_, *shader2_);
  this->setModel(*shader2_);
  shader2_->unBind();
}

void TestRenderTiles::onRender() {
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));
  renderer_.draw(*shader1_, *va1_, *ib1_);
  renderer_.draw(*shader2_, *va2_, *ib2_);
}

void TestRenderTiles::onImGuiRender() {
  ImGui::Text("Window width: %d", this->getTestContext().windowManager->getWidth());
  ImGui::Text("Window height: %d", this->getTestContext().windowManager->getHeight());
  ImGui::Text("%s", aspectRatio_.formattedValue().c_str());
  ImGui::ColorEdit4("Color", backgroundColor_.data());
  ImGui::Checkbox("Use perspective", &usePerspective_);
  if (usePerspective_) {
    ImGui::SliderFloat("FOV", &fov_, 1.0F, 180.0F, "WIP value = %.2f");
  } else {
    ImGui::SliderFloat("Zoom", &zoom_, 1.0F, 100.0F, "WIP value = %.2f");
  }
  ImGui::SliderFloat("Delta X", &deltaX_, -100.0F, 100.0F, "WIP value = %.2f");
  ImGui::SliderFloat("Delta Y", &deltaY_, -100.0F, 100.0F, "WIP value = %.2f");
  ImGui::Text("Camera pos X: %.2f", cameraPosX_);
  ImGui::Text("Camera pos Y: %.2f", cameraPosY_);
  ImGui::Text("Base tile index: %d", currentCameraTileIdx_);
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

    const auto zoom = zoom_ * 0.1F;
    const auto orthoX = 1.0F * zoom;
    const auto orthoY = reversedAspectRatio_ * zoom;
    shader.setUniformMat4("u_projection", glm::ortho((-orthoX) + cameraPosX_, orthoX + cameraPosX_,
                                                     (-orthoY) + cameraPosY_, orthoY + cameraPosY_, -1.0F, 1.0F));
  }
}

void TestRenderTiles::setModel(ShaderManager& shader) {
  shader.bind();
  const auto model = glm::translate(MatrixHelper::identityMatrix, glm::vec3(cameraPosX_, cameraPosY_, 0.0F));
  shader.setUniformMat4("u_model", model);
}

auto TestRenderTiles::makeTilesVertices(unsigned int size) -> std::vector<TileVertex> {
  ASSERT(size < TestRenderTiles::maxGridSize);
  // Note: how to check for overflow?
  const auto totalVertices = size * size;

  std::vector<TileVertex> allVertices = {};
  allVertices.reserve(totalVertices);

  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      auto posX = static_cast<float>(j) * (TestRenderTiles::tileSize + TestRenderTiles::tileSpacing);
      posX = posX - (static_cast<float>(size) / 2.0F);  // To center the grid of tiles

      auto posY = static_cast<float>(i) * (TestRenderTiles::tileSize + TestRenderTiles::tileSpacing);
      posY = posY - (static_cast<float>(size) / 2.0F);  // To center the grid of tiles

      float tileTextureId = 0.0F;
      if (j == 0 || j == size - 1 || i == 0 || i == size - 1) {
        // Draw "wall" texture on the edge of the grid
        tileTextureId = 1.0F;
      }

      TileVertex vertex1 = {{posX, posY}, {0.0F, 0.0F}, tileTextureId};
      TileVertex vertex2 = {{posX + TestRenderTiles::tileSize, posY}, {1.0F, 0.0F}, tileTextureId};
      TileVertex vertex3 = {
          {posX + TestRenderTiles::tileSize, posY + TestRenderTiles::tileSize}, {1.0F, 1.0F}, tileTextureId};
      TileVertex vertex4 = {{posX, posY + TestRenderTiles::tileSize}, {0.0F, 1.0F}, tileTextureId};

      allVertices.emplace_back(vertex1);
      allVertices.emplace_back(vertex2);
      allVertices.emplace_back(vertex3);
      allVertices.emplace_back(vertex4);
    }
  }
  return allVertices;
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
    ASSERT(tilesCount == TestRenderTiles::gridSize);
    // We know/hope our grid is centered at coordinate 0/0
    // So if the position of the camera is negative on Y axis,
    // it means we "should" only search in the bottom rows of our tile grid
    if (posY < 0) {
      // Search only for half the count (plus the number for one more row/column)
      maxCount = (tilesCount / 2) + TestRenderTiles::gridRowColumnCount;
    } else if (posY >= 0) {
      // Skip first half (minus the number for one row/column)
      startCount = (tilesCount / 2) - TestRenderTiles::gridRowColumnCount;
    }
  }
  ASSERT(startCount >= 0);
  ASSERT(maxCount <= tilesCount);

  for (unsigned int i = startCount; i < maxCount; i++) {
    if (!optimizeThreshold) {
      continue;
    } else {
      const auto idxLine = i % TestRenderTiles::gridRowColumnCount;
      if (posX < 0.0F) {
        // Skip tiles at the end of lines
        // If we passed at least half of the line
        if (idxLine > TestRenderTiles::gridRowColumnCount / 2) {
          // Bump current "i" index to finish the line
          const auto offset = TestRenderTiles::gridRowColumnCount - idxLine - 1;
          i = i + offset;
        }
      } else if (posX >= 0.0F) {
        // Skip tiles from the beginning of lines
        // If we are still at least in the first half
        if (idxLine < TestRenderTiles::gridRowColumnCount / 2) {
          // Bump current "i" index to the middle of line
          const auto offset = (TestRenderTiles::gridRowColumnCount / 2) - 1;
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

    if (posX >= (vertex1.positions[0] - TestRenderTiles::tileSpacing) && posX <= vertex3.positions[0]) {
      if (posY >= (vertex1.positions[1] - TestRenderTiles::tileSpacing) && posY <= vertex3.positions[1]) {
        return static_cast<int>(vertex1Idx);
      }
    }
  }
  return -1;
}

}  // namespace test
