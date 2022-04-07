#ifndef GAME_STATISTICS_H_
#define GAME_STATISTICS_H_

struct GameStatistics {
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();

  void resetStartEndTime() {
    startTime = std::chrono::steady_clock::now();
    endTime = std::chrono::steady_clock::now();
  }

  void setEndTimeNow() { endTime = std::chrono::steady_clock::now(); }

  [[nodiscard]] auto getNowTime() const -> std::chrono::steady_clock::time_point {
    return std::chrono::steady_clock::now();
  }

  [[nodiscard]] auto getCurrentDeltaTime() const -> float {
    return std::chrono::duration_cast<std::chrono::duration<float>>(getNowTime() - startTime).count();
  }

  [[nodiscard]] auto getLatestDeltaTime() const -> float {
    return std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count();
  }
};

#endif  // GAME_STATISTICS_H_