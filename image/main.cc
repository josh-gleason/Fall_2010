#include "funcs.h"

using namespace std;

int main(int argc, char *argv[])
{
  jdg::Image<complex<float> > img(argv[1]);

  jdg::fft( img );

  jdg::Image<float> img_new = img;

  float minval, maxval;

  img_new(0,0) = std::log(1 + img_new(0,0));
  minval = maxval = img_new(0,0);

  for ( int i = img_new.getWidth()-1; i>=0; --i )
    for ( int j = img_new.getHeight()-1; j>=0; --j )
    {
      if ( i != 0 && j != 0 )
      {
        img_new(i,j) = std::log(1 + img_new(i,j));
        if ( img_new(i,j) < minval )
          minval = img_new(i,j);
        if ( img_new(i,j) > maxval )
          maxval = img_new(i,j);
      }
    }
  
  cout << minval << endl
       << maxval << endl;

  maxval -= minval;
  for ( int i = img_new.getWidth()-1; i>=0; --i )
    for ( int j = img_new.getHeight()-1; j>=0; --j )
    {
      img_new(i,j) -= minval;

      img_new(i,j) *= 255.0/maxval;
    }

  jdg::Image<unsigned char> img_show = img_new;
  img_show.show();

  return 0;
}

