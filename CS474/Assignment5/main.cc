#include "funcs.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

template <class pType>
struct Point
{
  Point() {}
  Point(int _x, int _y, pType _val) : x(_x), y(_y), val(_val) {}
  int x, y;
  pType val;
};

// find the 'size' largest points in the image and store in lst which should
// be pre-initialized to 'size' elements
template <class pType>
void findLargest( const jdg::Image<pType>& img, Point<pType> lst[], int size );

int main(int argc, char* argv[])
{
  jdg::Image<double> lenna((argc>1?argv[1]:"images/lenna.pgm"));
  jdg::Image<double> coefs, recons, show;

  jdg::haarTransform(lenna,coefs);
  
  // mess with coefs

/*  jdg::Image<double> filt(coefs.getWidth()*2,coefs.getHeight()*2);

//  jdg::gaussian( filt, 80, -1 );
  jdg::idealfilter( filt, 256, -1 );

  // place center in upper left
  filt.pad(filt.getWidth()/2, filt.getHeight()/2, jdg::ZEROS, -filt.getWidth()/2, -filt.getHeight()/2);

  coefs *= filt;

  show = filt;
  show.normalize(jdg::MINMAX,0.0,255.0);
  show.show();
*/
/*  static const int SIZE = 10000;
  Point<double> lst[SIZE];
  findLargest( coefs, lst, SIZE );

  jdg::Image<double> newimg(1,1);

  // create blank image
  newimg.pad(coefs.getWidth(), coefs.getHeight());
  
  for ( int i = 0; i < SIZE; i++ )
    if ( lst[i].x >= 0 || lst[i].y >= 0 )
      newimg(lst[i].x, lst[i].y) = lst[i].val;

  coefs = newimg; */
/*
  double minimum = coefs.min()-1;

  jdg::Image<double> largest(coefs.getWidth(),coefs.getHeight());
  for ( int x = 0; x < coefs.getWidth(); x++ )
  for ( int y = 0; y < coefs.getHeight(); y++ )
    largest(x,y) = 0.0;//minimum;

  largest(0,0) = coefs(0,0);
  coefs(0,0) = minimum;

  for ( int i = 0; i < 256*256*0.19; i++ )
  {
    static int x=0, y=0;
    coefs.max(x,y);
    largest(x,y) = coefs(x,y);
    coefs(x,y) = minimum;
  }

  coefs = largest; */
  // reconstruct

  jdg::ihaarTransform(coefs,recons);
  
  show = coefs;
  show.normalize( jdg::MINMAX_LOG, 0.0, 255.0 );

  show.show();  
  recons.show();

  return 0;
}

template <class pType>
void findLargest( const jdg::Image<pType>& img, Point<pType> lst[], int size )
{
  int len=0;
  // initialize list to all zero
  /*Point<pType> least(-1,-1,img.min()-1);
  for ( int i = 0; i < size; i++ )
    lst[i] = least;*/

  Point<pType> current, temp;
  int height = img.getHeight(),
      width = img.getWidth();

  for ( int x = 0; x < width; x++ )
    for ( int y = 0; y < height; y++ )
    {
      current.x = x;
      current.y = y;
      current.val = img(x,y);
      
      // no point in binary search
      if ( len < 20 )
      {
        int i = 0;
        while ( i < len )
        {
          if ( lst[i].val < current.val )
          {
            temp = lst[i];
            lst[i] = current;
            current = temp;
          }
          ++i;
        }
        if ( len < size )
        {
          lst[len] = current;
          len++;
        }
      }
      else if ( lst[len-1].val >= current.val ) // special case
      {
        if ( len < size )
        {
          lst[len] = current;
          len++;
        }
      }
      else  // do binary search
      {
        int min,max,mid;
        min = 0;
        max = len-1;

        // find location
        while ( lst[mid].val != current.val && min <= max )
        {
          mid = (max+min)/2;
          if ( current.val < lst[mid].val )
            min = mid+1;
          else
            max = mid-1;
        }

        if ( mid + 1 < size )
        {
          if ( len < size )
            len++;

          // need to place into location mid+1
          for ( int i = mid+1; i < len; i++ )
          {
            temp = lst[i];
            lst[i] = current;
            current = temp;
          }
        }
      }

    }
}

