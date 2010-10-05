#include "image.h"

using namespace std;

int main(int argc, char *argv[])
{
  jdg::Image<unsigned char> img;
  jdg::Image<unsigned char> img2;
  img.load(argv[1]);
  img2.load(argv[2]);
  //img.show();

  img /= 2;
  img2 /= 2;

  (img + img2).show();

  img.show();
  img2.show();
  
  return 0;
}
