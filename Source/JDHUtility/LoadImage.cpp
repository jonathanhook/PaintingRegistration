/*
 * Image Loader (BMP, TGA and JPEG)
 * by Daniel Jackson, 2001-2006
 *
 */

#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <JDHUtility/OpenGL.h>
#include "LoadImage.h"


// extern void *loadImage(const char *filename, int *outWidth, int *outHeight, bool *outAlphaUsed = NULL, int *outTWidth = NULL, int *outTHeight = NULL, bool scaleUp = false);

// Use printf for debugging output
#define dprintf printf

// Byte-order independent
#define BMP_READ_BYTE(C)  *c++
#define BMP_READ_SHORT(C) (unsigned short)*c + (((unsigned short)*(c+1))<<8); c += 2
#define BMP_READ_LONG(C)  (int)*c + (((int)*(c+1))<<8) + (((int)*(c+2))<<16) + (((int)*(c+3))<<24); c += 4


typedef struct {
  unsigned short bfType;
  int bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  int bfOffBits;
} bmp_fileheader;

typedef struct {
  int biSize;
  int biWidth;
  int biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  int biCompression;
  int biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  int biClrUsed;
  int biClrImportant;
} bmp_infoheader;



bool bmpCanHandleBuffer(void *buffer, int length) {
  // LATER: Improve checking for supported types
  if (length >= 2 && ((unsigned char*)buffer)[0] == 'B' && ((unsigned char*)buffer)[1] == 'M' ) {
    return true;
  }
  return false;
}


