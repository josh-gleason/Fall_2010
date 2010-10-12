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
  
  jdg::fft(mag,-1);
  jdg::fft(phase,-1);
  
  //mag.pad( orig.getWidth(), orig.getHeight() );
  //phase.pad( orig.getWidth(), orig.getHeight() );

  jdg::Image<float> mag_show=mag;
  jdg::Image<float> phase_show=phase;

  mag_show.pad( orig.getWidth(), orig.getHeight() );
  phase_show.pad( orig.getWidth(), orig.getHeight() );

  mag_show.normalize( jdg::MINMAX_LOG, 0, 255 );
  phase_show.normalize( jdg::MINMAX, 0, 255 );
  
  mag_show.show();
  phase_show.show();

  jdg::convolve(mag,phase,jdg::WRAP);

  // shift and resize image by 1/2
  mag.pad(
    orig.getWidth(),
    orig.getHeight(),
    jdg::WRAP,
    mag.getWidth()/2,
    mag.getHeight()/2
  );  

  mag_show=mag;
  mag_show.show();

  if ( argc > 2 )
  mag_show.save(argv[2]);

}
