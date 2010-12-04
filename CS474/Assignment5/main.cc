#include "funcs.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector> 

using namespace std;

enum Wavelet { HAAR, DAUBECHIES };

template <class pType>
struct Point
{
  Point() {}
  Point(int _x, int _y, pType _val) : x(_x), y(_y), val(_val) {}
  int x, y;
  pType val;
};

struct Settings
{
  string input, output;
  double minErr;
  Wavelet type;
  bool showImages;
  bool showProgress;
  bool showResults;
};

const int DIVIDER = 18;

// find the 'size' largest points in the image and store in 'lst' which should
// be pre-initialized to 'size' elements, ignores point (0,0)
template <class pType>
void findLargest( jdg::Image<pType>& img, vector<Point<pType> >& lst, int size );

template <class pType>
double mse( const jdg::Image<pType>& img1, const jdg::Image<pType>& img2 );

void readSettings( int argc, char* argv[], Settings& settings );

template <class pType>
int removeCoefs( const jdg::Image<pType>& img, jdg::Image<pType>& coefs, double maxErr, Wavelet type, bool showProgress );

int main(int argc, char* argv[])
{
  // read settings
  Settings settings;
  readSettings( argc, argv, settings );
 
  // initialize variables
  jdg::Image<double> img(settings.input);
  jdg::Image<double> coefs, recons;
 
  // show original image
  if ( settings.showImages )
    img.show();

  // remove coefficients
  int pixels =  // remove just enough to get the minimum desired error
    removeCoefs( img, coefs, settings.minErr, settings.type, settings.showProgress );

  // reconstruct
  if ( settings.type != HAAR )
    jdg::idaubechiesTransform(coefs,recons);
  else if ( settings.type == DAUBECHIES )
    jdg::ihaarTransform(coefs,recons);
  
  // save results
  if ( settings.output != "" )
    recons.save(settings.output);

  // display statistical results
  if ( settings.showResults )
    cout << "\nFinal Results" << endl
         << "MSE: " << mse(img, recons)
         << "\tUsing " << (100.0*pixels)/(img.getWidth()*img.getHeight()) << '%'
         << "\t(" << pixels << " pixels)" << endl;
  
  // show output image
  if ( settings.showImages )
    recons.show();

  return 0;
}