void *bmpReadRGBA(void *buffer, int length, int *width, int *height, bool *alphaUsed) {
  if (buffer == NULL || length == 0) { return NULL; };
  unsigned char *c = (unsigned char *)buffer;
  bool ret = false;
  bmp_fileheader fileheader;
  bmp_infoheader infoheader;
  int i;

  unsigned char *output;
  bool flip = true;
  if (alphaUsed != NULL) { *alphaUsed = false; }


  fileheader.bfType = BMP_READ_SHORT(c);
  fileheader.bfSize = BMP_READ_LONG(c);
  fileheader.bfReserved1 = BMP_READ_SHORT(c);
  fileheader.bfReserved2 = BMP_READ_SHORT(c);
  fileheader.bfOffBits = BMP_READ_LONG(c);

  infoheader.biSize = BMP_READ_LONG(c);
  infoheader.biWidth = BMP_READ_LONG(c);
  infoheader.biHeight = BMP_READ_LONG(c);
  infoheader.biPlanes = BMP_READ_SHORT(c);
  infoheader.biBitCount = BMP_READ_SHORT(c);
  infoheader.biCompression = BMP_READ_LONG(c);  // 0=None, 1=RLE8 (8-bit), 2=RLE4 (4-bit), 3=Bitfields (16-bit/32-bit)
  infoheader.biSizeImage = BMP_READ_LONG(c);
  infoheader.biXPelsPerMeter = BMP_READ_LONG(c);
  infoheader.biYPelsPerMeter = BMP_READ_LONG(c);
  infoheader.biClrUsed = BMP_READ_LONG(c);
  infoheader.biClrImportant = BMP_READ_LONG(c);

  // If height is negative then won't need to flip
  int sheight = (int)infoheader.biHeight;
  if (sheight < 0) {
    sheight = -sheight;
    infoheader.biHeight = (int)sheight;
    flip = false;
  }


  if (((unsigned char *)&fileheader.bfType)[0] != 'B' || ((unsigned char *)&fileheader.bfType)[1] != 'M') {
    dprintf("BMP-Error: Unrecognised file type\n");
  }

  // Seek to start of palette
  c += infoheader.biSize - 40;

  int palette[256];
  for (i = 0; i < 255; i++) {
    palette[i] = 0x00000000;
  }
  int palEntries = 0;
  if (infoheader.biBitCount <= 8) {
    unsigned char *p = (unsigned char *)palette;
    palEntries = infoheader.biClrUsed;
    if (palEntries == 0) {
      palEntries = (1<<infoheader.biBitCount);
    }
    if (palEntries > 256) {
      palEntries = 256;
    }
    unsigned char r, g, b, a = 255;
    for (int i = 0; i < palEntries; i++) {
      b = *c++; g = *c++; r = *c++; 
      if (infoheader.biSize > 12) { c++; }  // 4-byte entries
      *p++ = r; *p++ = g; *p++ = b; *p++ = a; 
    }
  }

  // Calculate the padding for uncompressed bitmaps
  int widthbytes = (((infoheader.biWidth-1) * infoheader.biBitCount) / 8) + ((infoheader.biBitCount-1) / 8 + 1);
  int padding = 3-((widthbytes-1)&3);


  // Seek to start of bitmap data
  c = (unsigned char *)buffer + fileheader.bfOffBits;

  dprintf("BMP: %dx%d\n", infoheader.biWidth, infoheader.biHeight);

  int outputsize = 4*infoheader.biWidth*infoheader.biHeight;
  output = new unsigned char[outputsize];
  memset(output, 0, outputsize);

  // only support one plane
  if (infoheader.biPlanes != 1) {
    dprintf("BMP-Error: Unsupported %d planes (only support 1)\n", infoheader.biPlanes);
  } else {

    if (infoheader.biBitCount == 1 && infoheader.biCompression == 0) {
      // 1-bit uncompressed
      int *o = (int *)output;
      for (int y = 0; y < infoheader.biHeight; y++) {
        for (int x = 0; x < infoheader.biWidth; ) {
          unsigned char d = *c++;
          unsigned char m = 128;
          for (int b = 0; b < 8; b++) {
            if (x < infoheader.biWidth) {
              *o++ = palette[(d&m)?1:0];
            }
            x++;
            m >>= 1;
          }
        }
        c += padding;
      }
      ret = true;
    } else if (infoheader.biBitCount == 4) {
      if (infoheader.biCompression == 0) {
        // 4-bit uncompressed
        int *o = (int *)output;
        for (int y = 0; y < infoheader.biHeight; y++) {
          for (int x = 0; x < infoheader.biWidth; x++) {
            unsigned char d = *c++;
            *o++ = palette[(d>>4)];
            if (++x < infoheader.biWidth) {
              *o++ = palette[d&0x0f];
            }
          }
          c += padding;
        }
        ret = true;
      } else if (infoheader.biCompression == 2) {
        // 4-bit compressed
        int x = 0, y = 0, w = infoheader.biWidth;
        unsigned char *end = c + infoheader.biSizeImage;
        int *o = (int *)output;
        while (c < end) {
          unsigned char marker = *c++;
          if (marker > 0) {             // RLE encoded run
            unsigned char d = *c++;
            int e = palette[d>>4];
            int f = palette[d&0xf];
            int length = marker;
            // Don't allow over-run
            if (x + length > w) { length = w - x; }
            for (int i = 0; i < length; i++) {
              *o++ = e;
              if (++i < length) *o++ = f;
            }
            x += length;
          } else {                      // escape marker
            marker = *c++;
            if (marker == 0) {          // end of scan-line marker
              // skip to start of next line
              y++; x = 0;
              o = (int *)output + (y * w) + x;
            } else if (marker == 1) {   // end of bitmap data marker
              break;
            } else if (marker == 2) {   // run-offset marker
              x += *c++; 
              y += *c++;
              o = (int *)output + (y * w) + x;
            } else {                    // Unencoded run
              int length = marker;
              // Don't allow over-run
              if (x + length > w) { length = w - x; }
              for (int i = 0; i < length; i++) {
                unsigned char d = *c++;
                *o++ = palette[(d>>4)];
                if (++i < length) { *o++ = palette[d&0x0f]; }
              }
              // Skip possible padding byte (should probably skip any over-run but shouldn't exist)
              c += (((marker+1)/2) & 1);
              x += length;
            }
          }
        }
        ret = true;
      } else {
        dprintf("BMP-Error: Unknown compression %d not supported for 4-bit BMPs.\n", infoheader.biCompression);
      }
    } else if (infoheader.biBitCount == 8) {
      if (infoheader.biCompression == 0) {
        // 8-bit uncompressed
        int *o = (int *)output;
        for (int y = 0; y < infoheader.biHeight; y++) {
          for (int x = 0; x < infoheader.biWidth; x++) {
            *o++ = palette[*c++];
          }
          c += padding;
        }
        ret = true;
      } else if (infoheader.biCompression == 1) {
        // 8-bit compressed
        int x = 0, y = 0, w = infoheader.biWidth;
        unsigned char *end = c + infoheader.biSizeImage;
        int *o = (int *)output;
        while (c < end) {
          unsigned char marker = *c++;
          if (marker > 0) {             // RLE encoded run
            int d = palette[*c++]; // colour
            int length = marker;
            // Don't allow over-run
            if (x + length > w) { length = w - x; }
            for (int i = 0; i < length; i++) {
              *o++ = d;
            }
            x += length;
          } else {                      // escape marker
            marker = *c++;
            if (marker == 0) {          // end of scan-line marker
              // skip to start of next line
              y++; x = 0;
              o = (int *)output + (y * w) + x;
            } else if (marker == 1) {   // end of bitmap data marker
              break;
            } else if (marker == 2) {   // run-offset marker
              x += *c++; 
              y += *c++;
              o = (int *)output + (y * w) + x;
            } else {                    // Unencoded run
              int length = marker;
              // Don't allow over-run
              if (x + length > w) { length = w - x; }
              for (int i = 0; i < length; i++) {
                *o++ = palette[*c++];
              }
              // Skip over-run and possible padding byte
              c += (marker - length) + (marker & 1);
              x += length;
            }
          }
        }
        ret = true;
      } else {
        dprintf("BMP-Error: Unknown compression %d not supported for 8-bit BMPs.\n", infoheader.biCompression);
      }
    } else if ((infoheader.biCompression == 3 || infoheader.biCompression == 0) && infoheader.biBitCount == 16) {
      dprintf("BMP-Error: 16-bit BMPs not supported (use 24-bit).\n");
      // LATER: Should check mask values (either RGB555 or RGB565)
    } else if (infoheader.biCompression == 0 && infoheader.biBitCount == 24) {
      // 24-bit uncompressed
      unsigned char r, g, b, a = 255;
      unsigned char *o = output;
      for (int y = 0; y < infoheader.biHeight; y++) {
        for (int x = 0; x < infoheader.biWidth; x++) {
          b = *c++; g = *c++; r = *c++;
          *o++ = r; *o++ = g; *o++ = b; *o++ = a;
        }
        c += padding;
      }
      ret = true;

    } else if ((infoheader.biCompression == 3 || infoheader.biCompression == 0) && infoheader.biBitCount == 32) {
      // 32-bit uncompressed (unusual, alpha still not supported)
      // HACK: Should look at mask values (this assumes BGRA8888)
      unsigned char r, g, b, a = 255;
      unsigned char *o = output;
      for (int y = 0; y < infoheader.biHeight; y++) {
        for (int x = 0; x < infoheader.biWidth; x++) {
          b = *c++; g = *c++; r = *c++; a = *c++; // Invert the alpha?
          *o++ = r; *o++ = g; *o++ = b; *o++ = a;
        }
        c += padding;
      }
      if (alphaUsed != NULL) { *alphaUsed = true; }
      ret = true;
    } else {
      dprintf("BMP-Error: Unsupported format at bit-depth %d and compression 0x%x.\n", infoheader.biBitCount, infoheader.biCompression);
    }
  }

  if (ret) {
    if (flip) {
      // Flip vertically
      int *sp = (int *)output;
      int *dp = sp + infoheader.biWidth*(infoheader.biHeight-1);
      for (int y = 0; y < infoheader.biHeight/2; y++, dp -= 2*infoheader.biWidth) {
        for (int x = 0; x < infoheader.biWidth; x++) { int t = *sp; *sp++ = *dp; *dp++ = t; }
      }
    }
    *width = infoheader.biWidth;
    *height = infoheader.biHeight;
    return output;
  }

  return NULL;
}





