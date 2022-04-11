#ifndef SCENE_TRAINING_H_
#define SCENE_TRAINING_H_

#include "GameCharacter.h"
#include "GameManager.h"
#include "Test.h"

namespace test {

class SceneTraining : public Test {
 public:
  explicit SceneTraining(const TestContext& ctx);
  ~SceneTraining() override;
  SceneTraining(const SceneTraining& other) = delete;
  SceneTraining(SceneTraining&& other) = delete;
  auto operator=(const SceneTraining& other) -> SceneTraining& = delete;
  auto operator=(SceneTraining&& other) -> SceneTraining& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  GameManager& gameManager_;
  GameCharacter* currentCharacter_;
};

}  // namespace test

#endif