#include "cbrpch.h"

#include "GameManager.h"

void GameManager::startGame() {
  gameStats_.resetStartEndTime();
  gameRunning_ = true;
}

void GameManager::stopGame() {
  gameStats_.setEndTimeNow();
  gameRunning_ = false;
}

auto GameManager::isGameRunning() const -> const bool& { return gameRunning_; }

auto GameManager::getTimePlayed() const -> float { return gameStats_.getLatestDeltaTime(); }

auto GameManager::getGameTime() const -> float { return gameRunning_ ? gameStats_.getCurrentDeltaTime() : 0.0F; }