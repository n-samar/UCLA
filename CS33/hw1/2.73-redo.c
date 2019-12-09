#include <stdio.h>

int saturating_add(int x, int y) {
  int res;
  if(__builtin_add_overflow(x, y, &res)) {
    if (x < 0) return INT_MIN;
    return INT_MAX;
  }
  return res;
}
