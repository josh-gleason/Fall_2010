#include "funcs.h"

using namespace std;

int main(int argc, char *argv[])
{
  jdg::Image<complex<float> > img(argv[1]);
  jdg::Image<float> orig = img;

  // create 7x7 averaging kernel
  jdg::Image<complex<float> > kern(7,7);

  for ( int i = 0; i < 7; i++ )
    for ( int j = 0; j < 7; j++ )
      kern(i,j)=1;

  jdg::convolve(img,kern);

  img.normalize( jdg::MINMAX, 0, 255 );

  jdg::Image<float> show = img;
  orig.show();
  show.show();

  return 0;
}