void readSettings( int argc, char* argv[], Settings& settings )
{
  // defaults
  settings.input = "images/lenna.pgm";
  settings.output = "";
  settings.type = DAUBECHIES;
  settings.minErr = 25.0;
  settings.showImages = true;
  settings.showProgress = true;
  settings.showResults = true;

  for ( int i = 0; i < argc; i++ )
  {
    if ( strcmp(argv[i],"-daubechies") == 0 )
      settings.type = DAUBECHIES;
    else if ( strcmp(argv[i],"-haar") == 0 )
      settings.type = HAAR;
    else if ( strcmp(argv[i],"-hideimages") == 0 )
      settings.showImages = false;
    else if ( strcmp(argv[i],"-hideprogress") == 0 )
      settings.showProgress = false;
    else if ( strcmp(argv[i],"-hideresults") == 0 )
      settings.showResults = false;
    else if ( strcmp(argv[i],"-hideall") == 0 )
      settings.showImages =
      settings.showProgress =
      settings.showResults = false;
    else if ( strcmp(argv[i],"-o") == 0 )
    {
      if ( argc > i )
        settings.output = argv[++i];
    }
    else if ( strcmp(argv[i],"-i") == 0 )
    {
      if ( argc > i )
        settings.input = argv[++i];
    }
    else if ( strcmp(argv[i],"-e") == 0 )
    {
      if ( argc > i )
        settings.minErr = atof(argv[++i]);
    }
    else if ( strcmp(argv[i],"-help") == 0 )
    {
      cout << "Commands are ..." << endl
           << "-daubechies        use daubechies wavelets (default)" << endl
           << "-haar              use haar wavelets" << endl
           << "-hideimages        don't display images    (shown by default)" << endl
           << "-hideprogress      hide progress messages  (shown by default)" << endl
           << "-hideresults       hide results statistics (shown by default)" << endl
           << "-hideall           hide images,progress,results" << endl
           << "-o FILENAME        save output file into file name" << endl
           << "-i FILENAME        load image to compress  (default images/lenna.pgm)" << endl
           << "-e FLOAT           specify max M.S.Error   (default 25.0)" << endl
           << endl;
      exit(0);
    }
  }
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
int removeCoefs( const jdg::Image<pType>& img, jdg::Image<pType>& coefs, double maxErr,
  Wavelet type, bool showProgress )
{
  int height = img.getHeight(),
      width = img.getWidth();
  int min = 0,
      max = height * width - 1;
  int mid=0;

  pType uLeft;

  double meanErr = 0.0;

  // create list of points
  vector<Point<pType> > lst;

  // holds all the coeficients, then all the ones that are put into lst are removed
  jdg::Image<pType> smallest(width,height), test(width,height);

  // resize coefs if required
  if ( coefs.getWidth() != width || coefs.getHeight() != height )
    coefs.pad(width,height);

  if ( type == HAAR )
    jdg::haarTransform( img, smallest );
  else  
    jdg::daubechiesTransform( img, smallest );

  uLeft = smallest(0,0);  // this needs to be in every image
  smallest(0,0) = 0.0;    // make it so that it's not counted when looking for max

  bool less = false;
  
  while ( max >= min )
  {
    if ( !less )
      mid = min+(max-min)/DIVIDER;  // growning slowley because it's faster
    else
      mid = (min+max)/2;

    findLargest( smallest, lst, mid );

    for ( int x = 0; x < width; x++ )
      for ( int y = 0; y < height; y++ )
        coefs(x,y) = 0.0;

    for ( int i = 0; i < mid; i++ )
      coefs(lst[i].x, lst[i].y) = lst[i].val;
    
    coefs(0,0) = uLeft;
      
    if ( type == HAAR )
      jdg::ihaarTransform( coefs, test );
    else  
      jdg::idaubechiesTransform( coefs, test );

    meanErr = mse(test, img);

    if ( showProgress )
      cout << "MSE: " << meanErr
           << "\tUsing " << (100.0*mid)/(width*height) << '%'
           << "\t(" << mid << " pixels)" << endl;
    
    if ( meanErr > maxErr )
      min = mid+1;  // use more coeffs
    else
    {
      max = mid-1;  // use less coeffs
      less = true;
    }
  }

  // in case it converges one to low
  if ( meanErr > maxErr )
  {
    mid++;
    if ( mid > (int)lst.size() )
    {
      findLargest( smallest, lst, mid );
    
      for ( int x = 0; x < width; x++ )
        for ( int y = 0; y < height; y++ )
          coefs(x,y) = 0.0;

      for ( int i = 0; i < mid; i++ )
        coefs(lst[i].x, lst[i].y) = lst[i].val;
      
      coefs(0,0) = uLeft;

    }
    else  // more likely
      coefs(lst[mid-1].x, lst[mid-1].y) = lst[mid-1].val;
  }

  return mid;
}


template <class pType>
void findLargest( jdg::Image<pType>& img, vector<Point<pType> >& lst, int size )
{
  int len;
  if ( (int)lst.size() < size )
  {
    len = (int)lst.size();
    lst.resize(size);
  }
  else  // already sorted
    return;


  Point<pType> current, temp;
  int height = img.getHeight(),
      width = img.getWidth();

  for ( int x = 0; x < width; x++ )
    for ( int y = 0; y < height; y++ )
      if ( img(x,y) != 0 )
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
  // remove values from img
  for ( int i = 0; i < size; i++ )
    img(lst[i].x, lst[i].y) = 0;
}

