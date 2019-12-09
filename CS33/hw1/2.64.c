#include <stdio.h>

/* Return 1 when any odd bit of x equals 1; 0 otherwise
   Assume w=32 */
int any_odd_one(unsigned x) {
  return !!(0x55555555 & x);
}

/*
int main(void)
{
  for(int i = 0; i < 64; i++)
    printf("%d  %d\n", i, any_odd_one(i));
}
*/
