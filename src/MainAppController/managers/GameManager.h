#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "GameCharacter.h"
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

  void setCurrentCharacter(const GameCharacter& gameCharacter);
  auto getCurrentCharacter() -> GameCharacter*;

 private:
  // TODO(cbr): move into a GameState struct
  bool gameRunning_{false};
  GameStatistics gameStats_;
  std::unique_ptr<GameCharacter> currentCharacter_{nullptr};
};

#endif