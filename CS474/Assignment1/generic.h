#include <cv.h>
#include <highgui.h>

namespace jdg
{
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
}

