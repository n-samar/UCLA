#include <limits.h>
/* Addition that saturates to TMin or TMax */
int saturating_add(int x, int  y) {
  // Over- or under-flow
  if ((x > 0)^(y + x > y)) {
    if (x < 0) return INT_MIN;
    return INT_MAX;
  }
  return x + y;
}

int main(void) {
  
}
