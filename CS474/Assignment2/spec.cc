#include <iostream>
#include <fstream>
#include <cv.h>
#include <highgui.h>
#include "generic.h"

using namespace std;

int main(int argc, char *argv[])
{
  if ( argc < 3 ) 
  {
    cout << "Usage: ./program <original image> <new image> {output filename} "
            "{output histogram filename} {original histogram filename} "
            "{matching histogram filename}" << endl
         << "Parameters in {} are optional" << endl;
    return -1;
  }

  IplImage* orig = cvLoadImage(argv[1],0);
  IplImage* match = cvLoadImage(argv[2],0);
  IplImage* speced = cvCloneImage( orig );
  
  // calculate histogram to match original with
  float histo[256];
  jdg::calcHisto( match, histo );
  
  // compute histogram specification on image
  jdg::specifyImageHisto( speced, histo );

  // save results
  float temp_histo[256];
  if ( argc >= 4 )  // save specified image
    cvSaveImage( argv[3], speced );
  if ( argc >= 5 )  // save specified histogram
  {
    jdg::calcHisto( speced, temp_histo );
    jdg::saveHisto( argv[4], temp_histo );
  }
  if ( argc >= 6 )  // save original histogram
  {
    jdg::calcHisto( orig, temp_histo );
    jdg::saveHisto( argv[5], temp_histo );
  }
  if ( argc >= 7 )  // save new histogram
    jdg::saveHisto( argv[6], histo );
  
  // display results
  cvNamedWindow("original");
  cvNamedWindow("match with");
  cvNamedWindow("specified");
  cvShowImage("original", orig);
  cvShowImage("match with", match);
  cvShowImage("specified", speced);
  cvWaitKey();
  cvReleaseImage( &orig );
  cvReleaseImage( &match );
  cvReleaseImage( &speced );
  cvDestroyWindow("original");
  cvDestroyWindow("match with");
  cvDestroyWindow("specified");

  return 0;
}
