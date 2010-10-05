#include "image.h"

using namespace std;

int main(int argc, char *argv[])
{
  jdg::Image<unsigned char> img;
  img.load(argv[1]);
  //img.show();

  img += 90;
  img -= (img / 2);
  img *= 2;

  if ( img == img )
    img.show();
  
  return 0;

}
