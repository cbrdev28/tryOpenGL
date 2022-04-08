#ifndef SCENE_SELECT_CHARACTER_H_
#define SCENE_SELECT_CHARACTER_H_

#include "GameCharacter.h"
#include "Renderer.h"
#include "Test.h"

namespace test {

class SceneSelectCharacter : public Test {
 public:
  explicit SceneSelectCharacter(const TestContext& ctx) : Test(ctx) {}
  ~SceneSelectCharacter() override;

  SceneSelectCharacter(const SceneSelectCharacter& other) = delete;
  SceneSelectCharacter(SceneSelectCharacter&& other) = delete;
  auto operator=(const SceneSelectCharacter& other) -> SceneSelectCharacter& = delete;
  auto operator=(SceneSelectCharacter&& other) -> SceneSelectCharacter& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  Renderer renderer_;
  std::array<float, 4> backgroundColor_{0.0F, 0.0F, 0.0F, 1.0F};
  std::array<GameCharacter, 2> characters_{{{"Clara"}, {"Felix"}}};
};

}  // namespace test

#endif