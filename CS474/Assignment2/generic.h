#include <cv.h>
#include <highgui.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

namespace jdg
{
void calcHisto( IplImage* src, float* histo, int bins=256);
void saveHisto( char* fileName, float* histo, int bins=256 );
void calcEqualizeTransform( float* histo, float* func, int bins=256 );

enum Noise_Type{ SALT_AND_PEPPER };

// quantize from range [<min>, <max>] into [0,<vals>] then back
void quantize( IplImage* src, IplImage* dest, int min, int max, int vals )
{
  int width = src->width,
      height = src->height,
      old_range = max-min+1,
      val;
  int step = old_range/vals;
  
  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
    {
      val = static_cast<int>( cvGetReal2D( src, i, j ) ) + min;
      val /= step;
      val = val*(old_range-1)/(vals-1);
      cvSetReal2D( dest, i, j, static_cast<double>(val) );
    }
}

void applyTransform( IplImage* img, float* func )
{
  int width = img->width,
      height = img->height;

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      cvSetReal2D(
        img, i, j,
        round(func[static_cast<int>(cvGetReal2D(img, i, j))])
      );
}

// as of now only computes histogram for 8 bit grayscale images
// assumes histo is initialized to the size of bins
void calcHisto( IplImage* src, float* histo, int bins )
{
  int height = src->height,
      width = src->width,
      bin;
  float size = height * width;

  // zero the histogram
  for ( int i = 0; i < bins; i++ )
    histo[i] = 0.0;

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
    {
      bin = static_cast<int>(cvGetReal2D( src, i, j ));
      histo[bin]++;         // add to the histogram bin
    }

  // make histogram sum to 1
  for ( int i = 0; i < bins; i++ )
    histo[i] /= size;
}

void calcEqualizeTransform( float* histo, float* func, int bins )
{
  func[0] = histo[0] * (bins-1);
  for ( int i = 1; i < 256; i++ )
    func[i] = func[i-1] + histo[i] * (bins - 1);
}

void equalizeImage( IplImage* img )
{
  float histo[256];
  float func[256];

  calcHisto( img, histo );
  calcEqualizeTransform( histo, func );
  applyTransform( img, func );
}

void specifyImageHisto( IplImage* img, float* histo_spec )
{
  float histo_orig[256];

  // calculate the original histogram
  calcHisto( img, histo_orig );

  float func_orig[256];
  float func_spec[256];

  calcEqualizeTransform( histo_orig, func_orig );
  calcEqualizeTransform( histo_spec, func_spec );

  float func_new[256];

  for ( int i = 0; i < 256; i++ )
  {
    // starting value
    int start = 0;
    if ( i != 0 )
      start = func_new[i-1];
    func_new[i] = -1;
    for ( int j = start; j < 256 && func_new[i] < 0; j++ )
      if ( func_spec[j] > func_orig[i] || func_spec[j] == 255 )
        func_new[i] = std::max(0,j-1);
  }
  applyTransform( img, func_new );
}

void saveHisto( char* fileName, float* histo, int bins )
{
  std::ofstream fout(fileName);
  for ( int i = 0; i < bins; i++ )
    fout << histo[i] << ' ';
  fout.close();
}

void addSaltAndPepper( IplImage* img, int percentage )
{
  int height = img->height,
      width = img->width;
  
  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      if (rand() % 100 < percentage)
        cvSetReal2D( img, i, j, (rand() % 2) * 255.0 );
}

void medianFilter( IplImage*& img, int filter_size )
{
  int padding = (filter_size - 1) / 2;
  uchar* values = new uchar[filter_size*filter_size];

  IplImage* result = cvCreateImage( cvGetSize(img), img->depth, 1 );

  for ( int i = 0; i < img->height; i++ )
    for ( int j = 0; j < img->width; j++ )
    {
      // itterate accross local region
      static int low_i, up_i, low_j, up_j, index;
      low_i = i-padding;
      up_i = i+padding;
      low_j = j-padding;
      up_j = j+padding;
      index = 0;
      // build list
      for ( int f_i = low_i; f_i <= up_i; f_i++ )
        for ( int f_j = low_j; f_j <= up_j; f_j++ )
        {
          if ( f_i < 0 || f_j < 0 || f_i >= img->height || f_j >= img->width )
            values[index] = 0;  // pad with zeros
          else
            values[index] = static_cast<unsigned char>(
              cvGetReal2D( img, f_i, f_j ));
          index++;
        }
      // sort the values
      std::sort(values, values+(filter_size*filter_size));
      // set to median
      cvSetReal2D( result, i, j,
        static_cast<double>(values[(filter_size*filter_size-1)/2]));
    }

  cvReleaseImage( &img );
  delete [] values;

  img = result;
}

void meanFilter( IplImage*& img, int filter_size )
{
  int padding = (filter_size - 1) / 2;
  IplImage* result = cvCreateImage( cvGetSize(img), img->depth, 1 );

  for ( int i = 0; i < img->height; i++ )
    for ( int j = 0; j < img->width; j++ )
    {
      // itterate accross local region
      static int low_i, up_i, low_j, up_j;
      static double total;
      low_i = i-padding;
      up_i = i+padding;
      low_j = j-padding;
      up_j = j+padding;
      total = 0;
      for ( int f_i = low_i; f_i <= up_i; f_i++ )
        for ( int f_j = low_j; f_j <= up_j; f_j++ )
          if ( !(f_i < 0 || f_j < 0 || f_i >= img->height || f_j >= img->width) )
            total += cvGetReal2D( img, f_i, f_j );
      // set to mean
      double mean = total / (filter_size * filter_size);
      cvSetReal2D( result, i, j, mean );
    }

  cvReleaseImage( &img );
  img = result;
}

void medianFilterBetter( IplImage*& img, int filter_size, int thresh )
{
  int padding = (filter_size - 1) / 2;
  uchar* values = new uchar[filter_size*filter_size];

  IplImage* result = cvCreateImage( cvGetSize(img), img->depth, 1 );

  for ( int i = 0; i < img->height; i++ )
    for ( int j = 0; j < img->width; j++ )
    {
      if ( cvGetReal2D( img, i, j ) <= thresh || cvGetReal2D( img, i, j ) >= 255-thresh )
      {
        // itterate accross local region
        static int low_i, up_i, low_j, up_j, index;
        low_i = i-padding;
        up_i = i+padding;
        low_j = j-padding;
        up_j = j+padding;
        index = 0;
        // build list
        for ( int f_i = low_i; f_i <= up_i; f_i++ )
          for ( int f_j = low_j; f_j <= up_j; f_j++ )
          {
            if ( f_i < 0 || f_j < 0 || f_i >= img->height || f_j >= img->width )
              values[index] = 0;  // pad with zeros
            else
              values[index] = static_cast<unsigned char>(
                cvGetReal2D( img, f_i, f_j ));
            if ( values[index] >= thresh && values[index] <= 255-thresh )
              index++;
          }
        // sort the values
        int median = 127;
        if ( index != 0 )
        {
          std::sort(values, values+(index-1));
          median = values[index/2];
        }
   
        // set to median
        cvSetReal2D( result, i, j, static_cast<double>(median));
      }
      else
        cvSetReal2D( result, i, j, cvGetReal2D( img, i, j ));
    }

  cvReleaseImage( &img );
  delete [] values;

  img = result;
}

}

