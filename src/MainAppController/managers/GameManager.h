#ifndef IM_GUI_MANAGER_H_
#define IM_GUI_MANAGER_H_

class GameManager {
 public:
  GameManager() = default;
  ~GameManager() = default;

  GameManager(const GameManager& other) = delete;
  GameManager(GameManager&& other) = delete;
  auto operator=(const GameManager& other) -> GameManager& = delete;
  auto operator=(GameManager&& other) -> GameManager& = delete;

 private:
};

#endif