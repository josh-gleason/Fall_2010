#include <stdio.h>

typedef union
{
  int ival;
  float fval;
} foo;

void main()
{
  foo obj;

  obj.ival = 1065353216;  // 1.0
  printf("%f\n",obj.fval);
  obj.ival = 1073741824;  // 2.0
  printf("%f\n",obj.fval);
}
