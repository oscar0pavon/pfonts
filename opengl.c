#include "pfonts.h"
#include <math.h>
#include "shader.h"
#include "truetype.h"

GLuint pfont_texture_id;

GLuint pfonts_sdf_texture;

void pfonts_set_ortho_projection(float width, float height){


    glLoadIdentity();

    //2D projection where 0,0 is top-left and width,height is bottom-right
    float left = 0.0f;
    float right = (float)width; //window's width
    float bottom = (float)height; //window's height
    float top = 0.0f;
    float near = -1.0f; // Near clipping plane
    float far = 1.0f;   // Far clipping plane

    glOrtho(left, right, bottom, top, near, far);

}

void pfonts_draw_rect(PColor color,  float x, float y, float width, float height){
    glColor4f(color.r, color.g, color.b,1.f); 

    glBegin(GL_QUADS);
        glVertex3f(x, y,-1);
        glVertex3f(x + width, y,-1);
        glVertex3f(x + width, y + height,-1);
        glVertex3f(x, y + height,-1);
    glEnd();
 
    //debug grid red border line
    // glColor4f(1, 0, 0,1.f); 
    // glLineWidth(1);
    // glBegin(GL_LINE_LOOP);
    //     glVertex3f(x, y,-1);
    //     glVertex3f(x + width, y,-1);
    //     glVertex3f(x + width, y + height,-1);
    //     glVertex3f(x, y + height,-1);
    // glEnd();
}

void pfonts_draw_char(uint8_t character, PColor color, float x, float y,
                  float width, float height) {

  float char_x = character % 16;
  float char_y = floor((float)character / 16);

  float char_size_x = 32.f / 512.f;
  float char_size_y = 32.f / 512.f;

  PUV uv1;
  PUV uv2;
  PUV uv3;
  PUV uv4;

  // button left
  uv1.x = char_x * char_size_x;
  uv1.y = char_y * char_size_y;

  // button right
  uv2.x = (char_x + 1) * char_size_x;
  uv2.y = char_y * char_size_y;

  // top right
  uv3.x = (char_x + 1) * char_size_x;
  uv3.y = (char_y + 1) * char_size_y;

  // top left
  uv4.x = char_x * char_size_x;
  uv4.y = (char_y + 1) * char_size_y;

  float alpha_value = 0.f;

  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindTexture(GL_TEXTURE_2D, pfont_texture_id);


  glColor3f(color.r, color.g, color.b);

  glBegin(GL_QUADS);
  glTexCoord2f(uv1.x, uv1.y);
  glVertex2f(x, y);
  glTexCoord2f(uv2.x, uv2.y);
  glVertex2f(x + width, y);
  glTexCoord2f(uv3.x, uv3.y);
  glVertex2f(x + width, y + height);
  glTexCoord2f(uv4.x, uv4.y);
  glVertex2f(x, y + height);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  glDisable(GL_BLEND);
}

void pfonts_draw_glyph_from_atlas(PFontsAtlas* atlas,
    int character, 
    float x, float y, float scale){

  PFontsGlyphInfo glyph = atlas->glyphs[character];

  float w = glyph.bitmap_width;
  float h = glyph.bitmap_height;

  float x0 = floorf(x + (float)glyph.x_offset);
  float y0 = floorf(y + (float)glyph.y_offset);
  float x1 = x0 + w;
  float y1 = y0 + h;

  glUseProgram(pfonts_shader);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, atlas->texture_id);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_QUADS);
      glTexCoord2f(glyph.u1, glyph.v0);
      glVertex2f(x0, y1);

      glTexCoord2f(glyph.u0, glyph.v0);
      glVertex2f(x0, y0);

      glTexCoord2f(glyph.u0, glyph.v1);
      glVertex2f(x1, y0);

      glTexCoord2f(glyph.u1, glyph.v1);
      
      glVertex2f(x1, y1);
  glEnd();

  glDisable(GL_TEXTURE_2D);
  
  glDisable(GL_BLEND);

  glUseProgram(0);
}

void pfonts_draw_with_texture_id(GLuint texture_id, float x, float y, float width, float height){

  glUseProgram(pfonts_shader);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, texture_id);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(x, y);

      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(x + width, y);

      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(x + width, y + height);

      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(x, y + height);
  glEnd();

  glDisable(GL_TEXTURE_2D);
  
  glDisable(GL_BLEND);

  glUseProgram(0);
}

void pfonts_draw_sdf_char(float x, float y, float width, float height){
  // pfonts_draw_with_texture_id(pfonts_sdf_texture, x, y , width, height);
  //
  // pfonts_draw_with_texture_id(pfonts_ascii_atlas.texture_id, 500, 300 , 256, 256);

  pfonts_draw_glyph_from_atlas(&pfonts_ascii_atlas, (int)'a', 100,200, 1);

}
void pfonts_generate_texture_sdf(GLuint* texture, const char* image_data, int width, int height) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    // CRITICAL: stbt_truetype outputs 1 byte per pixel. 
    // If your width is not a multiple of 4, OpenGL will misread rows without this.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Use GL_LUMINANCE for both internal format and data format.
    // This tells OpenGL that each byte in image_data is a single grayscale value.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, image_data);
}

void pfonts_generate_texture(GLuint* texture, const char* image_data, int width, int height){

  glGenTextures(1, texture);

  glBindTexture(GL_TEXTURE_2D, *texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
}


void pfonts_load_image_data(const char* image_data, int width, int height){

  pfonts_generate_texture(&pfont_texture_id, image_data, width, height);

}
