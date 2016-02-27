#ifndef LOAD_UTILS_H
#define LOAD_UTILS_H

#include "My_math_utils.h"
#include "GL/gl3w.h"

ivec4_type Load_obj_mesh_from_file(const char *const file_name, vertex** vertices_array, GLuint** indices_array);


#endif  //LOAD_UTILS_H
