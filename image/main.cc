#include "funcs.h"

using namespace std;

int main(int argc, char *argv[])
{
  jdg::Image<float> img("boy.pgm");
  jdg::Image<float> img2("noise_30_deg.pgm");
  img = img;
  img2 -= 126;

  img += img2*10;

  img.normalize(jdg::MINMAX,0,255);

  img.show();

  return 0;
}

/*
enum FilterType{ IDEAL=0, GAUSSIAN=1, BUTTERWORTH=2 };

template <class pType>
void buildLowPass( jdg::Image<pType>& filter, FilterType type, float cutoff1,
  float cutoff2=0.0, bool freq_domain=false );

int main(int argc, char *argv[])
{
  jdg::Image<complex<float> > f("noise_30_deg.pgm");
  jdg::Image<complex<float> > h=f;
  jdg::Image<complex<float> > i=f;

  //buildLowPass( h, GAUSSIAN, 0.017, 0, true );
  //buildLowPass( h, BUTTERWORTH, 0.5, 0, true );
  //buildLowPass( h, IDEAL, 0.1, 0, true );
  //buildLowPass( i, IDEAL, 0.098, 0, true );
  buildLowPass( h, IDEAL, 0.09883, 1, true );
  buildLowPass( i, IDEAL, 0.09882, 1, true );

  // noisy pixels at 0.098821176880261 distance from center

  h-=i; // band pass

  // invert to make bandstop
  // h*=-1;
  // h+=1;

  jdg::Image<float> show = h;

  show.normalize( jdg::MINMAX, 0, 255 );
  show.show();

  jdg::fft(f);

  show = f;
  show.normalize( jdg::MINMAX_LOG, 0, 255 );
  show.show();
  f*=h;

  jdg::fft(f,-1);

  show = f;
  show.show();
  //show.save("noise.pgm");


  return 0;
}


template <class pType>
void buildLowPass( jdg::Image<pType>& filter, FilterType type, float param1,
  float param2, bool freq_domain )
{
  //filter.resizeCanvas(512,512);
  int width = filter.getWidth();
  int height = filter.getHeight();

  float startX = -width / 2.0,
      startY = -height / 2.0,
      stopX = -startX,
      stopY = -startY;

  param1 = param1 * 0.5*sqrt(width*width+height*height);

  float param1_sqr = param1*param1;

  for ( float y = startY; y < stopY; y+=1.0 )
    for ( float x = startX; x < stopX; x+=1.0 )
      if ( type==IDEAL )
      {
        if ( sqrt(x*x+y*y) > param1 )
          filter(x-startX,y-startY) = 0;
        else
          filter(x-startX,y-startY) = 1;
      }
      else if ( type==GAUSSIAN )
      {
        filter(x-startX,y-startY) = exp(-(0.5*x*x+0.5*y*y)/(param1_sqr));
      }
      else if ( type==BUTTERWORTH )
      {
        //cout << "(" << x-startX << ", " << y-startY << ") " << flush;
        filter(x-startX,y-startY) = 1.0/(1.0+pow((x*x+y*y)/param1_sqr,param2));
      }
      cout << endl;
  if ( !freq_domain )
    jdg::fft( filter, -1 );
}


/*
#define blah

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
  const int KERN = 31;
  jdg::Image<complex<float> > kern(KERN, KERN);

  for ( int i = 0; i < KERN; i++ )
    for ( int j = 0; j < KERN; j++ )
      kern(i,j)=1.0/(KERN*KERN);

  jdg::convolve(img,kern,jdg::NEAREST);

  jdg::Image<float> show = img;
  show.show();
  
  if ( argc > 2 )
    show.save(argv[2]);
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
*/
