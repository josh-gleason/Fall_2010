#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <sstream>
#include "generic.h"

using namespace std;

string err( const char* err )
{
  string retVal = err;
  return err;
}

void quantize( string fileName )
{
  // open image
  cout << "Loading " << fileName << "...";
  IplImage* img = cvLoadImage( fileName.c_str(), 0 );

  if ( img == 0 )
    throw err("Invalid file or image format");

  cout << "Complete" << endl;

  // remove file extension from name for later
  string extention;
  istringstream sin(fileName);
  getline( sin, fileName, '.' );
  sin >> extention;

  // will be used to construct output file name
  ostringstream sout;

  // create the object to hold the quantized image
  IplImage* quantized = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
  
  // display original
  cvNamedWindow( "Image" );
  cvShowImage( "Image", img );
  cvWaitKey();

  // 2, 4, and 6 bits
  for ( int dim = 2; dim < 8; dim+=2 )
  {
    // calculate the number of colors
    int colors = static_cast<int>( pow(2,dim) );
    
    // quantize image
    cout << "Quantizing Image..." << flush;
    jdg::quantize( img, quantized, 0, 255, colors );
    cout << "Complete" << endl;
   
    // save file
    sout.str("");
    sout << fileName << '_' << dim << "bit" << '.' << extention;
    
    cout << "Saving " << sout.str() << "..." << flush;
    cvSaveImage(sout.str().c_str(),quantized);
    cout << "Complete" << endl;
    
    // display image
    cvShowImage("Image",quantized);
    cvWaitKey();
  }

  cvReleaseImage( &img );
  cvReleaseImage( &quantized );
}

int main(int argc, char *argv[])
{
  try
  {
    if ( argc < 2 )
      throw err("Please supply an image filename");
    quantize( argv[1] );
  }
  catch(string err)
  {
    cout << "\n\tError : " << err << endl;
  }

  return 0;
}
