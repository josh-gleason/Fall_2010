#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;

string err( const char* err )
{
  string retVal = err;
  return err;
}

void spacial( string fileName )
{
  // open image
  cout << "Loading " << fileName << "...";
  IplImage* img = cvLoadImage( fileName.c_str(), 0 );

  if ( img == 0 )
    throw err("Invalid file or image format");
  if ( img->width != 256 || img->height != 256 )
    throw err("Width and height of image must be 256");

  cout << "Complete" << endl;

  // remove file extension
  string extention;
  istringstream sin(fileName);
  getline( sin, fileName, '.' );
  sin >> extention;

  // will be used to construct output file name
  ostringstream sout;

  // create the resized image
  IplImage* resized = cvCreateImage( cvSize( 256, 256 ), IPL_DEPTH_8U, 1 );
  
  // display original
  cvNamedWindow( "Image" );
  cvShowImage( "Image", img );
  cvWaitKey();

  // size is new resolution (32, 64, or 128) and pix is the size of a block
  int size, pix, offsetX, offsetY, avg;
  
  // 2^5 2^6 2^7
  for ( int dim = 5; dim < 8; dim++ )
  {
    // calculate the size
    size = static_cast<int>(pow(2,dim));
    pix = 256/size;
   
    cout << "Changing spacial resolution to " << size << "..." << flush;
    
    // change spacial resolution and resize using nearest neighbor simultaniously
    for ( int i = 0; i < size; i++ )
      for ( int j = 0; j < size; j++ )
      {
        // location of upper left corner of block
        offsetX = i*pix;
        offsetY = j*pix;

        // get average value in block
        avg = 0;
        for ( int k = 0; k < pix; k++ )
          for ( int l = 0; l < pix; l++ )
            avg += cvGetReal2D( img, offsetX+k, offsetY+l );
        avg /= pix*pix;
        
        // set all pixels in block to that average
        for ( int k = 0; k < pix; k++ )
          for ( int l = 0; l < pix; l++ )
            cvSetReal2D( resized, offsetX+k, offsetY+l, avg );
      }

    cout << "Complete" << endl;
   
    // save file
    sout.str("");
    sout << fileName << '_' << size << '.' << extention;
    
    cout << "Saving " << sout.str() << "..." << flush;
    cvSaveImage(sout.str().c_str(),resized);
    cout << "Complete" << endl;
    
    // display image
    cvShowImage("Image",resized);
    cvWaitKey();
  }

  cvReleaseImage( &img );
  cvReleaseImage( &resized );
}

int main(int argc, char *argv[])
{
  try 
  {
    if ( argc < 2 )
      throw err("Please supply an image filename that is 256x256");
    spacial( argv[1] );
  } 
  catch(string err) 
  {
    cout << "\n\tError : " << err << endl;
  }

  return 0;
}
