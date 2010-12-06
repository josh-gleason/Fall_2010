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
  int pixels;
  double percentage;
  Wavelet type;
  bool showImages;
  bool showProgress;
  bool showResults;
};

template <class pType>
struct Filters
{
  Filters() : D2(2), D4(4), D6(6), D8(8), D10(10)
  {
    D2[0] = 0.707106781,
    D2[1] = 0.707106781;

    D4[0] = 0.482962913,
    D4[1] = 0.836516304,
    D4[2] = 0.224143868,
    D4[3] = -0.129409523;

    D6[0] = 0.332670555,
    D6[1] = 0.806891512,
    D6[2] = 0.459877502,
    D6[3] = -0.135011023,
    D6[4] = -0.085441275,
    D6[5] = 0.035226292;
    
    D8[0] = 0.230377814686837;
    D8[1] = 0.714846574023517;
    D8[2] = 0.630881660124131;
    D8[3] = -0.0279837720786611;
    D8[4] = -0.187034813579463;
    D8[5] = 0.0308413815002668;
    D8[6] = 0.0328830109095867;
    D8[7] = -0.010597402258575;

    D10[0] = 0.160102396147341;
    D10[1] = 0.603829267884446;
    D10[2] = 0.724308525426066;
    D10[3] = 0.138428144948276;
    D10[4] = -0.242294883260072;
    D10[5] = -0.0322448682527693;
    D10[6] = 0.0775714877291344;
    D10[7] = -0.00624149013617742;
    D10[8] = -0.0125807519269895;
    D10[9] = 0.00333572527906182;
  }

  vector<pType> D2,D4,D6,D8,D10;
};

const int DIVIDER = 17;

// define the filters
const Filters<double> FILTERS;

// find the 'size' largest points in the image and store in 'lst' which should
// be pre-initialized to 'size' elements, ignores point (0,0)
template <class pType>
void findLargest( jdg::Image<pType>& img, vector<Point<pType> >& lst, int size );

template <class pType>
double mse( const jdg::Image<pType>& img1, const jdg::Image<pType>& img2 );

void readSettings( int argc, char* argv[], Settings& settings );

template <class pType>
int removeCoefs( const jdg::Image<pType>& img, jdg::Image<pType>& coefs, double maxErr, Wavelet type, bool showProgress );

template <class pType>
void removeExactCoefs( const jdg::Image<pType>& img, jdg::Image<pType>& coefs, int pixels,
  Wavelet type );

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
  if ( settings.pixels > 0 )
    removeExactCoefs( img, coefs, settings.pixels, settings.type );
  else if ( settings.percentage > 0.0 )
  {
    settings.pixels = settings.percentage*(img.getHeight()*img.getWidth()-1.0);
    removeExactCoefs( img, coefs, settings.pixels, settings.type );
  }
  else // remove just enough to get the minimum desired error
    settings.pixels =
      removeCoefs( img, coefs, settings.minErr, settings.type, settings.showProgress );

  // reconstruct
  if ( settings.type == DAUBECHIES )
    jdg::iwaveletTrans(coefs,recons, FILTERS.D10 );
  else if ( settings.type == HAAR )
    jdg::iwaveletTrans(coefs,recons, FILTERS.D2 );

  // save results
  if ( settings.output != "" )
    recons.save(settings.output);

  // display statistical results
  if ( settings.showResults )
    cout << "\nFinal Results" << endl
         << "MSE: " << mse(img, recons)
         << "\tUsing " << (100.0*settings.pixels)/(img.getWidth()*img.getHeight()-1.0) << '%'
         << "\t(" << settings.pixels << " coefficients)" << endl;
  
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
  settings.pixels = -1;
  settings.percentage = -1.0;
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
    else if ( strcmp(argv[i],"-error") == 0 )
    {
      if ( argc > i )
        settings.minErr = atof(argv[++i]);
    }
    else if ( strcmp(argv[i],"-pixels") == 0 )
    {
      if ( argc > i )
        settings.pixels = atoi(argv[++i]);
    }
    else if ( strcmp(argv[i],"-percentage") == 0 )
    {
      if ( argc > i )
        settings.percentage = atof(argv[++i])/100.0;
    }
    else if ( strcmp(argv[i],"-help") == 0 )
    {
      cout << "Commands are ..." << endl
           << "\t-hideimages        don't display images    (shown by default)" << endl
           << "\t-hideprogress      hide progress messages  (shown by default)" << endl
           << "\t-hideresults       hide results statistics (shown by default)" << endl
           << "\t-hideall           hide images,progress,results" << endl
           << "\t-o FILENAME        save output file into file name" << endl
           << "\t-i FILENAME        load image to compress  (default images/lenna.pgm)\n"
           << "\nUse only one of the following (default -error 25.0)" << endl
           << "\t-error FLOAT       specify max M.S.Error" << endl
           << "\t-pixels INT        specify exact number of coeficients" << endl
           << "\t-percentage FLOAT  specify exact percentage of pixel coeficients" << endl
           << "\nUse only one of the following (default -daubechies)" << endl
           << "\t-daubechies        use daubechies wavelets (default)" << endl
           << "\t-haar              use haar wavelets" << endl
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
void removeExactCoefs( const jdg::Image<pType>& img, jdg::Image<pType>& coefs, int pixels,
  Wavelet type )
{
  int height = img.getHeight(),
      width = img.getWidth();
  
  // create list of points
  vector<Point<pType> > lst;

  // holds all the coeficients, then all the ones that are put into lst are removed
  jdg::Image<pType> smallest(width,height);

  // resize coefs if required
  if ( coefs.getWidth() != width || coefs.getHeight() != height )
    coefs.pad(width,height);

  // wavelet transform
  if ( type == HAAR )
    jdg::waveletTrans( img, coefs, FILTERS.D2 );
  else
    jdg::waveletTrans( img, coefs, FILTERS.D10 );
    
  if ( pixels >= height*width-1 )
    return; // don't need to remove any :p

  // remove the top left because it is not a coefficient
  pType uLeft = coefs(0,0);
  coefs(0,0) = 0;

  // find the largest pixels
  findLargest( coefs, lst, pixels );

  // clear the image and put only the largest pixels in
  for ( int x = 0; x < width; x++ )
    for ( int y = 0; y < height; y++ )
      coefs(x,y) = 0.0;

  for ( int i = 0; i < pixels; i++ )
    coefs(lst[i].x, lst[i].y) = lst[i].val;

  // don't forget this one!
  coefs(0,0) = uLeft;
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
    jdg::waveletTrans( img, smallest, FILTERS.D2 );
  else
    jdg::waveletTrans( img, smallest, FILTERS.D10 );

  uLeft = smallest(0,0);  // this needs to be in every image
  smallest(0,0) = 0.0;    // make it so that it's not counted when looking for max

  bool less = false;
  
  while ( max >= min )
  {
    if ( !less )
      mid = min+(max-min)/DIVIDER;  // growing slowley because it's faster :P
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
      jdg::iwaveletTrans( coefs, test, FILTERS.D2 );
    else  
      jdg::iwaveletTrans(coefs,test, FILTERS.D10 );

    meanErr = mse(test, img);

    if ( showProgress )
      cout << "MSE: " << meanErr
           << "\tUsing " << (100.0*mid)/(width*height-1.0) << '%'
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

