#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cv.h>
#include <highgui.h>

namespace jdg
{

enum PadWith { ZEROS=0, NEAREST=1 };

template <class pType>
class Image
{
  public:
    ~Image();
    Image();
    Image(const Image<pType>& rhs);
    Image(const std::string& filename);
    Image(int width, int height);
   
    // display the image in a highgui window and pause for input
    void show(const std::string& window = "image") const;

    // re-allocate memory for the image and change _width and _height
    inline void resizeCanvas(int width, int height);

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    const pType* getData() const { return _data; }

    // pad the image
    void pad( int width, int height, const PadWith padding = ZEROS, int xoffset=0, int yoffset=0 );

    // operator overloads
    Image<pType> operator+ (const pType& rhs) const;
    Image<pType> operator- (const pType& rhs) const;
    Image<pType> operator* (const pType& rhs) const;
    Image<pType> operator/ (const pType& rhs) const;
    Image<pType> operator+ (const Image<pType>& rhs) const;
    Image<pType> operator- (const Image<pType>& rhs) const;
    Image<pType> operator* (const Image<pType>& rhs) const;
    Image<pType> operator/ (const Image<pType>& rhs) const;
    Image<pType>& operator+=(const pType& rhs);
    Image<pType>& operator-=(const pType& rhs);
    Image<pType>& operator*=(const pType& rhs);
    Image<pType>& operator/=(const pType& rhs);
    Image<pType>& operator+=(const Image<pType>& rhs);
    Image<pType>& operator-=(const Image<pType>& rhs);
    Image<pType>& operator*=(const Image<pType>& rhs);
    Image<pType>& operator/=(const Image<pType>& rhs);
    
    inline pType& operator()(int x, int y);
    inline pType operator()(int x, int y, const PadWith padding = ZEROS) const;

    bool operator==(const Image<pType>& rhs) const;
    bool operator!=(const Image<pType>& rhs) const { return !((*this)==rhs); }
    Image<pType>& operator=(const Image<pType>& rhs);

    // load or save the image as a pgm image

    // pType must be primitive for load/save to work
    void load(const std::string& filename);
    void save(const std::string& filename) const;

  private:
    int _width;
    int _height;
    pType* _data;
};

template <class pType>
void Image<pType>::pad( int width, int height, const PadWith padding, int xoffset, int yoffset)
{
  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j-xoffset, i-yoffset, padding);

  *this = ret;
}

// display image in grayscale
template <class pType>
void Image<pType>::show(const std::string& window) const
{
  IplImage* img = cvCreateImage( cvSize( _width, _height ), IPL_DEPTH_8U, 1 );

  // create new IplImage
  for ( int i = 0; i < _height;i++ )
    for ( int j = 0; j < _width; j++ )
      cvSetReal2D(img,i,j,static_cast<double>(_data[j+i*_width]));

  cvNamedWindow(window.c_str());
  cvShowImage(window.c_str(),img);
  cvWaitKey();
  cvDestroyWindow(window.c_str());
  cvReleaseImage(&img);
}

template <class pType>
inline void Image<pType>::resizeCanvas(int width, int height)
{
  if ( _data )
    delete [] _data;
  _width = width;
  _height = height;
  _data = new pType[_width*_height];
}

template <class pType>
Image<pType> Image<pType>::operator+ (const pType& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) + rhs;

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator- (const pType& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) - rhs;

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator* (const pType& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) * rhs;

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator/ (const pType& rhs) const
{
  Image<pType> ret(_width, _height);

  for ( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      ret(j, i) = (*this)(j,i) / rhs;

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator+ (const Image<pType>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) + rhs(j,i);

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator- (const Image<pType>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) - rhs(j,i);

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator* (const Image<pType>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) * rhs(j,i);

  return ret;
}

template <class pType>
Image<pType> Image<pType>::operator/ (const Image<pType>& rhs) const
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height,rhs_height),
      width = std::max(_width, rhs_width);

  Image<pType> ret(width, height);

  for ( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      ret(j, i) = (*this)(j,i) / rhs(j,i);

  return ret;
}

template <class pType>
Image<pType>& Image<pType>::operator+=(const pType& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) += rhs;

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator-=(const pType& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) -= rhs;

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator*=(const pType& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) *= rhs;

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator/=(const pType& rhs)
{
  for( int i = 0; i < _height; i++ )
    for ( int j = 0; j < _width; j++ )
      (*this)(j, i) /= rhs;

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator+=(const Image<pType>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) += rhs(j,i);

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator-=(const Image<pType>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) -= rhs(j,i);

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator*=(const Image<pType>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) *= rhs(j,i);

  return *this;
}

template <class pType>
Image<pType>& Image<pType>::operator/=(const Image<pType>& rhs)
{
  int rhs_height = rhs.getHeight(),
      rhs_width = rhs.getWidth();

  // max
  int height = std::max(_height, rhs_height),
      width = std::max(_width, rhs_width);

  for( int i = 0; i < height; i++ )
    for ( int j = 0; j < width; j++ )
      (*this)(j, i) /= rhs(j,i);

  return *this;
}

