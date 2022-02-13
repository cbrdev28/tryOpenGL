#ifndef __WORLD_MANAGER_H__
#define __WORLD_MANAGER_H__

#include "ShaderManager.h"

/**
 * World manager to define what to draw & how to interact.
 */
class WorldManager {
 public:
  WorldManager();

  /**
   * Initialize world manager: shader...
   * @return WorldManager&
   * @throw -1
   */
  WorldManager& init();

  /**
   * Render world
   * @return WorldManager&
   * @throw -1
   */
  WorldManager& render();

 private:
  ShaderManager _shaderManager;
};

#endif
