#include <iostream>
#include "funcs.h"
#include <sstream>

using namespace std;

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
    a = a*filter;
    
    // step 4
    jdg::fft(a,-1);
    
    // step 5
    a.callFunc( &exponential );

    show = a;
    show.normalize( jdg::MINMAX_LOG, 0, 225 );
    //show.show();

    ostringstream sout;
    sout << "./images/girl_" << YL*10 << "_" << YH*10 << ".pgm";
    show.save(sout.str().c_str());
  }

  return 0;
}

