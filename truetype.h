#ifndef PFONTS_TRUETYPE_H
#define PFONTS_TRUETYPE_H

#include "pfonts.h"

typedef struct PFontsGlyphInfo {
  int advance_x, advance_y;
  int bitmap_width, bitmap_height;
  int bitmap_left, bitmap_top;
  float u0,v0,u1,v1;//UVs
  float x_offset, y_offset;
}PFontsGlyphInfo;

typedef struct PFontsAtlas {
  PFontsGlyphInfo glyphs[128];
  int width;
  int height;
  unsigned char* data;
  GLuint texture_id;
}PFontsAtlas;

extern unsigned char * pfonts_sdf_image1;

extern PFontsAtlas pfonts_ascii_atlas;

void pfonts_init_pfonts_atlas();

#endif
