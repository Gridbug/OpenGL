#ifndef SHADERS_H_
#define SHADERS_H_

#include "GL/gl3w.h"

GLuint Make_program_from_shaders(const char *const veterx_shdr_name, const char *const fragment_shdr_name);
void Delete_program_and_shaders(GLuint program_id);
void Handle_OGL_errors(const char *const message);

#endif  //SHADERS_H_
