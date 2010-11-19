#include <png.h>
#include <cstdio>

int main(int argc, char* argv[])
{
  if ( argc < 2 )
  {
    printf("Enter file name\n");
    return 0;
  }

  FILE *fp = fopen(argv[1],"rb");
  if ( !fp )
  {
    printf("Invalid Filename\n");
    return 0;
  }
  
  // get size
  fseek(fp, 0L, SEEK_END);
  long fs = ftell(fp);
  rewind(fp);
    
  if ( fs < 8 )
  {
    printf("Invalid png (too small)\n");
    return 0;
  }

  // declare the header
  png_byte* header = new png_byte[fs];

  // read the header of the file
  fread(header, sizeof(png_byte), 8, fp);
  
  // check if it is a png
  if ( png_sig_cmp(header,0,8) )  
  {
    printf("Invalid png\n");
    return 0;
  }

  // go back to beginning of file
  rewind(fp);

  // initialize png_struct
  png_structp png_ptr =
    png_create_read_struct(
      PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);

  if ( !png_ptr )
  {
    printf("png_struct did not initialize");
    return 0;
  }

  // initialize png_infop
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if ( !info_ptr )
  {
    printf("png_info did not initialize");
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return 0;
  }

  png_infop end_info = png_create_info_struct(png_ptr);
  if ( !end_info )
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL );
    printf("end_info did not intialize");
    return 0;
  }

  // read the file
  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);
  png_set_compression_buffer_size(png_ptr, 8192);

  png_read_info(png_ptr, info_ptr);
  int bdepth = png_get_bit_depth(png_ptr, info_ptr),
      ctype = png_get_color_type(png_ptr, info_ptr);
   
  if(ctype == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);

  if(ctype == PNG_COLOR_TYPE_GRAY && bdepth < 8)
    png_set_expand_gray_1_2_4_to_8(png_ptr);

  if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr);

  if(bdepth == 16)
    png_set_strip_16(png_ptr);
  else if(bdepth < 8)
    png_set_packing(png_ptr);

  png_read_update_info(png_ptr, info_ptr);

  png_uint_32 width, height;
  png_get_IHDR(png_ptr, info_ptr, &width, &height,
    &bdepth, &ctype, NULL, NULL, NULL);

  // PNG_COLOR_TYPE_GRAY
  // PNG_COLOR_TYPE_GRAY_ALPHA
  // PNG_COLOR_TYPE_RGB
  // PNG_COLOR_TYPE_RGB_ALPHA

  if ( ctype == PNG_COLOR_TYPE_GRAY )
    len = 1;
  else if ( ctype == PNG_COLOR_TYPE_GRAY_ALPHA )
    len = 2;
  else if ( ctype == PNG_COLOR_TYPE_RGB )
    len = 3;
  else if ( ctype == PNG_COLOR_TYPE_RGB_ALPHA )
    len = 4;
  else
  {
    printf("Invalid color type");
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    return 0;
  }


  GLubyte *pixel = new GLubyte[width*height];

  png_bytep* rows = new png_bytep[height];
  for ( int i = 0; i < height; i++ )
    row_pointers[i] = new png_bytep[i*width*
  
  // cleans up the memory
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

  return 0;
}