// Byte-order independent
#define TGA_READ_BYTE(C)  *c++
#define TGA_READ_SHORT(C) (unsigned short)*c + (((unsigned short)*(c+1))<<8); c += 2
#define TGA_READ_LONG(C)  (int)*c + (((int)*(c+1))<<8) + (((int)*(c+2))<<16) + (((int)*(c+3))<<24); c += 4

//  byte  id_length  // size of ID field that follows 18 byte header (0 usually)
//  byte  cmap_type  // type of colour map 0=none, 1=has palette
//  byte  image_type // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

// id_length cmap_type image_type
// ??        0         2
// ??        0         10

typedef struct {
  unsigned char id_length;
  unsigned char cmap_type;
  unsigned char image_type;
  unsigned char cmap_spec[5];
  unsigned short xorigin;
  unsigned short yorigin;
  unsigned short imwidth;
  unsigned short imheight;
  unsigned char pixdepth;
  unsigned char imdescrip;
} tga_header;


bool tgaCanHandleBuffer(void *buffer, int length) {
  // LATER: Improve checking for supported types
  if (length >= 3 && ((unsigned char*)buffer)[1] == 0x00 && ((((unsigned char*)buffer)[2] == 0x02) || (((unsigned char*)buffer)[2] == 0x0A))) {
    return true;
  }
  return false;
}

void *tgaReadRGBA(void *buffer, int length, int *width, int *height, bool *alphaUsed) {
  if (buffer == NULL || length == 0) { return NULL; };
  bool ret = true;
  unsigned char *c = (unsigned char *)buffer;
  tga_header tgh;
  unsigned char *pix;
  int i;
  
  // Painful read so that it's structure packing and byte-order independent
  tgh.id_length = TGA_READ_BYTE(c);
  tgh.cmap_type = TGA_READ_BYTE(c);
  tgh.image_type = TGA_READ_BYTE(c);
  tgh.cmap_spec[0] = TGA_READ_BYTE(c);
  tgh.cmap_spec[1] = TGA_READ_BYTE(c);
  tgh.cmap_spec[2] = TGA_READ_BYTE(c);
  tgh.cmap_spec[3] = TGA_READ_BYTE(c);
  tgh.cmap_spec[4] = TGA_READ_BYTE(c);
  tgh.xorigin = TGA_READ_SHORT(c);
  tgh.yorigin = TGA_READ_SHORT(c);
  tgh.imwidth = TGA_READ_SHORT(c);
  tgh.imheight = TGA_READ_SHORT(c);
  tgh.pixdepth = TGA_READ_BYTE(c);
  tgh.imdescrip = TGA_READ_BYTE(c);

  c += tgh.id_length;

  pix = new unsigned char[sizeof(unsigned char)*4*tgh.imwidth*tgh.imheight];

  dprintf("TGA: x-orig=%d y-orig=%d width=%d height=%d image-type=%d cmap-type=%d depth=%d\n", tgh.xorigin, tgh.yorigin, tgh.imwidth, tgh.imheight, tgh.image_type, tgh.cmap_type, tgh.pixdepth);

  if (tgh.xorigin != 0 || tgh.yorigin != 0) {
    dprintf("TGA-ERROR: X- or Y-origin is not zero -- this won't work.\n");
    return NULL;
  }

  int type = (tgh.image_type & 0x7);
  bool rle = ((tgh.image_type & 0x8) != 0);

  if (type == 2 && tgh.cmap_type == 0)
  {
    if (!rle) {
      if (tgh.pixdepth == 24) { 
        if (alphaUsed != NULL) { *alphaUsed = false; }
        int len = (tgh.imwidth)*(tgh.imheight);
        int s = 0, d = 0;
        for (i = 0; i < len; i++) {
          pix[d+3] = 255;
          pix[d+2] = c[s];
          pix[d+1] = c[s+1];
          pix[d+0] = c[s+2];
          s += 3;
          d += 4;
        }
      } else if (tgh.pixdepth == 32) {
        if (alphaUsed != NULL) { *alphaUsed = true; }
        int len = (tgh.imwidth)*(tgh.imheight);
        int d = 0;
        for (i = 0; i < len; i++) {
          pix[d+2] = c[d];
          pix[d+1] = c[d+1];
          pix[d+0] = c[d+2];
          pix[d+3] = c[d+3];
          d += 4;
        }
      } else {
        dprintf("TGA-Error: Unsupported rgb uncompressed depth of %d\n", tgh.pixdepth);
        ret = false;
      }
    } else {
      if (tgh.pixdepth == 24) { 
        if (alphaUsed != NULL) { *alphaUsed = false; }
        int size = 4*(tgh.imwidth*tgh.imheight);
        int num = 0;
        int d = 0;
        unsigned char c1, c2, c3;
        while (d < size) {
          unsigned char ph = *c++;
          if (ph > 127) {
            // Compressed packet
            num = ph - 127;
            c1 = *c++; c2 = *c++; c3 = *c++; 
            while (num > 0) {
              pix[d+3] = 255;
              pix[d+2] = c1;
              pix[d+1] = c2;
              pix[d+0] = c3;
              d += 4;
              num--;
            }
          } else {
            // Uncompressed packet
            num = ph + 1;
            while (num > 0) {
              pix[d+3] = 255;
              pix[d+2] = *c++;
              pix[d+1] = *c++;
              pix[d+0] = *c++;
              d += 4;
              num--;
            }
          }
        }
      } else if (tgh.pixdepth == 32) {  
        if (alphaUsed != NULL) { *alphaUsed = true; }
        int size = 4*(tgh.imwidth*tgh.imheight);
        int num = 0;
        int d = 0;
        unsigned char c1, c2, c3, c4;
        while (d < size) {
          unsigned char ph = *c++;
          if (ph > 127) {
            // Compressed packet
            num = ph - 127;
            c1 = *c++; c2 = *c++; c3 = *c++; c4 = *c++;
            while (num > 0) {
              pix[d+2] = c1;
              pix[d+1] = c2;
              pix[d+0] = c3;
              pix[d+3] = c4;
              d += 4;
              num--;
            }
          } else {
            // Uncompressed packet
            num = ph + 1;
            while (num > 0) {
              pix[d+2] = *c++;
              pix[d+1] = *c++;
              pix[d+0] = *c++;
              pix[d+3] = *c++;
              d += 4;
              num--;
            }
          }
        }
      } else {
        dprintf("TGA-Error: Unsupported rgb rle depth of %d\n", tgh.pixdepth);
        ret = false;
      }
    }
  } else {
    dprintf("TGA-Error: Unsupported image type %d/cmap-type %d combination\n", tgh.image_type, tgh.cmap_type);
    ret = false;
  }

  if (ret) {
    // Flip vertically
    int *sp = (int *)pix;
    int *dp = sp + tgh.imwidth*(tgh.imheight-1);
    for (int y = 0; y < tgh.imheight/2; y++, dp -= 2*tgh.imwidth) {
      for (int x = 0; x < tgh.imwidth; x++) { int t = *sp; *sp++ = *dp; *dp++ = t; }
    }
    *width = tgh.imwidth;
    *height = tgh.imheight;
    return pix;
  }

  return NULL;
}









