#ifndef TEST_MODEL_H_
#define TEST_MODEL_H_

#include "../helpers/openGLErrorHelpers.h"

struct TestModel {
  static constexpr unsigned int kTestCount = 1;
  std::array<glm::vec2, kTestCount> publicTestMember = {glm::vec2(0.0F, 0.0F)};
};

#endif