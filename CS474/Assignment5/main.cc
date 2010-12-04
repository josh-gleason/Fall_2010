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
void findLargest( const jdg::Image<pType>& img, Point<pType> lst[], int size,
  const jdg::Image<int>& mask );

template <class pType>
double mse( const jdg::Image<pType>& img1, const jdg::Image<pType>& img2 );

template <class pType>
void keepLargest( const jdg::Image<pType>& img, int count, jdg::Image<pType>& output,
  const jdg::Image<int>& mask );


int main(int argc, char* argv[])
{
  jdg::Image<double> lenna((argc>2?argv[2]:"images/lenna.pgm"));
  jdg::Image<double> all_coefs, coefs, show, recons;

  //lenna.show();

  // create haar coeficient image in coefs

  jdg::Image<int> mask(1,1);

  int height, width;

  int mask_elems = 0;

  if ( argc > 3 && argv[3][0] != '0' )
  {
    jdg::daubechiesTransform(lenna,all_coefs);
    width = all_coefs.getWidth();
    height = all_coefs.getHeight();
    mask.pad(width, height, jdg::ZEROS);
    mask(0,0) = 1;
    mask(0,1) = 1;
    mask(1,0) = 1;
    mask(1,1) = 1;
    mask_elems = 4;
  }
  else
  {
    jdg::haarTransform(lenna,all_coefs);
    width = all_coefs.getWidth();
    height = all_coefs.getHeight();
    mask.pad(width, height, jdg::ZEROS);
    mask(0,0) = 1;
    mask_elems = 1;
  }

  // mess with coefs

  int min = 0, max = width*height-mask_elems;
  int mid = (max+min)/2;

  double ideal = (argc>1?atof(argv[1]):5.0),
         mean_err = 1;
  
  while ( max >= min )
  {
    mid = (max+min)/2;

    // keep largest
    keepLargest( all_coefs, mid, coefs, mask );

    // reconstruct
    if ( argc > 3 && argv[3][0] != '0' )
      jdg::idaubechiesTransform(coefs,recons);
    else
      jdg::ihaarTransform(coefs,recons);

    // check mean square error
    mean_err = mse(recons, lenna);

    cout << "MSE: " << mean_err
         << "\tUsing " << (100.0*mid)/(width*height) << '%'
         << "\t(" << mid << " pixels)" << endl;

    if ( mean_err > ideal )
      min = mid+1;  // use more coeffs
    else
      max = mid-1;  // use less coeffs
  }
  
  // use 1 more pixel to get over the ideal
  if ( mean_err > ideal )
  {
    mid+=1;
    keepLargest( all_coefs, mid, coefs, mask );
  }

  // only calculate if size is less than 100% otherwise keep everything
  // reconstruct

  if ( argc > 3 && argv[3][0] != '0' )
    jdg::idaubechiesTransform(coefs,recons);
  else
    jdg::ihaarTransform(coefs,recons);
  
  // display results

  cout << "\nFinal Results" << endl
       << "MSE: " << mse(lenna, recons)
       << "\tUsing " << (100.0*mid)/(width*height) << '%'
       << "\t(" << mid << " pixels)" << endl;

  show = coefs;
  show.normalize( jdg::MINMAX_LOG, 0.0, 255.0 );

  show.show();  

  recons.normalize( jdg::MINMAX, 0.0, 255.0 );

  recons.show();
  
  return 0;
}

template <class pType>
double mse( const jdg::Image<pType>& img1, const jdg::Image<pType>& img2 )
{
  // assumes same width and height
  int height = img1.getHeight();
  int width = img2.getHeight();

  double meanerr = 0.0;
  pType diff;
  for ( int x = 0; x < height; x++ )
    for ( int y = 0; y < width; y++ )
    {
      diff = img1(x,y)-img2(x,y);
      meanerr += diff*diff;
    }
  meanerr /= height*width;
  return meanerr;
}

template <class pType>
void findLargest( const jdg::Image<pType>& img, Point<pType> lst[], int size, const jdg::Image<int>& mask )
{
  int len=0;

  Point<pType> current, temp;
  int height = img.getHeight(),
      width = img.getWidth();

  for ( int x = 0; x < width; x++ )
    for ( int y = 0; y < height; y++ )
      if ( mask(x,y) == 0 )
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

// nonzero values of the mask are simply copied over to the output
template <class pType>
void keepLargest( const jdg::Image<pType>& img, int count, jdg::Image<pType>& output,
  const jdg::Image<int>& mask )
{
  if ( count >= img.getWidth() * img.getHeight() )
    return;

  int width = img.getWidth(),
      height = img.getHeight();

  Point<pType>* lst = new Point<pType>[count];
  findLargest( img, lst, count, mask ); // make list of largest values

  // clear output image
  if ( output.getWidth() != width || output.getHeight() != height )
    output.pad(width, height);

  // copy values over
  for ( int x = 0; x < width; x++ )
    for ( int y = 0; y < width; y++ )
    {
      if ( mask(x,y) == 0 )
        output(x,y) = 0;
      else
        output(x,y) = img(x,y);
    }

  // add largest back to output
  for ( int i = 0; i < count; i++ )
    output(lst[i].x, lst[i].y) = lst[i].val;

  // clean up
  delete [] lst;
}