#define jpgClamp(_a) ((_a) >= 0 ? ((_a) <= 255 ? (_a) : 255) : 0)

typedef unsigned short qTable[64];

typedef struct
{
  struct tables {
    unsigned char size;
    unsigned char code;
  } small[512], large[65536];
} HuffTable;

typedef struct
{
  HuffTable *huffAC, *huffDC;
  qTable *qTab;
  int dcPrev,smpx, smpy;
  float t[256];
} ComponentTable;

ComponentTable component[4];
HuffTable huffTableAC[4], huffTableDC[4];
qTable qtable[4];

int xblock, yblock, blockx, blocky, bsize, restartInt, bfree;
int dt;
unsigned char *data, *bpos, *dend, eof, ssStart, ssEnd, sbits, prec, ncomp;
float dctt[64];
int comp;
unsigned char *jpgBuff;
int jpgWidth, jpgHeight;

int zigzag[64]=
{
  0,	1,	8,	16,	9,	2,	3,	10,
    17,	24,	32,	25,	18,	11,	4,	5,
    12,	19,	26,	33,	40,	48,	41,	34,
    27,	20,	13,	6,	7,	14,	21,	28,
    35,	42,	49,	56,	57,	50,	43,	36,
    29,	22,	15,	23,	30,	37,	44,	51,
    58,	59,	52,	45,	38,	31,	39,	46,
    53,	60,	61,	54,	47,	55,	62,	63
};


unsigned char jpgGetByte(void)
{
  if (bpos >= dend) {
    eof = 1;
    return 0;
  } else {
    return *bpos++;
  }
}

void jpgStreamSkip(int n)
{
  unsigned char a, b;
  bfree += n;
  dt <<= n;
  while (bfree >= 8) {
    bfree -= 8;
    b = jpgGetByte();
    if (b == 0xff) { a = jpgGetByte(); }
    dt |= (b << bfree);
  }
}

int jpgHuffDec(HuffTable *h)
{
  int id, n, c;
  id = (dt >> 23);
  n = h->small[id].size;
  c = h->small[id].code;
  if (n == 0xff) {
    id = (dt >> 16);
    n = h->large[id].size;
    c = h->large[id].code;
  }
  jpgStreamSkip(n);
  return c;
}

int jpgWordDec(int n)
{
  int w;
  int s;
  if (n == 0) { return 0; }
  s = (dt >> 31);
  w = (dt >> (32 - n));
  jpgStreamSkip(n);
  if (s == 0) { w = (w | (0xffffffff << n)) + 1; }
  return w;
}

bool jpgGetInfo()
{
  unsigned char cn, sf, qt;
  int i;

  prec = jpgGetByte();
  if (prec != 8) { dprintf("JPG: Unsupported precision.\n"); return false; }
  jpgHeight = ((jpgGetByte()<<8)+jpgGetByte());
  jpgWidth = ((jpgGetByte()<<8)+jpgGetByte());
  ncomp = jpgGetByte();

  dprintf("JPG: (%d x %d x %d)\n", jpgWidth, jpgHeight, ncomp);

  if ((ncomp != 3) && (ncomp != 1)) { dprintf("JPG: Unsupported number of components.\n"); return false; }

  for (i = 0; i < ncomp; i++) {
    cn = jpgGetByte();
    sf = jpgGetByte();
    qt = jpgGetByte();

    component[cn-1].qTab = &qtable[qt];
    component[cn-1].smpy = (sf & 15);
    component[cn-1].smpx = ((sf >> 4) & 15);
  }

  blockx = (component[0].smpx == 1) ? 8 : 16;
  blocky = (component[0].smpy == 1) ? 8 : 16;

  xblock = jpgWidth / blockx;
  if ((jpgWidth & (blockx - 1)) != 0) { xblock++; }

  yblock = jpgHeight / blocky;
  if ((jpgHeight & (blocky - 1)) !=0) { yblock++; }

  return true;
}

