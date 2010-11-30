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

// find the 'size' largest points in the image and store in 'lst' which should
// be pre-initialized to 'size' elements, ignores point (0,0)
template <class pType>
void findLargest( const jdg::Image<pType>& img, Point<pType> lst[], int size );

int main(int argc, char* argv[])
{
  jdg::Image<double> lenna((argc>2?argv[2]:"images/lenna.pgm"));
  jdg::Image<double> coefs, recons, show;

  //lenna.show();

  // create haar coeficient image in coefs

  jdg::haarTransform(lenna,coefs);
  
  // mess with coefs

  const int size = coefs.getWidth()*coefs.getHeight()*(argc>1?atof(argv[1]):0.1);
  Point<double> lst[size]; // = new Point<double>[size];
  findLargest( coefs, lst, size );

  jdg::Image<double> newimg(1,1);

  // create blank image
  newimg.pad(coefs.getWidth(), coefs.getHeight());
 
  newimg(0,0) = coefs(0,0); // this is not a coeficient, it must be there

  for ( int i = 0; i < size; i++ )
    if ( lst[i].x >= 0 || lst[i].y >= 0 )
      newimg(lst[i].x, lst[i].y) = lst[i].val;

  coefs = newimg;
  
  // reconstruct

  jdg::ihaarTransform(coefs,recons);
  
  // display results

  show = coefs;
  show.normalize( jdg::MINMAX_LOG, 0.0, 255.0 );

  show.show();  
  recons.show();

  // clean
  //delete [] lst;

  return 0;
}

template <class pType>
void findLargest( const jdg::Image<pType>& img, Point<pType> lst[], int size )
{
  int len=0;

  Point<pType> current, temp;
  int height = img.getHeight(),
      width = img.getWidth();

  for ( int x = 0; x < width; x++ )
    for ( int y = 0; y < height; y++ )
      if ( x != 0 || y != 0 ) // ignore (0,0)
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
            if ( abs(lst[i].val) < abs(current.val) )
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
        else if ( abs(lst[len-1].val) >= abs(current.val) ) // special case
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
          mid = (max+min)/2;

          // find location
          while ( abs(lst[mid].val) != abs(current.val) && min <= max )
          {
            mid = (max+min)/2;
            if ( abs(current.val) < abs(lst[mid].val) )
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
      } // end loop
}

