#ifndef MATRIX_HELPER_H_
#define MATRIX_HELPER_H_

#include <glmHeaders.h>

/**
 * Helper to deal with: model, view & projection matrix
 */
class MatrixHelper {
 public:
  MatrixHelper() = default;
  ~MatrixHelper() = default;

  MatrixHelper(const MatrixHelper& other) = delete;
  MatrixHelper(MatrixHelper&& other) = delete;
  auto operator=(const MatrixHelper& other) -> MatrixHelper& = delete;
  auto operator=(MatrixHelper&& other) -> MatrixHelper& = delete;

  // Constant(s)
  static constexpr const glm::mat4 identityMatrix{1.0F};
  static constexpr const float defaultFOV = glm::radians(45.0F);
  static constexpr const float defaultNearPlane = 0.1F;
  static constexpr const float defaultFarPlane = 100.0F;

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  glm::mat4 model{identityMatrix};

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  glm::mat4 view{identityMatrix};

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  glm::mat4 projection{identityMatrix};

  /**
   * Update the projection matrix using the default
   * perspective matrix builder.
   * @param width the width of the window
   * @param height the height of the window
   */
  inline auto updateProjection(float width, float height) -> MatrixHelper& {
    projection = defaultPerspectiveMatrix(width, height);
    return *this;
  }

  /**
   * Update the view matrix using the default "look at" matrix builder.
   * @param position the vector of the camera position
   * @param target the vector of the camera target
   * @param up the up vector
   */
  inline auto updateView(glm::vec3 position, glm::vec3 target, glm::vec3 up) -> MatrixHelper& {
    view = defaultLookAtMatrix(position, target, up);
    return *this;
  }

  static auto defaultPerspectiveMatrix(float width, float height) -> glm::mat4;
  static auto defaultLookAtMatrix(glm::vec3 pos, glm::vec3 target, glm::vec3 up) -> glm::mat4;
};

#endif