/**
 * Helper to deal with: model, view & projection matrix
 */
#include "MatrixHelper.h"

auto MatrixHelper::defaultPerspectiveMatrix(float width, float height) -> glm::mat4 {
  const float aspect = width / height;
  return glm::perspective(MatrixHelper::defaultFOV, aspect, MatrixHelper::defaultNearPlane,
                          MatrixHelper::defaultFarPlane);
}

auto MatrixHelper::defaultLookAtMatrix(glm::vec3 pos, glm::vec3 target, glm::vec3 up) -> glm::mat4 {
  glm::mat4 lookAt = glm::lookAt(pos, target, up);
  return lookAt;
}