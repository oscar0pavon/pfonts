#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <stdio.h>
#include <stdlib.h>

#include "pfonts.h"


stbtt_fontinfo pfonts_loaded_font;


unsigned char * pfonts_sdf_image1 = NULL;

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




