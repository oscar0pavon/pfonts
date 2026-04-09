#include "pfonts.h"
#include <math.h>

GLuint pfont_texture_id;

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
