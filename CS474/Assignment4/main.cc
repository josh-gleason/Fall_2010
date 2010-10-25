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
  jdg::Image<complex<double> > gaussian(40, 40);
  jdg::Image<double> display;
  display = lenna;
  display.show();
  jdg::gaussian( gaussian, 20, -1, 0, 1 );
  gaussian.normalize( jdg::L1, 1000000.0, 0.0 );
  jdg::convolve( lenna, gaussian, jdg::ZEROS );

  display = lenna;
  cout << display.min() << ' ' << display.max() << endl;
  //display.normalize( jdg::MINMAX, 0, 255 );
  display.show();

  gaussian.resizeCanvas(15, 15);
  jdg::gaussian( gaussian, 2.6, -1, 0, 1 );
  gaussian.normalize( jdg::L1, 1.0, 0.0 );
  jdg::convolve( lenna, gaussian );

  display = lenna;
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

