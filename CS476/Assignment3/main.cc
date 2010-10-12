#include "funcs.h"

using namespace std;

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
  
  jdg::Image<float> show_img;
  jdg::Image<complex<float> > img;
  jdg::Image<complex<float> > img2;
  
  // experiment 1
    img.load("./images/rect.pgm");
    jdg::fft(img,1);
    show_img = img;
    show_img.normalize(jdg::MINMAX_LOG,0.0,255.0);
    show_img.save("./images/rect_fft.pgm");
    show_img.show();

  // experiment 2
    img.load("./images/boy_noisy.pgm");
    jdg::fft(img,1);
      
    show_img = img;
    show_img.normalize(jdg::MINMAX_LOG,0.0,255.0);
    show_img.show();
    show_img.save("./images/boy_noisy_fft_orig.pgm");

    // find top 4 values and remove
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

      // minimum distance from center is 1/20 of image max(height,width)
      float min_dist = max(img.getHeight(),img.getWidth())/20.0;

      #define DIST(p1,p2) sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))

      for ( int i = img.getHeight()-1; i>=0; --i )
        for ( int j = img.getWidth()-1; j>=0; --j )
        {
          temp.x = j;
          temp.y = i;
          if ( DIST(temp, center) > min_dist )
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

      #undef DIST

      // average with values around it
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
    
    // save fixed fft
    show_img = img;
    show_img.normalize(jdg::MINMAX_LOG,0.0,255.0);
    show_img.show();
    show_img.save("./images/boy_noisy_fft_fixed.pgm");

    // inverse fft
    jdg::fft(img,-1);
    
    // save fixed image
    show_img = img;
    show_img.show();
    show_img.save("./images/boy_noisy_fixed.pgm");

    // try with gausianm 5x5 and 7x7
    img.load("./images/boy_noisy.pgm");

    // 5x5
    img2.resizeCanvas(5,5);
    buildLowPass( img2, jdg::GAUSSIAN, 0.307307, 0, true ); 

    float sum = 0.0;
    // normalize the gaussian to sum to 1
    for ( int i = 0; i < 5; i++ )
      for ( int j = 0; j < 5; j++ )
        sum += abs(img2(j,i));
    img2 /= sum;

    jdg::Image<complex<float> > temp_img(img);

    jdg::convolve( temp_img, img2 );
    show_img = temp_img;
    show_img.show();
    show_img.save("./images/boy_noisy_5x5_gaussian.pgm");

    jdg::convolve( temp_img, img );
   
    // 7x7
    img2.resizeCanvas(7,7);
    buildLowPass( img2, jdg::GAUSSIAN, 0.307307, 0, true ); 

    sum = 0.0;
    // normalize the gaussian to sum to 1
    for ( int i = 0; i < 7; i++ )
      for ( int j = 0; j < 7; j++ )
        sum += abs(img2(j,i));
    img2 /= sum;

    temp_img = img;

    jdg::convolve( temp_img, img2 );
    show_img = temp_img;
    show_img.show();
    show_img.save("./images/boy_noisy_7x7_gaussian.pgm");

    // 15x15
    img2.resizeCanvas(15,15);
    buildLowPass( img2, jdg::GAUSSIAN, 0.307307, 0, true ); 

    sum = 0.0;
    // normalize the gaussian to sum to 1
    for ( int i = 0; i < 15; i++ )
      for ( int j = 0; j < 15; j++ )
        sum += abs(img2(j,i));
    img2 /= sum;

    temp_img = img;

    jdg::convolve( temp_img, img2 );
    show_img = temp_img;
    show_img.show();
    show_img.save("./images/boy_noisy_15x15_gaussian.pgm");
    
    // 25x25
    img2.resizeCanvas(25,25);
    buildLowPass( img2, jdg::GAUSSIAN, 0.307307, 0, true ); 

    sum = 0.0;
    // normalize the gaussian to sum to 1
    for ( int i = 0; i < 25; i++ )
      for ( int j = 0; j < 25; j++ )
        sum += abs(img2(j,i));
    img2 /= sum;

    temp_img = img;

    jdg::convolve( temp_img, img2 );
    show_img = temp_img;
    show_img.show();
    show_img.save("./images/boy_noisy_25x25_gaussian.pgm");

  // return
  return 0;
}

