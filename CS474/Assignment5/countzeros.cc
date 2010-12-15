#include "funcs.h"
#include "filters.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace jdg;

// count the number of coefficients in different transforms and output to file
int main( int argc, char* argv[] )
{
  if ( argc < 2 )
    return 0;

  Image<double> orig(argv[1]);
  Image<complex<double> > fourier = orig;
  Image<double> show, wavelet[11];
  Filters<double> filts;
  int wzeros[11], fzeros;

  ofstream fout;

  if ( argc > 2 )
  {
    fout.open(argv[2]);
    cout << "Outputing to file " << argv[2] << endl;
  }

  // fourier
  fft( fourier );
  
  fzeros = 
    orig.getWidth() * orig.getHeight() - fourier.getWidth() * fourier.getHeight();
    

  for ( int x = 0; x < fourier.getWidth(); x++ )
    for ( int y = 0; y < fourier.getHeight(); y++ )
      if ( abs(fourier(x,y)) < 0.5 )
        fzeros++;
  
  cout << "Fourier: " << fzeros << endl;
  
  if ( argc > 2 )
    fout << 100.0*fzeros/(orig.getWidth() * orig.getHeight()) << ' ';
  
  // wavelets
  for ( int i = 0; i < 11; i++ )
  {
    waveletTrans( orig, wavelet[i], filts((Wavelet)i) );
    wzeros[i] = 
      orig.getWidth() * orig.getHeight() - wavelet[i].getWidth() * wavelet[i].getHeight();
    for ( int x = 0; x < fourier.getWidth(); x++ )
      for ( int y = 0; y < fourier.getHeight(); y++ )
        if ( abs(wavelet[i](x,y)) < 0.5 )
          wzeros[i]++;
    
    cout << "D" << (i+1)*2 << ": " << wzeros[i] << endl;
    
    if ( argc > 2 )
      fout << 100.0*wzeros[i]/(orig.getWidth() * orig.getHeight()) << ' ';
  }
  
  if ( argc > 2 )
  {
    fout << endl;
    fout.close();
  }

  return 0;
}

