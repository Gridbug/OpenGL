#ifndef PHYSICS_H
#define PHYSICS_H

#include "My_math_utils.h"
#include "Meshes.h"

#define COLLIDING_OBJECT 1
#define NOT_COLLIDING_OBJECT 0

typedef struct physics_object
{
    struct loaded_model* link_to_model;
    int phys_object_index_for_array_of_links;       //array of links of the models to which this phys object was linked

    struct mat4_type model_matrix;

    vec4_type translation_speed;
    vec4_type translation_acceleration;

    int collision_detection_flag;       //Will be used in further development
}physics_object;

struct all_physics_objects
{
    struct physics_object* array;
    int num_of_phys_objs;
    //int allocated_size;
    //int used_size;
};

void Initialize_physics_struct(void);
physics_object* Add_phys_obj(struct loaded_model* model_link);
vec4_type Get_object_coordinates(struct physics_object* my_obj);
void Increase_transl_speed(struct physics_object* my_obj, vec4_type addit_speed);
void Physics_cycle(void);
void Translate_physics_object(struct physics_object* my_obj, vec4_type translation_vector);
void Increase_transl_accel(struct physics_object* my_obj, vec4_type additional_acceleration);

#endif  //PHYSICS_H
