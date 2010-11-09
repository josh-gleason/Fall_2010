#include <stdio.h>

void p()
{
  int y;
  printf("%d ", y);
  y = 2;
}

void q()
{
  float z = 1.0;
}

void main()
{
  p();
  q();
  p();
}

