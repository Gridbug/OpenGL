#include "My_math_utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOLERANCE 0.00001f

const struct mat4_type IDENTITY_MAT4 = {{   {1.0f, 0.0f, 0.0f, 0.0f},
                                            {0.0f, 1.0f, 0.0f, 0.0f},
                                            {0.0f, 0.0f, 1.0f, 0.0f},
                                            {0.0f, 0.0f, 0.0f, 1.0f}}};

const struct vec4_type ZERO_VEC4 = {0.0f, 0.0f, 0.0f, 0.0f};

const struct quaternion IDENTITY_QUAT = {0.0f, 0.0f, 0.0f, 1.0f};

const float PI = 3.14159265358979323846f;

float Degr_to_rad(const float degrees)
{
    return degrees * PI / 180.0f;
}


struct vec4_type Construct_vec4(const float x, const float y, const float z, const float w)
{
    struct vec4_type temp = {x, y, z, w};
    return temp;
}


struct mat4_type Matrix_by_matrix(const struct mat4_type mat1, const struct mat4_type mat2)
{
    struct mat4_type res = IDENTITY_MAT4;

    for (int i = 0; i < 4; i++)
    {
        res.col[i].x =  mat1.col[0].x * mat2.col[i].x +
                        mat1.col[1].x * mat2.col[i].y +
                        mat1.col[2].x * mat2.col[i].z +
                        mat1.col[3].x * mat2.col[i].w;
        res.col[i].y =  mat1.col[0].y * mat2.col[i].x +
                        mat1.col[1].y * mat2.col[i].y +
                        mat1.col[2].y * mat2.col[i].z +
                        mat1.col[3].y * mat2.col[i].w;
        res.col[i].z =  mat1.col[0].z * mat2.col[i].x +
                        mat1.col[1].z * mat2.col[i].y +
                        mat1.col[2].z * mat2.col[i].z +
                        mat1.col[3].z * mat2.col[i].w;
        res.col[i].w =  mat1.col[0].w * mat2.col[i].x +
                        mat1.col[1].w * mat2.col[i].y +
                        mat1.col[2].w * mat2.col[i].z +
                        mat1.col[3].w * mat2.col[i].w;
    }

    return res;
}

struct vec4_type Matrix_by_vector(const struct mat4_type mat1, const struct vec4_type vec2)
{
    struct vec4_type res = ZERO_VEC4;

    res.x = mat1.col[0].x * vec2.x +
            mat1.col[1].x * vec2.y +
            mat1.col[2].x * vec2.z +
            mat1.col[3].x * vec2.w;
    res.y = mat1.col[0].y * vec2.x +
            mat1.col[1].y * vec2.y +
            mat1.col[2].y * vec2.z +
            mat1.col[3].y * vec2.w;
    res.z = mat1.col[0].z * vec2.x +
            mat1.col[1].z * vec2.y +
            mat1.col[2].z * vec2.z +
            mat1.col[3].z * vec2.w;
    res.w = mat1.col[0].w * vec2.x +
            mat1.col[1].w * vec2.y +
            mat1.col[2].w * vec2.z +
            mat1.col[3].w * vec2.w;

    return res;
}

struct mat4_type Scale_matrix(const struct vec4_type scale_vector)
{
    struct mat4_type res = IDENTITY_MAT4;

    res.col[0].x = scale_vector.x;
    res.col[1].y = scale_vector.y;
    res.col[2].z = scale_vector.z;

    return res;
}

struct mat4_type Translation_matrix(const struct vec4_type translation_vector)    //delta coordinates composed into vector
{
    struct mat4_type res = IDENTITY_MAT4;

    res.col[3].x = translation_vector.x;
    res.col[3].y = translation_vector.y;
    res.col[3].z = translation_vector.z;

    return res;
}

struct mat4_type Rotate_around_x_matrix(const float radians)
{
    struct mat4_type res = IDENTITY_MAT4;

    res.col[1].y = (float) cos(radians);
    res.col[1].z = (float) sin(radians);
    res.col[2].y = (float) -sin(radians);
    res.col[2].z = (float) cos(radians);

    return res;
}

struct mat4_type Rotate_around_y_matrix(const float radians)
{
    struct mat4_type res = IDENTITY_MAT4;

    res.col[0].x = (float) cos(radians);
    res.col[0].z = (float) -sin(radians);
    res.col[2].x = (float) sin(radians);
    res.col[2].z = (float) cos(radians);

    return res;
}

struct mat4_type Rotate_around_z_matrix(const float radians)
{
    struct mat4_type res = IDENTITY_MAT4;

    res.col[0].x = (float) cos(radians);
    res.col[0].y = (float) sin(radians);
    res.col[1].x = (float) -sin(radians);
    res.col[1].y = (float) cos(radians);

    return res;
}

struct mat4_type Create_perspective_projection_mat(const float fovy, const float aspect, const float near_plane, const float far_plane)
{
    struct mat4_type res = IDENTITY_MAT4;

    float y_scale = 1.0f / ((float) tan(Degr_to_rad(fovy / 2.0f)));
    float x_scale = y_scale / aspect;
    float frustum_length = far_plane - near_plane;

