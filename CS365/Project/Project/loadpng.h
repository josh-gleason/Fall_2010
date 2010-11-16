// Complements of 

#ifndef PNGLOADEROPENGL
#define PNGLOADEROPENGL

#include <cstdio>
#include <cstdlib>
#include <png.h>
#include <GL/gl.h>

int GetTextureInfo(int ColourType);
GLuint loadImage(const char *filename);

#endif
