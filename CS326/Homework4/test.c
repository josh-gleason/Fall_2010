#include <stdio.h>

int func1(int retval) {
  printf("FUNCTION ONE\n");
  return retval;
}
int func2(int retval) {
  printf("FUNCTION TWO\n");
  return retval;
}
int And(int val1, int val2) {
  return func1(val1) ? func2(val2) : 0;
}

int main(int argc, char *argv[])
{
  printf("True True\n%i\n\n",And(1,1));
  printf("True False\n%i\n\n",And(1,0));
  printf("False True\n%i\n\n",And(0,1));
  printf("False False\n%i\n\n",And(0,0));
  return 0;
}

