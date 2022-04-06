#ifndef WINDOW_STATS_H_
#define WINDOW_STATS_H_

struct WindowStats {
  unsigned int frameCount = 0;
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
  std::chrono::duration<float> frameDeltaTime{0};
};

#endif