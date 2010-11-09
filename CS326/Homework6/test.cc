#include <iostream>

void func(int a, int b=0, int c=0, int d=0);

int main()
{
  int b, c, d;
  b = 0;
  c = 0;
  d = 0;
  for ( long int i = 0; i < 5000000000L; i++ )
    func(1);
  return 0;
}

void func(int a, int b, int c, int d)
{
  int p = a + b + c + d;
  p *= 1;
}