template <class pType>
bool Image<pType>::operator==(const Image<pType>& rhs) const
{
  if ( rhs.getHeight() == _height && rhs.getWidth() == _width )
  {
    const pType* data = rhs.getData();
    for ( int i = _height*_width-1; i >= 0; --i )
      if (_data[i] != data[i])
        return false;
    return true;
  }
  return false;
}

template <class pType>
Image<pType>& Image<pType>::operator=(const Image<pType>& rhs)
{
  if ( rhs.getHeight() != _height && rhs.getWidth() != _width )
    resizeCanvas(rhs.getWidth(), rhs.getHeight());

  const pType* data = rhs.getData();
  for ( int i = _height*_width-1; i >= 0; --i )
    _data[i] = data[i];

  return *this;
}

template <class pType>
inline pType& Image<pType>::operator()(int x, int y)
{
  return _data[x+y*_width];
}

template <class pType>
inline pType Image<pType>::operator()(int x, int y, const PadWith padding) const
{
  if ( x >= _width || y >= _height || x < 0 || y < 0 )
  {
    if ( padding == ZEROS )
      return static_cast<pType>(0);
    else if ( padding == NEAREST )
    {
      x = std::max(0,x);
      y = std::max(0,y);
      x = std::min(_width-1,x);
      y = std::min(_height-1,y);
    }
  }

  return _data[x+y*_width];
}

// constructors/destructor /////////////////////////////////////////////////////

template <class pType>
Image<pType>::Image(int width, int height) : _width(width), _height(height)
{
  _data = new pType[width*height];
}

template <class pType>
Image<pType>::Image() : _width(0), _height(0), _data(0)
{}

template <class pType>
Image<pType>::Image(const std::string& filename) : _width(0), _height(0), _data(0)
{
  load(filename);
}

template <class pType>
Image<pType>::Image(const Image<pType>& rhs)
{
  _width = rhs.getWidth();
  _height = rhs.getHeight();
  _data = new pType[_width*_height];

  const pType* data = rhs.getData();

  for ( int i = _width*_height-1; i>=0; --i )
    _data[i] = data[i];
}

template <class pType>
Image<pType>::~Image()
{
  if ( _data )
    delete [] _data;
}

// file I/O functions //////////////////////////////////////////////////////////

template <class pType>
void jdg::Image<pType>::load(const std::string& filename)
{
  int i;
  int N, M, Q;
  unsigned char *charImage;
  char header [100], *ptr;
  std::ifstream ifp;

  ifp.open(filename.c_str(), std::ios::in | std::ios::binary);

  if (!ifp) {
    std::cout << "Can't read image: " << filename << std::endl;
    exit(1);
  }

  // read header

  ifp.getline(header,100,'\n');
  if ( (header[0]!=80) ||    // 'P'
      (header[1]!=53) ) {    // '5'
    std::cout << "Image " << filename << " is not PGM" << std::endl;
    exit(1);
  }

  ifp.getline(header,100,'\n');
  while(header[0]=='#')
    ifp.getline(header,100,'\n');

  M=strtol(header,&ptr,0);
  N=atoi(ptr);

  ifp.getline(header,100,'\n');
  Q=strtol(header,&ptr,0);

  charImage = (unsigned char *) new unsigned char [M*N];

  ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

  if (ifp.fail()) {
    std::cout << "Image " << filename << " has wrong size" << std::endl;
    exit(1);
  }

  ifp.close();

  //
  // Convert the unsigned characters to integers
  //

  _width = M;
  _height = N;
  
  resizeCanvas(M,N);

  for(i=M*N-1; i>=0; --i)
    _data[i] = static_cast<pType>(charImage[i]);

  delete [] charImage;
}

template <class pType>
void jdg::Image<pType>::save(const std::string& filename) const
{
  int i;
  unsigned char *charImage;
  std::ofstream ofp;

  charImage = new unsigned char[_width*_height];

  // convert the integer values to unsigned char

  for(i=_height*_width-1; i>=0; --i)
    charImage[i]=static_cast<unsigned char>(_data[i]);

  ofp.open(filename.c_str(), std::ios::out | std::ios::binary);

  if (!ofp) {
    std::cout << "Can't open file: " << filename << std::endl;
    exit(1);
  }

  ofp << "P5" << std::endl;
  ofp << _width << " " << _height << std::endl;
  ofp << 255 << std::endl;

  ofp.write( reinterpret_cast<char *>(charImage), (_width*_height)*sizeof(unsigned char));

  if (ofp.fail()) {
    std::cout << "Can't write image " << filename << std::endl;
    exit(0);
  }

  ofp.close();

  delete [] charImage;

}

} // namespace jdg