void jpgDecodeHuffTable(int len)
{
  int length[257], i, j, n, code, codelen, delta, rcode, cd, rng;
  unsigned char lengths[16], b, symbol[257];
  HuffTable *h;

  len -= 2;

  while (len > 0) {
    b = jpgGetByte();
    len--;
    h = &huffTableDC[0];

    switch(b){
      case 0:
        h = &huffTableDC[0];
        break;

      case 1:
        h = &huffTableDC[1];
        break;

      case 16:
        h = &huffTableAC[0];
        break;

      case 17:
        h = &huffTableAC[1];
        break;
    }

    for(i = 0; i < 16; i++) {  lengths[i] = jpgGetByte(); }

    len -= 16;
    n = 0;

    for (i = 0; i < 16; i++) {
      len -= lengths[i];
      for(j = 0; j < lengths[i]; j++) {
        symbol[n] = jpgGetByte();
        length[n++] = i + 1;
      }
    }

    code = 0;
    codelen = length[0];

    for(i = 0; i < n; i++) {
      rcode = code << (16 - codelen);
      cd = rcode >> 7;

      if (codelen <= 9){
        rng = 1 << (9 - codelen);
        for (j = cd; j < cd + rng; j++) {
          h->small[j].code = (unsigned char)symbol[i];
          h->small[j].size = (unsigned char)codelen;
        }
      } else{
        h->small[cd].size=(unsigned char)255;
        rng = 1 << (16 - codelen);
        for(j = rcode; j < rcode + rng; j++){
          h->large[j].code = (unsigned char)symbol[i];
          h->large[j].size = (unsigned char)codelen;
        }
      }

      code++;
      delta = length[i+1] - codelen;
      code <<= delta;
      codelen += delta;
    }
  }
}

void jpgFidct(void)
{
  static float a = 0.353553385f, b = 0.490392625f, c = 0.415734798f, d = 0.277785122f, e = 0.097545162f, f = 0.461939752f, g = 0.191341713f;
  static float cd = 0.6935199499f, be = 0.5879377723f, bc = 0.9061274529f, de = 0.3753302693f;
  float a0, f2, g2, a4, f6, g6, s0, s1, s2, s3;
  float t0, t1, t2, t3, m0, m1, m2, m3;
  float h0, h1, h2, h3, r0, r1, r2, r3, w;
  int i;

  for (i = 0; i < 64; i += 8) {
    if ((dctt[i+1] != 0) || (dctt[i+2] != 0) || (dctt[i+3] != 0) || (dctt[i+4] != 0) || (dctt[i+5] != 0) || (dctt[i+6] != 0) || (dctt[i+7] != 0))
    {
      a0 = a*dctt[i];
      f2 = f*dctt[i+2];
      g2 = g*dctt[i+2];
      a4 = a*dctt[i+4];
      g6 = g*dctt[i+6];
      f6 = f*dctt[i+6];
      m0 = a0+a4;
      m1 = a0-a4;
      m2 = f2+g6;
      m3 = g2-f6;
      s0 = m0+m2;
      s1 = m1+m3;
      s2 = m1-m3;
      s3 = m0-m2;
      h2 = dctt[i+7]+dctt[i+1];
      h3 = dctt[i+7]-dctt[i+1];
      r2 = dctt[i+3]+dctt[i+5];
      r3 = dctt[i+3]-dctt[i+5];
      h0 = cd*dctt[i+1];
      h1 = be*dctt[i+1];
      r0 = be*dctt[i+5];
      r1 = cd*dctt[i+3];
      w = de*r3;
      t0 = h1+r1+e*(h3+r3)-w;
      t1 = h0-r0-d*(h2-r3)-w;
      w = bc*r2;
      t2 = h0+r0+c*(h3+r2)-w;
      t3 = h1-r1-b*(h2+r2)+w;
      dctt[i] = s0+t0;
      dctt[i+1] = s1+t1;
      dctt[i+2] = s2+t2;
      dctt[i+3] = s3+t3;
      dctt[i+4] = s3-t3;
      dctt[i+5] = s2-t2;
      dctt[i+6] = s1-t1;
      dctt[i+7] = s0-t0;
    } else {
      a0 = dctt[i] * a;
      dctt[i] = dctt[i+1] = dctt[i+2] = dctt[i+3] = dctt[i+4] = dctt[i+5] = dctt[i+6] = dctt[i+7] = a0;
    }
  }

  for (i = 0; i < 8; i++) {
    if ((dctt[8+i] != 0) || (dctt[16+i] != 0) || (dctt[24+i] != 0) || (dctt[32+i] != 0) || (dctt[40+i] != 0) || (dctt[48+i] != 0) || (dctt[56+i] != 0))
    {
      a0 = a*dctt[i];
      f2 = f*dctt[16+i];
      g2 = g*dctt[16+i];
      a4 = a*dctt[32+i];
      g6 = g*dctt[48+i];
      f6 = f*dctt[48+i];
      m0 = a0+a4;
      m1 = a0-a4;
      m2 = f2+g6;
      m3 = g2-f6;
      s0 = m0+m2;
      s1 = m1+m3;
      s2 = m1-m3;
      s3 = m0-m2;
      h2 = dctt[56+i]+dctt[8+i];
      h3 = dctt[56+i]-dctt[8+i];
      r2 = dctt[24+i]+dctt[40+i];
      r3 = dctt[24+i]-dctt[40+i];
      h0 = cd*dctt[8+i];
      h1 = be*dctt[8+i];
      r0 = be*dctt[40+i];
      r1 = cd*dctt[24+i];
      w = de*r3;
      t0 = h1+r1+e*(h3+r3)-w;
      t1 = h0-r0-d*(h2-r3)-w;
      w = bc*r2;
      t2 = h0+r0+c*(h3+r2)-w;
      t3 = h1-r1-b*(h2+r2)+w;
      dctt[i] = s0+t0;
      dctt[i+8] = s1+t1;
      dctt[i+16] = s2+t2;
      dctt[i+24] = s3+t3;
      dctt[i+32] = s3-t3;
      dctt[i+40] = s2-t2;
      dctt[i+48] = s1-t1;
      dctt[i+56] = s0-t0;
    } else{
      a0 = dctt[i]*a;
      dctt[i] = dctt[i+8] = dctt[i+16] = dctt[i+24] = dctt[i+32] = dctt[i+40] = dctt[i+48] = dctt[i+56] = a0;
    }
  }
}

