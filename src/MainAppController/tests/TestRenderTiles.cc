#include "TestRenderTiles.h"

#include <VertexBufferLayout.h>
#include <glmHeaders.h>
#include <imgui.h>
#include <openGLErrorHelpers.h>

#include <vector>

namespace test {

TestRenderTiles::TestRenderTiles(const TestContext& ctx) : Test(ctx) {
  tileVertices_ = this->makeTileVertices(4);
  std::vector<unsigned int> allTileIndices = this->makeTileIndices(tileVertices_);
  std::vector<float> serializedVertices = TileVertex::serialize(tileVertices_);

  va_ = std::make_unique<VertexArray>();
  vb_ = std::make_unique<VertexBuffer>(serializedVertices.data(), serializedVertices.size() * sizeof(float));

  VertexBufferLayout layout;
  layout.pushFloat(TileVertex::posCount);
  layout.pushFloat(TileVertex::textureCoordCount);
  layout.pushFloat(1);  // TileVertex textureIdx
  va_->addBuffer(*vb_, layout);

  ib_ = std::make_unique<IndexBuffer>(allTileIndices.data(), allTileIndices.size());

  shader_ = std::make_unique<ShaderManager>("../res/shaders/test_texture.shader");
  shader_->init();
  shader_->bind();
  glm::mat4 identityMatrix = glm::mat4{1.0F};
  shader_->setUniformMat4("u_model", identityMatrix);
  this->setViewProjection(usePerspective_);

  textureGrass_ = std::make_unique<Texture>("../res/textures/grass.png");
  textureWall_ = std::make_unique<Texture>("../res/textures/wall.png");
  textureGrass_->bind(0);
  textureWall_->bind(1);
  // Set an array of samplers in our shader with values: 0, 1 (respectively matching the texture bind(...) function)
  shader_->setUniform1iv("u_textureSamplers", {0, 1});

  va_->unBind();
  vb_->unBind();
  ib_->unBind();
  shader_->unBind();
}

TestRenderTiles::~TestRenderTiles() = default;

void TestRenderTiles::onUpdate(float deltaTime) {}

void TestRenderTiles::onRender() {
  this->setViewProjection(usePerspective_);
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));
  renderer_.draw(*shader_, *va_, *ib_);
}

void TestRenderTiles::onImGuiRender() {
  ImGui::Text("Window width: %d", this->getTestContext().windowManager.getWidth());
  ImGui::Text("Window height: %d", this->getTestContext().windowManager.getHeight());
  ImGui::Text("%s", this->getTestContext().windowManager.getAspectRatio().formattedValue().c_str());
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
  ImGui::Text("Base tile index: %d", this->findTileBaseIdxForPos(cameraPosX_, cameraPosY_, tileVertices_));
}

void TestRenderTiles::setViewProjection(bool usePerspective) {
  shader_->bind();
  if (usePerspective) {
    const auto deltaX = deltaX_ * 0.1F;
    const auto deltaY = deltaY_ * 0.1F;
    cameraPosX_ = deltaX;
    cameraPosY_ = deltaY;
    glm::vec3 pos = {cameraPosX_, cameraPosY_, 0.0F};
    glm::vec3 target = {0.0F, 0.0F, -1.0F};
    glm::vec3 up = {0.0F, 1.0F, 0.0F};
    // Move camera: up on Z axis & back on the Y axis (to look from above and a bit behind)
    glm::vec3 posOffset = {0.0F, -1.0F, 1.0F};
    shader_->setUniformMat4("u_view", glm::lookAt(pos + posOffset, target + pos, up));

    const auto perspective = glm::perspective(
        glm::radians(fov_), 1.0F / this->getTestContext().windowManager.getAspectRatio().ratio, 0.0F, 10.0F);
    shader_->setUniformMat4("u_projection", perspective);
  } else {
    shader_->setUniformMat4("u_view", glm::mat4{1.0F});

    const auto reversedAspectRatio = 1.0F / this->getTestContext().windowManager.getAspectRatio().ratio;
    const auto zoom = zoom_ * 0.1F;
    const auto deltaX = deltaX_ * 0.1F;
    const auto deltaY = deltaY_ * 0.1F;
    cameraPosX_ = deltaX;
    cameraPosY_ = deltaY;
    const auto orthoX = 1.0F * zoom;
    const auto orthoY = reversedAspectRatio * zoom;
    shader_->setUniformMat4("u_projection", glm::ortho((-orthoX) + cameraPosX_, orthoX + cameraPosX_,
                                                       (-orthoY) + cameraPosY_, orthoY + cameraPosY_, -1.0F, 1.0F));
  }
}

auto TestRenderTiles::makeTileVertices(unsigned int count) -> std::vector<TileVertex> {
  ASSERT(count < 256);
  const float tileSize = 1.0F;
  const float tileSpacing = 0.1F;
  std::vector<TileVertex> allVertices = {};

  for (unsigned int i = 0; i < count; i++) {
    for (unsigned int j = 0; j < count; j++) {
      auto posX = static_cast<float>(j) * (tileSize + tileSpacing);
      posX = posX - (static_cast<float>(count) / 2.0F);
      auto posY = static_cast<float>(i) * (tileSize + tileSpacing);
      posY = posY - (static_cast<float>(count) / 2.0F);
      TileVertex vertex1 = {{posX, posY}, {0.0F, 0.0F}, 0.0F};
      TileVertex vertex2 = {{posX + tileSize, posY}, {1.0F, 0.0F}, 0.0F};
      TileVertex vertex3 = {{posX + tileSize, posY + tileSize}, {1.0F, 1.0F}, 0.0F};
      TileVertex vertex4 = {{posX, posY + tileSize}, {0.0F, 1.0F}, 0.0F};

      allVertices.emplace_back(vertex1);
      allVertices.emplace_back(vertex2);
      allVertices.emplace_back(vertex3);
      allVertices.emplace_back(vertex4);
    }
  }
  return allVertices;
}

auto TestRenderTiles::makeTileIndices(const std::vector<TileVertex>& tileVertices) -> std::vector<unsigned int> {
  const auto numOfVerticesPerTile = 4;
  const auto numOfTiles = tileVertices.size() / numOfVerticesPerTile;
  const auto numOfIndices = numOfTiles * 6;
  std::vector<unsigned int> allIndices = {};
  allIndices.reserve(numOfIndices);

  for (int i = 0; i < numOfTiles; i++) {
    const auto indiceIdx = i * numOfVerticesPerTile;
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
  const auto numOfVertexForTile = 4;
  const auto numOfTile = vertices.size() / numOfVertexForTile;

  for (int i = 0; i < numOfTile; i++) {
    const auto vertex1Idx = i * numOfVertexForTile + 0;
    const auto vertex3Idx = i * numOfVertexForTile + 2;
    ASSERT(vertex1Idx < vertices.size());
    ASSERT(vertex3Idx < vertices.size());

    const auto& vertex1 = vertices[vertex1Idx];
    const auto& vertex3 = vertices[vertex3Idx];

    if (posX >= vertex1.positions[0] && posX <= vertex3.positions[0]) {
      if (posY >= vertex1.positions[1] && posY <= vertex3.positions[1]) {
        return vertex1Idx;
      }
    }
  }
  return -1;
}

}  // namespace test
