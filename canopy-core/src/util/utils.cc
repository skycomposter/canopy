#include "utils.h"

#include <algorithm>

double clamp(double val, double low, double high) {
  return std::max(low, std::min(high, val));
}
