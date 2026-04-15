#include "pfonts.h"
#include "shader.h"

void pfonts_init(){

  pfonts_create_shaders();
  pfonts_get_glyph_bitmap('A', 64);


}

