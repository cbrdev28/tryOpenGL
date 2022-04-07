#ifndef ASPECT_RATIO_H_
#define ASPECT_RATIO_H_

#include <fmt/core.h>

struct AspectRatio {
  int numerator{1};
  int denominator{1};
  float ratio{static_cast<float>(numerator) / static_cast<float>(denominator)};
  auto formattedValue() -> std::string { return fmt::format("Ratio: {}/{}", numerator, denominator); }
};

#endif