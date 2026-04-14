#ifndef PFONTS_H
#define PFONTS_H

#include <GL/gl.h>

typedef struct PUV{
    float x;
    float y;
}PUV;

typedef struct PColor{
  GLfloat r;
  GLfloat g;
  GLfloat b;
} PColor;

void pfonts_init();

void pfonts_draw_rect(PColor color,  float x, float y, float width, float height);


void pfonts_draw_char(uint8_t character, PColor color, float x, float y,
                  float width, float height);

void pfonts_set_ortho_projection(float width, float height);

unsigned char* pfonts_get_glyph_bitmap(char character, int size);

void pfonts_load_font(const char* font_path);

void pfonts_load_image_data(const char* image_data, int width, int height);

extern GLuint pfont_texture_id;

#endif
