#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <stdio.h>
#include <stdlib.h>
#include "truetype.h"
#include "pfonts.h"


stbtt_fontinfo pfonts_loaded_font;


unsigned char * pfonts_sdf_image1 = NULL;

PFontsAtlas pfonts_ascii_atlas;

void pfonts_atlas_range(PFontsAtlas* atlas, float scale, int min, int max){
  int padding = 5;            // Space around glyph for the "glow"
  unsigned char onedge = 180; // Value at the glyph boundary (0-255)
  float dist_scale = 180.0f / (float)padding; // How fast distance ramps
  
  int current_x = 0;
  int current_y = 0;
  int max_row_h = 0;

  for (int i = min; i < max; i++) {
      int w, h, xoff, yoff;
      unsigned char* sdf = stbtt_GetGlyphSDF(
          &pfonts_loaded_font, 
          scale, stbtt_FindGlyphIndex(&pfonts_loaded_font,i), 
          padding, onedge, dist_scale, &w, &h, &xoff, &yoff);
      if(!sdf){
        printf("Can't get glyph\n");
        continue;
      }

      // Simple row-based packing logic
      if (current_x + w + 1 >= atlas->width) {
          current_x = 0;
          current_y += max_row_h + 1;
          max_row_h = 0;
      }

      // Copy glyph SDF into atlas_data
      for (int j = 0; j < h; j++) {
          memcpy(&atlas->data[(current_y + j) * atlas->width + current_x], &sdf[j * w], w);
      }

      // Store UVs for rendering
      atlas->glyphs[i].texture_x = (float)current_x / atlas->width;
      atlas->glyphs[i].texture_y = (float)current_y / atlas->height;
      atlas->glyphs[i].bitmap_width = w;
      atlas->glyphs[i].bitmap_height = h;
      // ... store offsets and advance ...

      current_x += w + 1;
      if (h > max_row_h) max_row_h = h;

      stbtt_FreeSDF(sdf, NULL);
  }
}

void pfonts_init_atlas(PFontsAtlas *atlas){
  unsigned char* new_data = (unsigned char*)calloc(atlas->width * atlas->height, 1);

  if(!new_data)
    printf("Can't allocate memory\n");

  atlas->data = new_data;

  float scale = stbtt_ScaleForPixelHeight(&pfonts_loaded_font, 64); // Target height in pixels

  pfonts_atlas_range(atlas,scale, 32, 127);

  pfonts_generate_texture_sdf(&atlas->texture_id, 
      atlas->data, atlas->width, atlas->height);

}

void pfonts_init_pfonts_atlas(){
  pfonts_ascii_atlas.height = 512;
  pfonts_ascii_atlas.width = 512;


  pfonts_init_atlas(&pfonts_ascii_atlas);

                                      

  
  
}

void pfonts_load_font(const char* font_path){

    long size;
    unsigned char* fontBuffer;
    FILE* file = fopen(font_path, "rb");
    if(!file){
      printf("Font file not found\n");
      return;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    fontBuffer = malloc(size);
    fread(fontBuffer, 1, size, file);
    fclose(file);

    if (!stbtt_InitFont(&pfonts_loaded_font, fontBuffer, 0)) {
        printf("Failed to init font\n");
        return;
    }

    pfonts_init();
}

unsigned char* pfonts_get_glyph_bitmap(char character, int size){

    /*  Setup SDF parameters */
    float scale = stbtt_ScaleForPixelHeight(&pfonts_loaded_font, size); // Target height in pixels
    int padding = 5;            // Space around glyph for the "glow"
    unsigned char onedge = 180; // Value at the glyph boundary (0-255)
    float dist_scale = 180.0f / (float)padding; // How fast distance ramps

    int width, height, xoff, yoff;
    int glyphIndex = stbtt_FindGlyphIndex(&pfonts_loaded_font, character);

    /*  Generate the SDF bitmap */
    pfonts_sdf_image1 = stbtt_GetGlyphSDF(&pfonts_loaded_font, scale, glyphIndex, 
                                                 padding, onedge, dist_scale, 
                                                 &width, &height, &xoff, &yoff);

    pfonts_generate_texture_sdf(&pfonts_sdf_texture, pfonts_sdf_image1, width, height);

}