void jpgDecodeQTable(int len)
{
  int i;
  unsigned char b;
  len -= 2;
  while (len > 0) {
    b = (unsigned char)jpgGetByte();
    len--;
    if ((b&16) == 0) {
      for (i = 0; i < 64; i++) {
        qtable[b&15][i] = jpgGetByte();
      }
      len -= 64;
    } else {
      for (i = 0; i < 64; i++) {
        qtable[b&15][i] = ((jpgGetByte()<<8) + jpgGetByte());
      }
      len -= 128;
    }
  }
}

void jpgDecodeBlock(void)
{
  int compn, i, j, b, p, codelen, code, cx, cy, otab[64];
  qTable *qtab;

  for (compn = 0; compn < ncomp; compn++)
  {
    qtab = component[compn].qTab;

    for (cy = 0; cy < component[compn].smpy; cy++){
      for(cx = 0; cx < component[compn].smpx; cx++){
        for(i = 0; i < 64; i++) { otab[i] = 0; }

        codelen = jpgHuffDec(component[compn].huffDC);
        code = jpgWordDec(codelen);

        otab[0] = code+component[compn].dcPrev;
        component[compn].dcPrev = otab[0];
        i = 1;

        while (i < 64) {
          codelen = jpgHuffDec(component[compn].huffAC);
          if (codelen == 0) {
            i = 64;
          } else {
            if (codelen == 0xf0) {
              i += 16;
            } else {
              code = jpgWordDec(codelen & 15);
              i = i + (codelen >> 4);
              otab[i++] = code;
            }
          }
        }

        for (i = 0; i < 64; i++) {
          dctt[zigzag[i]] = (float)((*qtab)[i] * otab[i]);
        }

        jpgFidct();
        b = (cy << 7) + (cx << 3);
        p = 0;

        for(i = 0; i < 8; i++) {
          for(j = 0; j < 8; j++) {
            component[compn].t[b++] = dctt[p++] + 128;
          }
          b += 8;
        }

      }
    }
  }
}

bool jpgDecodeScan()
{
  int i;
  int nnx, nny, j;
  int ymin, ymax;
  int xmin, xmax, blockn, adr1, adr2;
  int y1, u1, v1, y2, u2, v2, u3, v3;
  int dux, duy, dvx, dvy;
  unsigned char sc, ts;
  float cy, cu, cv;

  comp = (int)jpgGetByte();

  dprintf("JPG: Buffer = (%d x %d x %d)\n", jpgWidth, jpgHeight, comp);

  jpgBuff = (unsigned char *)malloc(jpgWidth * jpgHeight * 4);

  for(i=0;i<comp;i++){
    sc = jpgGetByte();
    ts = jpgGetByte();

    component[sc-1].huffDC = &huffTableDC[ts>>4];
    component[sc-1].huffAC = &huffTableAC[ts&15];
  }

  ssStart = jpgGetByte();
  ssEnd = jpgGetByte();
  sbits = jpgGetByte();

  if ((ssStart != 0) || (ssEnd != 63)) {
    dprintf("JPG: Unsupported start/end (%d/%d, %d bits)\n", ssStart, ssEnd, sbits);
    return false;
  }

  if (comp == 3) {
    dux = 2 + component[1].smpx-component[0].smpx;
    duy = 2 + component[1].smpy-component[0].smpy;
    dvx = 2 + component[2].smpx-component[0].smpx;
    dvy = 2 + component[2].smpy-component[0].smpy;
  }

  dt = 0;
  bfree = 0;
  jpgStreamSkip(32);

  blockn = 0;
  ymin = 0;

  for (nny = 0; nny < yblock; nny++) {
    ymax = ymin + blocky;
    if (ymax > jpgHeight) { ymax = jpgHeight; }
    xmin = 0;

    for (nnx = 0; nnx < xblock; nnx++) {
      xmax = xmin + blockx;

      if (xmax > jpgWidth) { xmax = jpgWidth; }

      jpgDecodeBlock();

      blockn++;
      if ((blockn == restartInt) && ((nnx < xblock - 1) || (nny < yblock - 1)))
      {
        blockn = 0;

        if (bfree != 0) { jpgStreamSkip(8 - bfree); }

        if (jpgWordDec(8) != 0xff) { dprintf("JPG: WordDec != 0xff\n"); return false; }

        for(i = 0; i < comp; i++) {
          component[i].dcPrev = 0;
        }

      }

      if (comp == 3) {
        y1 = u1 = v1 = 0;
        adr1 = (jpgHeight - 1 - ymin) * jpgWidth + xmin;

        for (i = ymin; i < ymax; i++) {
          adr2 = adr1;
          adr1 -= jpgWidth;
          y2 = y1;
          y1 += 16;
          u3 = (u1 >> 1) << 4;
          u1 += duy;
          v3 = (v1 >> 1) << 4;
          v1 += dvy;
          u2 = v2 = 0;

          for (j = xmin; j < xmax; j++) {
            int cr, cg, cb;
            cy = component[0].t[y2++];
            cu = component[1].t[u3 + (u2 >> 1)] - 128.0f;
            cv = component[2].t[v3 + (v2 >> 1)] - 128.0f;

            cr = (int)(cy + 1.402f * cv);
            cg = (int)(cy - 0.34414f * cu - 0.71414f * cv);
            cb = (int)(cy + 1.772f * cu);

            jpgBuff[4 * adr2 + 0] = jpgClamp(cr);
            jpgBuff[4 * adr2 + 1] = jpgClamp(cg);
            jpgBuff[4 * adr2 + 2] = jpgClamp(cb);
            jpgBuff[4 * adr2 + 3] = 0;
            adr2++;

            u2 += dux;
            v2 += dvx;
          }
        }
      } else {
        if (comp == 1)
        {
          y1 = 0;
          adr1 = (jpgHeight - 1 - ymin) * jpgWidth + xmin;

          for (i = ymin; i < ymax; i++) {
            adr2 = adr1;
            adr1 -= jpgWidth;
            y2 = y1;
            y1 += 16;

            for (j = xmin; j < xmax; j++) {
              int lum = (int)component[0].t[y2++];
              lum = jpgClamp(lum);
              jpgBuff[4 * adr2 + 0] = lum;
              jpgBuff[4 * adr2 + 1] = lum;
              jpgBuff[4 * adr2 + 2] = lum;
              jpgBuff[4 * adr2 + 3] = 0;
              adr2++;
            }
          }
        }
      }
      xmin = xmax;
    }
    ymin = ymax;
  }
  return true;
}



