#ifndef VIEW_MATRIX_UTILS_H
#define VIEW_MATRIX_UTILS_H

#include "My_math_utils.h"
#include "GL/gl3w.h"


struct camera_structure
{
    struct mat4_type view_matrix;
    GLint view_matrix_uniform_location;

    vec4_type cam_translation_speed;
    vec4_type cam_translation_accel;

    struct quaternion rotation;
};

void Initialize_camera_struct(void);
void Set_view_matrix_uniform_location(GLint new_loc);
void Set_view_matrix(struct mat4_type new_view_matrix);
struct mat4_type Get_view_matrix(void);

void Translate_camera(struct vec4_type translation_vector);
void Increase_camera_speed(struct vec4_type additional_speed);
void Set_camera_acceleration(struct vec4_type new_acceleration);
void Stop_camera(void);
struct vec4_type Get_camera_speed(void);
struct vec4_type Get_camera_acceleration(void);

void Rotate_camera(struct vec4_type rot_axis, float angle);

#endif
