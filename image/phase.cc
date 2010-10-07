#include "funcs.h"

using namespace std;

int main( int argc, char* argv[] )
{
  jdg::Image<complex<float> > orig(argv[1]);
  jdg::Image<complex<float> > fft=orig;
  jdg::fft(fft);
  jdg::Image<complex<float> > mag=fft;
  jdg::Image<complex<float> > phase=fft;

  for ( int i = mag.getHeight()-1; i>=0; --i )
    for ( int j = mag.getWidth()-1; j>=0; --j )
    {
      mag(j,i) = abs(mag(j,i));
      phase(j,i) = phase(j,i) / abs(phase(j,i));
    }
  
//  jdg::Image<complex<float> > prod=mag*phase;

  jdg::fft(mag,-1);
  jdg::fft(phase,-1);
//  jdg::fft(prod,-1);

  mag.pad( orig.getWidth(), orig.getHeight() );
  phase.pad( orig.getWidth(), orig.getHeight() );
//  prod.pad( orig.getWidth(), orig.getHeight() );

  jdg::Image<float> mag_show=mag;
  jdg::Image<float> phase_show=phase;
//  jdg::Image<float> prod_show=prod;

  mag_show.normalize( jdg::MINMAX_LOG, 0, 255 );
  phase_show.normalize( jdg::MINMAX, 0, 255 );
  //prod_show.normalize( jdg::MINMAX, 0, 255 );
  
  mag_show.show();
  phase_show.show();

  jdg::convolve(mag,phase);

  mag_show=mag;
  mag_show.normalize( jdg::MINMAX, 0, 255 );
  mag_show.show();

 // prod_show.show();

}
