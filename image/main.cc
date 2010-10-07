#include "funcs.h"

using namespace std;

//#define blah

template <class pType>
struct Point
{
  Point() {}
  Point(pType _val, int _x, int _y) :
    val(_val), x(_x), y(_y) {}
  pType val;
  int x;
  int y;
};

int main(int argc, char *argv[])
{
  jdg::Image<complex<float> > img(argv[1]);
#ifdef blah
  // create 7x7 averaging kernel
  jdg::Image<complex<float> > kern(7,7);

  for ( int i = 0; i < 7; i++ )
    for ( int j = 0; j < 7; j++ )
      kern(i,j)=1/49.0;

  jdg::convolve(img,kern);

  img.normalize( jdg::MINMAX_LOG, 0, 255 );

  jdg::Image<float> show = img;
  show.show();
#else
  jdg::fft(img,1);
  
  jdg::Image<float> show = img;
  show.normalize( jdg::MINMAX_LOG, 0, 255 );

// find top 4 values and remove //temporary because its ugly but it works

  Point<complex<float> >* top4 = new Point<complex<float> >[4];
  Point<complex<float> > temp, temp2;
  for ( int i = 0; i < 4; i++ )
  {
    temp.val = img(i,0);
    temp.x = i;
    temp.y = 0;
    top4[i] = temp;
  }

  Point<int> center;
  center.x = img.getHeight()/2;
  center.y = img.getHeight()/2;

  // minimum distance from center
  float mindist = max(img.getHeight(),img.getWidth())/20;

  #define DIST(p1,p2) sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))

  for ( int i = img.getHeight()-1; i>=0; --i )
    for ( int j = img.getWidth()-1; j>=0; --j )
    {
      temp.x = j;
      temp.y = i;
      if ( DIST(temp, center) > mindist )
      {
        temp.val = img(j,i);
        for ( int k = 0; k < 4; k++ )
        {
          if ( abs(top4[k].val) < abs(img(j,i)) )
          {
            temp2 = temp;
            temp = top4[k];
            top4[k] = temp2;
          }
        }
      }
    }

  for ( int i = 0; i < 4; i++ )
  {
    img(top4[i].x,top4[i].y) = 0;
    for ( int x = -1; x <= 1; x++ )
      for ( int y = -1; y <= 1; y++ )
        if (!( x == 0 && y == 0 ))
        {
          img(top4[i].x,top4[i].y)+=img(top4[i].x+x,top4[i].y+y);
        }
    img(top4[i].x,top4[i].y) /= 8.0;
  }

  delete [] top4;

  #undef DIST

  jdg::fft(img,-1);
  show = img;
  //show.normalize( jdg::MINMAX_LOG, 0, 255 );
 
  show.show();

  if ( argc > 2 )
    show.save(argv[2]);
#endif
  return 0;
}

