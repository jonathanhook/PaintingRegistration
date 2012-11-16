/*
 * Image Loader (BMP, TGA and JPEG)
 * by Daniel Jackson, 2001-2006
 *
 */
#include <stdio.h>

// Comment out below to remove GL-specific functions
#ifndef GL_FUNCS
#define GL_FUNCS 1
#endif

extern void *loadImage(const char *filename, int *outWidth, int *outHeight, bool *outAlphaUsed = NULL, int *outTWidth = NULL, int *outTHeight = NULL, int scaleUp = 0, bool flip = false);

#if (GL_FUNCS == 1)
extern int createTexture(const char *filename);
extern bool setTexture(int id);
#endif

