#include <iostream>
#include "funcs.h"
#include <sstream>
#include <iomanip>

using namespace std;

#define PART1
//#define PART2

complex<double> natlog( complex<double> val )
{
  return log(abs(val));
}

complex<double> exponential( complex<double> val )
{
  return exp(abs(val));
}

int main(int argc, char* argv[])
{
  // part 1
#ifdef PART1
  jdg::Image<complex<double> > lenna("images/girl.pgm");
  jdg::Image<complex<double> > gaussian15(15,15);
  jdg::Image<complex<double> > gaussian7(7,7);
  jdg::Image<complex<double> > result;
  jdg::Image<double> display;
 
  int mask15[] =
    {2 ,2 ,3 ,4 ,5 ,5 ,6 ,6 ,6 ,5 ,5 ,4 ,3 ,2 ,2,
     2 ,3 ,4 ,5 ,7 ,7 ,8 ,8 ,8 ,7 ,7 ,5 ,4 ,3 ,2,
     3 ,4 ,6 ,7 ,9 ,10,10,11,10,10,9 ,7 ,6 ,4 ,3,
     4 ,5 ,7 ,9 ,10,12,13,13,13,12,10,9 ,7 ,5 ,4,
     5 ,7 ,9 ,11,13,14,15,16,15,14,13,11,9 ,7 ,5,
     5 ,7 ,10,12,14,16,17,18,17,16,14,12,10,7 ,5,
     6 ,8 ,10,13,15,17,19,19,19,17,15,13,10,8 ,6,
     6 ,8 ,11,13,16,18,19,20,19,18,16,13,11,8 ,6,
     6 ,8 ,10,13,15,17,19,19,19,17,15,13,10,8 ,6,
     5 ,7 ,10,12,14,16,17,18,17,16,14,12,10,7 ,5,
     5 ,7 ,9 ,11,13,14,15,16,15,14,13,11,9 ,7 ,5,
     4 ,5 ,7 ,9 ,10,12,13,13,13,12,10,9 ,7 ,5 ,4,
     3 ,4 ,6 ,7 ,9 ,10,10,11,10,10,9 ,7 ,6 ,4 ,3,
     2 ,3 ,4 ,5 ,7 ,7 ,8 ,8 ,8 ,7 ,7 ,5 ,4 ,3 ,2,
     2 ,2 ,3 ,4 ,5 ,5 ,6 ,6 ,6 ,5 ,5 ,4 ,3 ,2 ,2};

  int mask7[] =
    {1 ,1 ,2 ,2 ,2 ,1 ,1,
     1 ,2 ,2 ,4 ,2 ,2 ,1,
     2 ,2 ,4 ,8 ,4 ,2 ,2,
     2 ,4 ,8 ,16,8 ,4 ,2,
     2 ,2 ,4 ,8 ,4 ,2 ,2,
     1 ,2 ,2 ,4 ,2 ,2 ,1,
     1 ,1 ,2 ,2 ,2 ,1 ,1};
  
  for ( int i = 0; i < 15; i++ )
    for ( int j = 0; j < 15; j++ )
      gaussian15(i,j) = mask15[i*15+j];

  for ( int i = 0; i < 7; i++ )
    for ( int j = 0; j < 7; j++ )
      gaussian7(i,j) = mask7[i*7+j];

  gaussian15.normalize( jdg::L1, 1.0 ); // normalize so sum = 1
  gaussian7.normalize( jdg::L1, 1.0 );

  result = lenna;
 
  // convolve using frequency domain
  jdg::convolve( result, gaussian15, jdg::WRAP );

  display = result;
  cout << display.min() << endl;
  cout << display.max() << endl;
  display.show();
#endif // PART1

  // part 2
#ifdef PART2
  for ( float YL = 0.2; YL <= 0.8; YL+=0.1 )
  for ( float YH = 1.2; YH <= 1.8; YH+=0.1 )
  {
    jdg::Image<complex<double> > a("images/girl.pgm");
    jdg::Image<complex<double> > filter(a.getWidth(),a.getHeight());
    jdg::Image<double> show;
    
    // step 1
    a.callFunc( &natlog );
    
    // step 2
    jdg::fft(a);
    
    // step 3
    // 0.3 and 1.3 ? seem to be good 
    jdg::butterworth( filter, 1.8, 1.0, YL, YH );
    //jdg::gaussian( filter, 1.8, 1, YL, YH );
    //jdg::idealfilter( filter, 1.8, 1, YL, YH );

    show = filter;
    show *= 255.0 / YH;
    show.show();

    a = a*filter;
    
    // step 4
    jdg::fft(a,-1);
    
    // step 5
    a.callFunc( &exponential );

    show = a;
    show.normalize( jdg::MINMAX_LOG, 0, 225 );
    show.show();

    ostringstream sout;
    sout << "./images/girl_" << YL*10 << "_" << YH*10 << ".pgm";
    show.save(sout.str().c_str());
  }
#endif // PART2
  return 0;
}

