#ifndef GAME_STATISTICS_H_
#define GAME_STATISTICS_H_

struct GameStatistics {
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
};

#endif  // GAME_STATISTICS_H_