#include "image.h"

using namespace std;

int main(int argc, char *argv[])
{
  jdg::Image<unsigned char> img;
  
  img.load(argv[1]);
  
  img.show();

  img.pad(600, 400, jdg::NEAREST, 40, 80);

  img.show();

  return 0;
}
