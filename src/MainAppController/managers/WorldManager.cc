/**
 * World manager to define what to draw & how to interact.
 */

#include "WorldManager.h"

#include <fmt/core.h>

WorldManager& WorldManager::init() {
  fmt::print("WorldManager::init()\n");
  return *this;
}