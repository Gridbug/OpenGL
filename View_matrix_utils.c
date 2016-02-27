#include "View_matrix_utils.h"

#include <stdio.h>
#include <stdlib.h>


static struct camera_structure my_cam;

static unsigned short init_flag = 0;


static void Send_view_matrix_to_glsl(void)
{
    if (-1 == my_cam.view_matrix_uniform_location)
    {
        fprintf(stderr, "ERROR: Failed to send view matr, bad unif loc.\n");
        exit(EXIT_FAILURE);
    }

    glUniformMatrix4fv(my_cam.view_matrix_uniform_location, 1, GL_FALSE, &my_cam.view_matrix.col[0].x);
}

void Initialize_camera_struct(void)
{
    my_cam.view_matrix = Translation_matrix(Construct_vec4(0.0f, 0.0f, -2.0f, 0.0f));

    if (-1 != my_cam.view_matrix_uniform_location)
    {
        Send_view_matrix_to_glsl();
    }

    my_cam.cam_translation_speed = ZERO_VEC4;
    my_cam.cam_translation_accel = ZERO_VEC4;
    my_cam.rotation = IDENTITY_QUAT;

    init_flag = 1;
}

void Set_view_matrix_uniform_location(GLint new_loc)
{
    if (-1 == new_loc)
    {
        fprintf(stderr, "ERROR: Failed to set view matr unif loc, bad new loc.\n");
        exit(EXIT_FAILURE);
    }

    my_cam.view_matrix_uniform_location = new_loc;
}

void Set_view_matrix(struct mat4_type new_view_matrix)
{
    my_cam.view_matrix = new_view_matrix;
}

struct mat4_type Get_view_matrix(void)
{
    return my_cam.view_matrix;
}

void Translate_camera(struct vec4_type translation_vector)
{
    if (0 == init_flag)
    {
        fprintf(stderr, "WARNING: Initialize the view matrix first!!!\n");
        return;
    }

    my_cam.view_matrix.col[3] = Vector_plus_vector(my_cam.view_matrix.col[3], Quat_by_vector(Get_conjugate_quat(my_cam.rotation), translation_vector));
    my_cam.view_matrix.col[3].w = 1.0f;

    Send_view_matrix_to_glsl();
}

void Increase_camera_speed(struct vec4_type additional_speed)
{
    my_cam.cam_translation_speed = Vector_plus_vector(my_cam.cam_translation_speed, additional_speed);
}

void Set_camera_acceleration(struct vec4_type new_acceleration)
{
    my_cam.cam_translation_accel = new_acceleration;
}

void Stop_camera(void)
{
    my_cam.cam_translation_speed = ZERO_VEC4;
}

struct vec4_type Get_camera_speed(void)
{
    return my_cam.cam_translation_speed;
}

struct vec4_type Get_camera_acceleration(void)
{
    return my_cam.cam_translation_accel;
}

void Rotate_camera(struct vec4_type rot_axis, float angle)
{
    if (0 == init_flag)
    {
        fprintf(stderr, "WARNING: Initialize the view matrix first!!!\n");
        return;
    }

    my_cam.rotation = Quat_by_quat(Quat_from_axis_and_angle(rot_axis, angle), my_cam.rotation);

    my_cam.view_matrix = Matrix_by_matrix(my_cam.view_matrix, Matrix_from_quat(my_cam.rotation));

    Send_view_matrix_to_glsl();
}
