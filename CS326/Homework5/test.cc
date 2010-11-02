#include <iostream>
#include <cstdlib>

using namespace std;

union foo
{
  float f;
  int i;
};

int main(int argc, char* argv[])
{
  foo val;

  val.i = atoi(argv[1]);

  int filter = 0x40000000;

  if ( val.i < 0 )
    cout << 1;
  else 
    cout << 0;

  for ( int i = 0; i < 31; i++ )
  {
    if ( (filter & val.i) == 0 )
      cout << 0;
    else
      cout << 1;

    if ( (i+2) % 4 == 0 )
      cout << ' ';

    filter = (filter >> 1);
  }

  
  cout << endl;
  cout << val.i << endl;
  cout << val.f << endl;
}