bool jpgDecode(void *buffer, int length)
{
  dprintf("JPG: Decoding %d bytes\n", length);

  data = (unsigned char *)buffer;
  bsize = length;

  component[0].dcPrev = 0;
  component[1].dcPrev = 0;
  component[2].dcPrev = 0;
  component[3].dcPrev = 0;
  restartInt = -1;
  bpos = data;
  dend = bpos + bsize;
  int w = ((jpgGetByte()<<8) + jpgGetByte());
//dprintf("JPG: Magic number 0x%X\n", w);
  if (w != 0xffd8) {
    dprintf("JPG: Wrong magic number 0x%X\n", w);
    return false;
  }

  bool readHeader = false, doneScan = false;
  eof = 0;
  while (!eof) {
    unsigned char a = (unsigned char)jpgGetByte();

//dprintf("JPG: 0xff = 0x%X\n", a);

    if (a != 0xff) { dprintf("JPG: Unexpected command\n"); return false; }

    a = (unsigned char)jpgGetByte();
    w = ((jpgGetByte()<<8) + jpgGetByte());

//dprintf("JPG: Command = 0x%X / %d  ---- ", a, w);

    switch(a) {

      case 0xe0:  // APP0 - Application marker
//dprintf("Read header\n");
        if (readHeader) { dprintf("JPG: Header already read\n"); break; }
        if (jpgGetByte() != 'J') { dprintf("JPG: Wrong header\n"); return false; }
        if (jpgGetByte() != 'F') { dprintf("JPG: Wrong header\n"); return false; }
        if (jpgGetByte() != 'I') { dprintf("JPG: Wrong header\n"); return false; }
        if (jpgGetByte() != 'F') { dprintf("JPG: Wrong header\n"); return false; }
        readHeader = true;
        w -= 4;
        break;

      case 0xc0:  // SOF0 - Start of frame
//dprintf("Get info\n");
        if (!jpgGetInfo()) { dprintf("JPG: Unsupported format\n"); return false; }
        w = 0;
        break;

      case 0xc4:  // DHT - Define Huffman Table
//dprintf("Decode huff\n");
        jpgDecodeHuffTable(w);
        w = 0;
        break;

      case 0xd9:  // EOI - End of image
//dprintf("Compressed image data\n");
        w = 0;
        break;

      case 0xda:  // SOS - Start of scan
//dprintf("Decode scan\n");
        if (doneScan) { break; }
        doneScan = true;
        if (!jpgDecodeScan()) { dprintf("JPG: Decode scan failed\n"); return false; }
        w = 0;
        eof = 1;
        break;

      case 0xdb:  // DQT - Quantization Table
//dprintf("Decode Q table\n");
        jpgDecodeQTable(w);
        w = 0;
        break;

      case 0xdd:
//dprintf("Set restart\n");
        restartInt = ((jpgGetByte()<<8) + jpgGetByte());
        w = 0;
        break;

      case 0xc2:
//dprintf("Progressive JPEG\n");
        dprintf("JPG-ERROR: Progressive JPEG not supported\n");
        return 0;
        break;

      default:
//dprintf("(other)\n");
        break;
    }

    // Consume remainder
    while( w > 2) { jpgGetByte(); w--; }

  }
//  dprintf("JPG: Done!\n"); 
  return true;
}


bool jpgCanHandleBuffer(void *buffer, int length) {
  // LATER: Improve checking for supported types
  if (length >= 2 && ((unsigned char*)buffer)[0] == 0xff && ((unsigned char*)buffer)[1] == 0xd8) {
    return true;
  }
  return false;
}

void *jpgReadRGBA(void *buffer, int length, int *width, int *height, bool *alphaUsed) {
  if (!jpgDecode(buffer, length)) {
    dprintf("JPG: Problem decoding image.\n");
    return NULL;
  }
  if (true) {
    // Flip vertically
    int *sp = (int *)jpgBuff;
    int *dp = sp + jpgWidth*(jpgHeight-1);
    for (int y = 0; y < jpgHeight/2; y++, dp -= 2*jpgWidth) {
      for (int x = 0; x < jpgWidth; x++) { int t = *sp; *sp++ = *dp; *dp++ = t; }
    }
  }
  if (width != NULL) { *width = jpgWidth; }
  if (height != NULL) { *height = jpgHeight; }
  if (alphaUsed != NULL) { *alphaUsed = false; }
  return jpgBuff;
}





