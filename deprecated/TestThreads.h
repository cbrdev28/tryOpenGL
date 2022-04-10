#ifndef TEST_THREADS_H_
#define TEST_THREADS_H_

#include <array>
#include <memory>
#include <thread>

#include "IndexBuffer.h"
#include "InstancedTriangle.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glmHeaders.h"

namespace test {

struct ThreadTransformationCtx {
  std::vector<glm::mat4>& transformations;
  unsigned int startIndex = 0;
  unsigned int endIndex = 0;
  float dtTime = 0.0F;
  std::string& status;
};

class TestThreads : public Test {
 public:
  explicit TestThreads(const TestContext& ctx);
  ~TestThreads() override;

  TestThreads(const TestThreads& other) = delete;
  TestThreads(TestThreads&& other) = delete;
  auto operator=(const TestThreads& other) -> TestThreads& = delete;
  auto operator=(TestThreads&& other) -> TestThreads& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  std::array<float, 4> backgroundColor_ = {0.8F, 0.8F, 0.8F, 1.0F};
  float deltaTime_{0.0F};

  Renderer renderer_;
  std::unique_ptr<VertexArray> va_;
  std::unique_ptr<VertexBuffer> vbModelVertex0_;
  std::unique_ptr<VertexBuffer> vbModelTransformation2_;
  std::unique_ptr<ShaderManager> shader_;

  InstancedTriangle instancedTriangle_;
  void addTriangleInstance();

  bool useThreads_{false};
  std::string onUpdateState_{"None"};
  std::unique_ptr<std::thread> thread1_{nullptr};
  std::unique_ptr<std::thread> thread2_{nullptr};
  std::string threadState1_{"Idle"};
  std::string threadState2_{"Idle"};
  void onUpdateThreads(float dtTime);

  inline static void threadedTransformation(ThreadTransformationCtx context) {
    ASSERT(context.startIndex >= 0);
    ASSERT(context.endIndex < context.transformations.size());
    context.status = "Working";
    for (unsigned int i = context.startIndex; i < context.endIndex; i++) {
      auto& transformation = context.transformations.at(i);
      transformation = glm::rotate(transformation, glm::radians(context.dtTime * 45.0F), glm::vec3(0.0F, 0.0F, 1.0F));
    }
    context.status = "Ending";
  }
};

}  // namespace test

#endif