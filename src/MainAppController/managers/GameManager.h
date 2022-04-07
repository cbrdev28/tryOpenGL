#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "GameStatistics.h"

class GameManager {
 public:
  GameManager() = default;
  ~GameManager() = default;

  GameManager(const GameManager& other) = delete;
  GameManager(GameManager&& other) = delete;
  auto operator=(const GameManager& other) -> GameManager& = delete;
  auto operator=(GameManager&& other) -> GameManager& = delete;

  void startGame();
  void stopGame();
  [[nodiscard]] auto isGameRunning() const -> const bool&;
  [[nodiscard]] auto getTimePlayed() const -> float;
  [[nodiscard]] auto getGameTime() const -> float;

 private:
  // TODO(cbr): move into a GameState struct
  bool gameRunning_{false};
  GameStatistics gameStats_;
};

#endif