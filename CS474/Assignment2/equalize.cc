#include <iostream>
#include <fstream>
#include <cv.h>
#include <highgui.h>

#include "generic.h"

//#define USE_OCV

using namespace std;

int main(int argc, char *argv[])
{
  if ( argc < 2 ) 
  {
    cout << "Usage: ./program <original image> {output filename} "
            "{output histogram filename} {original histogram filename} " << endl
         << "Parameters in {} are optional" << endl;
    return -1;
  }

  IplImage* orig = cvLoadImage( argv[1], 0 );
  IplImage* equalized = cvCloneImage( orig );

  jdg::equalizeImage( equalized );
  
  // save results
  float temp_histo[256];
  if ( argc >= 3 )
    cvSaveImage( argv[2], equalized );
  if ( argc >= 4 )
  {
    jdg::calcHisto( equalized, temp_histo );
    jdg::saveHisto( argv[3], temp_histo );
  }
  if ( argc >= 5 )
  {
    jdg::calcHisto( orig, temp_histo );
    jdg::saveHisto( argv[4], temp_histo );
  }

  // display results
  cvNamedWindow("Original");
  cvNamedWindow("Equalized");
  cvShowImage("Original", orig );
  cvShowImage("Equalized", equalized );
  cvWaitKey();
  cvReleaseImage( &orig );
  cvReleaseImage( &equalized );

  return 0;
}
