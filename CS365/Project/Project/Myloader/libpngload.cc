#include <png.h>
#include <cstdio>

int main(int argc, char* argv[])
{
  if ( argc < 2 )
    printf("Enter file name\n");

  FILE *fp = fopen(argv[1],"rb");
  if ( !fp )
    printf("Invalid Filename\n");
  
  // declare the header
  png_byte header[8]; 

  // read the header of the file
  fread(header, sizeof(png_byte), 8, fp);
  
  // check if it is a png
  if ( png_sig_cmp(header,0,8) )  
    printf("Invalid png\n");

  // go back to beginning of file
  rewind(fp);

  png_byte image = 

  return 0;
}
