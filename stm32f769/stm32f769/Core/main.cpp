#include "main.hpp"
#include "boot.hpp"

int
main(void)
{
  // System setup
  if (SystemInitialize())
  {
    while (1) {}
  }

  // Processing loop
  while (1) {}

  return 0;
}
