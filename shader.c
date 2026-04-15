#include "shader.h"
#include <GL/glext.h>
#include <stddef.h>
#include <stdio.h>
#include "pfonts.h"

GLuint pfonts_shader;


static const char * pfonts_vertex_shader = 
"#version 120\n"
"void main() {\n"
"    gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"    gl_Position = ftransform();\n"
"}";


static const char * pfonts_fragment_shader = 
"#version 120\n"
"uniform sampler2D u_texture;\n"
"void main() {\n"
"    float dist = texture2D(u_texture, gl_TexCoord[0].ts).r;\n"
"    float width = fwidth(dist);\n"
"    float alpha = smoothstep(0.5 - width, 0.5 + width, dist);\n"
"    gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);\n"
"}";

void pfonts_create_shaders(){
  GLint success;
  char infoLog[512];

  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);


  glShaderSource(vertex, 1, &pfonts_vertex_shader, NULL);

  glShaderSource(fragment, 1, &pfonts_fragment_shader, NULL);

  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      printf("ERROR::VERTEX_SHADER_COMPILATION_FAILED\n%s\n", infoLog);
  }

  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      printf("ERROR::FRAGMENT_SHADER_COMPILATION_FAILED\n%s\n", infoLog);
  }

  pfonts_shader = glCreateProgram(); 

  glAttachShader(pfonts_shader, vertex);
  glAttachShader(pfonts_shader, fragment);

  glLinkProgram(pfonts_shader);


}


