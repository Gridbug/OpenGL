#ifndef MATH_TYPES_H
#define MATH_TYPES_H

extern const float PI;

typedef struct vertex_struct
{
    float xyzw[4];
    float rgba[4];
}vertex;

typedef struct vec4_type
{
    float x;
    float y;
    float z;
    float w;
}vec4_type;

typedef struct ivec4_type
{
    int x;
    int y;
    int z;
    int w;
}ivec4_type;

typedef struct mat4_type
{
    struct vec4_type col[4];    //columns
}mat4_type;

struct quaternion
{
    float x;
    float y;
    float z;
    float w;
};

extern const struct mat4_type IDENTITY_MAT4;
extern const struct vec4_type ZERO_VEC4;
extern const struct quaternion IDENTITY_QUAT;

float Degr_to_rad(const float degrees);

struct vec4_type Construct_vec4(const float x, const float y, const float z, const float w);

//struct mat4_type Get_identity_matrix(void);
struct mat4_type Matrix_by_matrix(const struct mat4_type mat1, const struct mat4_type mat2);        //multiplication
struct vec4_type Matrix_by_vector(const struct mat4_type mat1, const struct vec4_type vec2);        //multiplication
struct mat4_type Scale_matrix(const struct vec4_type scale_vector);
struct mat4_type Translation_matrix(const struct vec4_type translation_vector);
struct mat4_type Rotate_around_x_matrix(const float radians);
struct mat4_type Rotate_around_y_matrix(const float radians);
struct mat4_type Rotate_around_z_matrix(const float radians);
struct mat4_type Create_perspective_projection_mat(const float fovy, const float aspect, const float near_plane, const float far_plane);

struct vec4_type Vector_by_scalar(const struct vec4_type vector, const float scalar);
struct vec4_type Vector_plus_vector(const struct vec4_type vector1, const struct vec4_type vector2);
struct vec4_type Normalize_vector(const struct vec4_type vector);

struct quaternion Normalize_quaternion(struct quaternion my_quat);
struct quaternion Quat_from_axis_and_angle(struct vec4_type axis, float angle);
struct mat4_type Matrix_from_quat(struct quaternion);
struct quaternion Quat_by_quat(struct quaternion quat1, struct quaternion quat2);
struct vec4_type Quat_by_vector(struct quaternion in_quat, struct vec4_type in_vec);
struct quaternion Get_conjugate_quat(struct quaternion in_quat);

//struct mat4_type* Create_look_at_mat(void);

#endif  //MATH_TYPES_H
