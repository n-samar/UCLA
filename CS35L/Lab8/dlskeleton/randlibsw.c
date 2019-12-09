/* Software implementation.  */
#include <stdio.h>
#include <stdlib.h>
#include "randlib.h"

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
__attribute__((constructor))
extern void
software_rand64_init (void)
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
static unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

extern unsigned long long rand64 (void)
{
  return software_rand64();
}

/* Finalize the software rand64 implementation.  */
__attribute__((destructor))
extern void
software_rand64_fini (void)
{
  fclose (urandstream);
}