    res.col[0].x = x_scale;
    res.col[1].y = y_scale;
    res.col[2].z = - (far_plane + near_plane) / frustum_length;
    res.col[2].w = -1.0f;
    res.col[3].z = -2 * far_plane * near_plane / frustum_length;

    return res;
}


struct vec4_type Vector_by_scalar(const struct vec4_type vector, const float scalar)
{
    return (struct vec4_type){  vector.x * scalar,
                                vector.y * scalar,
                                vector.z * scalar,
                                vector.w * scalar};
}

struct vec4_type Vector_plus_vector(const struct vec4_type vector1, const struct vec4_type vector2)
{
    return (struct vec4_type){  vector1.x + vector2.x,
                                vector1.y + vector2.y,
                                vector1.z + vector2.z,
                                vector1.w + vector2.w};
}

struct vec4_type Normalize_vector(const struct vec4_type vector)
{
    struct vec4_type res = ZERO_VEC4;

    float length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

    if (TOLERANCE < fabs(length))
    {
        res.x = vector.x / length;
        res.y = vector.y / length;
        res.z = vector.z / length;
        res.w = vector.w;
    }

    return res;
}


struct quaternion Normalize_quaternion(struct quaternion my_quat)
{
    float magnitude2 = my_quat.w * my_quat.w + my_quat.x * my_quat.x + my_quat.y * my_quat.y + my_quat.z * my_quat.z;

    struct quaternion res = IDENTITY_QUAT;
    if (TOLERANCE < fabs(magnitude2) && TOLERANCE < fabs(magnitude2 - 1.0f))    //copypaste from gpwiki.org
    {
        float magnitude = sqrtf(magnitude2);

        res.x = my_quat.x / magnitude;
        res.y = my_quat.y / magnitude;
        res.z = my_quat.z / magnitude;
        res.w = my_quat.w / magnitude;
    }

    return res;
}

struct quaternion Quat_by_quat(struct quaternion quat1, struct quaternion quat2)    //mostly a copypaste
{
    return (struct quaternion){ quat1.w * quat2.x + quat1.x * quat2.w + quat1.y * quat2.z - quat1.z * quat2.y,
                                quat1.w * quat2.y + quat1.y * quat2.w + quat1.z * quat2.x - quat1.x * quat2.z,
                                quat1.w * quat2.z + quat1.z * quat2.w + quat1.x * quat2.y - quat1.y * quat2.x,
                                quat1.w * quat2.w - quat1.x * quat2.x - quat1.y * quat2.y - quat1.z * quat2.z};
}

struct quaternion Quat_from_axis_and_angle(struct vec4_type axis, float angle)  //axis must be normalized
{
    float half_angle = angle * 0.5f;
    float sin_angle = sinf(half_angle);
    struct vec4_type n_axis = Normalize_vector(axis);

    struct quaternion res = IDENTITY_QUAT;
    res.x = n_axis.x * sin_angle;
    res.y = n_axis.y * sin_angle;
    res.z = n_axis.z * sin_angle;
    res.w = cosf(half_angle);

    return res;
}

struct mat4_type Matrix_from_quat(struct quaternion in_quat)
{
    float xx = in_quat.x * in_quat.x;
    float yy = in_quat.y * in_quat.y;
    float zz = in_quat.z * in_quat.z;
    float xy = in_quat.x * in_quat.y;
    float xz = in_quat.x * in_quat.z;
    float yz = in_quat.y * in_quat.z;
    float wx = in_quat.w * in_quat.x;
    float wy = in_quat.w * in_quat.y;
    float wz = in_quat.w * in_quat.z;

    struct mat4_type res = IDENTITY_MAT4;
    res.col[0].x = 1.0f - 2.0f * (yy + zz);
    res.col[0].y = 2.0f * (xy - wz);
    res.col[0].z = 2.0f * (xz + wy);

    res.col[1].x = 2.0f * (xy + wz);
    res.col[1].y = 1.0f - 2.0f * (xx + zz);
    res.col[1].z = 2.0f * (yz - wx);

    res.col[2].x = 2.0f * (xz - wy);
    res.col[2].y = 2.0f * (yz + wx);
    res.col[2].z = 1.0f - 2.0f * (xx + yy);

    return res;
}

struct quaternion Get_conjugate_quat(struct quaternion in_quat)
{
    return (struct quaternion){-in_quat.x, -in_quat.y, -in_quat.z, in_quat.w};
}

struct vec4_type Quat_by_vector(struct quaternion in_quat, struct vec4_type in_vec)
{
    struct vec4_type n_vec = in_vec;//Normalize_vector(in_vec);

    struct quaternion conjugate = Get_conjugate_quat(in_quat);

    struct quaternion vec_quat = {n_vec.x, n_vec.y, n_vec.z, 0.0f};

    struct quaternion res = Quat_by_quat(vec_quat, conjugate);
    res = Quat_by_quat(in_quat, res);

    return (struct vec4_type){res.x, res.y, res.z, in_vec.w};
}


// struct mat4_type* Create_look_at_mat(void)
// {
//
//     return NULL;
// }
