#include <iostream>
#include "generic.h"

using namespace std;

int main(int argc, char *argv[])
{
  // check arguments
  if ( argc < 2 )
  {
    cout << "Usage: ./program <image filename> {filter size=3} "
            "{salt/pepper percentage=50} {median filename} "
            "{mean filename} {salt/pepper filename}" << endl
         << "Parameters in {} are optional" << endl;
    return -1;
  }

  // get filter_size and salt/pepper percentage
  int filter_size = 3,
      pepper_percentage = 50;

  if ( argc >= 3 )
  {
    filter_size = atoi(argv[2]);
    filter_size += filter_size % 2 - 1; // ensure value is odd
  }
  if ( argc >= 4 )
    pepper_percentage = atoi(argv[3]);

  // initialize images
  IplImage* orig = cvLoadImage( argv[1], 0 );
  IplImage* salt_pepper = cvCloneImage( orig );

  // add salt and pepper to original
  jdg::addSaltAndPepper( salt_pepper, pepper_percentage );

  // intialize median and mean images
  IplImage* median = cvCloneImage( salt_pepper );
  IplImage* median_better = cvCloneImage( salt_pepper );
  IplImage* mean = cvCloneImage( salt_pepper );

  // run filters on median and mean
  jdg::medianFilter( median, filter_size );
  jdg::meanFilter( mean, filter_size );

  // better median filter which ignores values <= 0+1 and >= 255-1 when
  // calculating median
  jdg::medianFilterBetter( median_better, filter_size, 1 );

  // save results
  if ( argc >= 5 )
    cvSaveImage( argv[4], median );
  if ( argc >= 6 )
    cvSaveImage( argv[5], mean );
  if ( argc >= 7 )
    cvSaveImage( argv[6], salt_pepper );

  // display results
  cvNamedWindow("Original");
  cvNamedWindow("Salt and Pepper");
  cvNamedWindow("Mean");
  cvNamedWindow("Median(Better)");
  cvNamedWindow("Median");

  cvShowImage("Original", orig);
  cvShowImage("Salt and Pepper", salt_pepper);
  cvShowImage("Mean", mean);
  cvShowImage("Median(Better)", median_better);
  cvShowImage("Median", median);

  cvWaitKey();

  // clean up memory
  cvDestroyWindow("Original");
  cvDestroyWindow("Salt and Pepper");
  cvDestroyWindow("Mean");
  cvDestroyWindow("Median(Better)");
  cvDestroyWindow("Median");

  cvReleaseImage(&orig);
  cvReleaseImage(&salt_pepper);
  cvReleaseImage(&mean);
  cvReleaseImage(&median_better);
  cvReleaseImage(&median);

  return 0;
}