void *loadImage(const char *filename, int *outWidth, int *outHeight, bool *outAlphaUsed, int *outTWidth, int *outTHeight, int scaleUp, bool flip) {
  int width = 0, height = 0;
  int tWidth = 0, tHeight = 0;
  bool alphaUsed = false;

  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    dprintf("ERROR: File not found: %s\n", filename);
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  long length = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  unsigned char *readBuffer = (unsigned char *)malloc((size_t)length);
  if (readBuffer == NULL) {
    dprintf("ERROR: Out of memory loading file data (%s).\n", filename);
    fclose(fp);
    return NULL;
  }
  int numRead = (int)fread(readBuffer, 1, (size_t)length, fp);
  if (numRead != length) {
    dprintf("ERROR: Only read %d of %d bytes (%s).\n", numRead, length, filename);
    fclose(fp);
    return NULL;
  }
  fclose(fp);

  dprintf("DEBUG: Read %d bytes of image data (%s).\n", numRead, filename);
  unsigned char *imgBuffer = NULL;
  
  // Try and load as BMP
  if (imgBuffer == NULL && bmpCanHandleBuffer(readBuffer, length)) {
    imgBuffer = (unsigned char *)bmpReadRGBA(readBuffer, length, &width, &height, &alphaUsed);
  }

  // Try and load as TGA
  if (imgBuffer == NULL && tgaCanHandleBuffer(readBuffer, length)) {
    imgBuffer = (unsigned char *)tgaReadRGBA(readBuffer, length, &width, &height, &alphaUsed);
  }

  // Try and load as JPG
  if (imgBuffer == NULL && jpgCanHandleBuffer(readBuffer, length)) {
    imgBuffer = (unsigned char *)jpgReadRGBA(readBuffer, length, &width, &height, &alphaUsed);
  }

  free(readBuffer);
  if (imgBuffer == NULL) {
    dprintf("ERROR: Problem loading file: %s\n", filename);
    return NULL;
  }

  // Flip vertically
  if (flip) {
    int *sp = (int *)imgBuffer;
    int *dp = sp + width*(height-1);
    for (int y = 0; y < height/2; y++, dp -= 2*width) {
      for (int x = 0; x < width; x++) { int t = *sp; *sp++ = *dp; *dp++ = t; }
    }
  }

  tWidth = width;
  tHeight = height;
  if (scaleUp != 0) {
    // Calculate texture size (next nearest power of 2)
    if (width > 1)  { tWidth  = (int)pow(2.0f, (int)(log((double)width -1)/log((double)2))+1); }
    if (height > 1) { tHeight = (int)pow(2.0f, (int)(log((double)height-1)/log((double)2))+1); }
    dprintf("DEBUG: (%d x %d) -> (%d x %d)\n", width, height, tWidth, tHeight);
  }

  // Set out params
  if (outWidth   != NULL) { *outWidth   = width; }
  if (outHeight  != NULL) { *outHeight  = height; }
  if (outTWidth  != NULL) { *outTWidth  = tWidth; }
  if (outTHeight != NULL) { *outTHeight = tHeight; }
  if (outAlphaUsed != NULL) { *outAlphaUsed = alphaUsed; }

  if (scaleUp == 0 || (tWidth == width && tHeight == height)) {
    // Width and height already powers of 2, or no re-scale
    return imgBuffer;
  } else {
    // Need to move to power of 2 sized texture
    unsigned char *texBuffer = (unsigned char *)malloc(tWidth * tHeight * 4);
    if (texBuffer == NULL) {
      dprintf("ERROR: Out of memory converting to texture data.\n");
      return NULL;
    }

    if (scaleUp == 1) {
      // Pads it out and you must adjust your texture co-ordinates accordingly (we could do a cheap re-scale instead)
      memset(texBuffer, 0, tWidth * tHeight * 4);
      for (int yo = 0; yo < height; yo++) {
        memcpy(texBuffer + ((yo * tWidth) * 4), imgBuffer + ((yo * width) * 4), width * 4);
      }
    } else {
      // Resizes the texture (nasty, nearest-pixel resize for now)
      int *sp = (int *)imgBuffer;
      int *dp = (int *)(texBuffer);
      int sy = 0;
      int dsy = (height << 10) / tHeight;
      for (int y = 0; y < tHeight; y++) {
        int *sp2 = sp + ((sy >> 10) * width);
        int sx = 0;
        int dsx = (width << 10) / tWidth;
        for (int x = 0; x < tWidth; x++) {
          *dp++ = sp2[sx>>10];
          sx += dsx;
        }
        sy += dsy;
      }
      if (outWidth   != NULL) { *outWidth   = tWidth; }
      if (outHeight  != NULL) { *outHeight  = tHeight; }
    }

    free(imgBuffer);
    return texBuffer;
  }
}


int createTexture(const char *filename)
{
    int tWidth = 0, tHeight = 0;
    bool tAlpha = false;
    
    void *textureData = loadImage(filename, NULL, NULL, &tAlpha, &tWidth, &tHeight, 2, true);
  
    if (textureData == NULL)
    {
        return -1;
    }
    
    GLuint textureId = 0;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &textureId);
  
    glBindTexture(GL_TEXTURE_2D, textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

#ifdef GLUT_WINDOWING
	gluBuild2DMipmaps(GL_TEXTURE_2D, tAlpha?4:3, tWidth, tHeight, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
#else 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
#endif
    
    if (!glIsTexture(textureId)) { textureId = (int)-1; }
    return (int)textureId;
}

bool setTexture(int id)
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  return true;
}

