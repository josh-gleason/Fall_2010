#include <complex>
#include "image.h"

using namespace std;

int main(int argc, char *argv[])
{
  jdg::Image<complex<float> > img(argv[1]);
  jdg::Image<double > img2(argv[1]);

  img = (img / 2) + (img2 / 2);

  jdg::Image<unsigned char> img3;

  img3 = img;

  img3.show();

  return 0;
}

