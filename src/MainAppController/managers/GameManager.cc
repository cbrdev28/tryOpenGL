#include "cbrpch.h"

#include "GameManager.h"

void GameManager::startGame() {
  gameRunning_ = true;
  // Update stats
}

void GameManager::stopGame() {
  gameRunning_ = false;
  // Update stats
}

auto GameManager::isGameRunning() const -> const bool& { return gameRunning_; }