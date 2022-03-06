#ifndef BASIC_CAMERA_H_
#define BASIC_CAMERA_H_

#include "glmHeaders.h"

/**
 * How far the camera will look from the Z axis.
 * Meaning, "how high" the camera will be.
 * Used with the view matrix.
 */
const float basicCameraZDelta = 2.0F;
/**
 * How far the camera will look from the Y axis.
 * Meaning, "how behind" the camera will be.
 * Used with the view matrix.
 */
const float basicCameraYDelta = 2.0F;
/**
 * How fast the camera will move
 */
const double basicCameraSpeed = 1.0F;

// Pre-defined vector as helper when moving camera forward/backward
const auto basicCameraYAxis = glm::vec3(0.0F, 1.0F, 0.0F);

// By default, we look at things from higher (Z axis) and from behind (Y axis)
// Each time we apply this offset when setting the camera position
const glm::vec3 basicCameraPositionOffset = glm::vec3(0.0F, -basicCameraYDelta, basicCameraZDelta);
// The position of where the camera looks at (its target), by default the origin-ish.
const glm::vec3 basicCameraTarget = glm::vec3(0.0F, 0.0F, -1.0F);
// Define a vector which points up
const glm::vec3 basicCameraUp = glm::vec3(0.0F, 1.0F, 0.0F);

#endif