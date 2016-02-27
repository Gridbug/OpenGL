#include "Physics.h"
#include "View_matrix_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static const float GRAVITY_ACCELERATION = -9.8f;
static const float DELTA_TIME = 0.0166666667f;

static struct all_physics_objects my_phys_objs;

void Initialize_physics_struct(void)
{
    my_phys_objs.array  = (physics_object*) calloc(1, sizeof(physics_object));
    if (NULL == my_phys_objs.array )
    {
        fprintf(stderr, "ERROR: Failed to allocate memory for physics_objects array.\n");
        exit(EXIT_FAILURE);
    }

    my_phys_objs.num_of_phys_objs = 0;
}

static void UPDATE_THIS_FUCKING_LINKS(void)     //Because fucking double realloc breaks all the fucking links between my fucking structure
{
    int useful_index = 0;
    for (int i = 0; i < my_phys_objs.num_of_phys_objs; i++)
    {
        useful_index = my_phys_objs.array[i].phys_object_index_for_array_of_links;

        my_phys_objs.array[i].link_to_model->links_to_phys_objects[useful_index] = &my_phys_objs.array[i];
    }
}

physics_object* Add_phys_obj(struct loaded_model* model_link)
{
    //We already have one cell, but we must allocate one more fo future use
    physics_object* temp = (physics_object*) realloc(my_phys_objs.array, sizeof(physics_object) * ((unsigned int) my_phys_objs.num_of_phys_objs + 1));
    if (NULL == temp)
    {
        fprintf(stderr, "ERROR: Failed to realloc memory for physics object.\n");
        return NULL;
    }
    my_phys_objs.array = temp;

    UPDATE_THIS_FUCKING_LINKS();

    int curr_obj = my_phys_objs.num_of_phys_objs;

    my_phys_objs.num_of_phys_objs++;


    my_phys_objs.array[curr_obj].link_to_model = model_link;

    physics_object** temp2 = (physics_object**) realloc(model_link->links_to_phys_objects, sizeof(physics_object*) * ((unsigned int) model_link->num_of_linked_objs + 1));
    if (NULL == temp2)
    {
        fprintf(stderr, "ERROR: Failed to realloc memory for new phys links.\n");
        return NULL;
    }
    model_link->links_to_phys_objects = temp2;
    model_link->links_to_phys_objects[model_link->num_of_linked_objs] = &my_phys_objs.array[curr_obj];
    my_phys_objs.array[curr_obj].phys_object_index_for_array_of_links = model_link->num_of_linked_objs;
    model_link->num_of_linked_objs++;

    Repair_links_of_physics_objects();

    my_phys_objs.array[curr_obj].model_matrix = IDENTITY_MAT4;

    my_phys_objs.array[curr_obj].translation_speed = (vec4_type){0.0f, 0.0f, 0.0f, 0.0f};
    my_phys_objs.array[curr_obj].translation_acceleration = (vec4_type){0.0f, 0.0f, 0.0f, 0.0f};


    //my_phys_objs.array[curr_obj].rotation_speed = (vec4_type){0.0f, 0.0f, 0.0f, 0.0f};

    my_phys_objs.array[curr_obj].collision_detection_flag = COLLIDING_OBJECT;

    return &my_phys_objs.array[curr_obj];
}

vec4_type Get_object_coordinates(struct physics_object* my_obj)
{
    return my_obj->model_matrix.col[3];
}

void Increase_transl_speed(struct physics_object* my_obj, vec4_type addit_speed)
{
    my_obj->translation_speed.x += addit_speed.x;
    my_obj->translation_speed.y += addit_speed.y;
    my_obj->translation_speed.z += addit_speed.z;
}

void Translate_physics_object(struct physics_object* my_obj, vec4_type translation_vector)
{
    my_obj->model_matrix.col[3].x += translation_vector.x;
    my_obj->model_matrix.col[3].y += translation_vector.y;
    my_obj->model_matrix.col[3].z += translation_vector.z;
}

void Increase_transl_accel(struct physics_object* my_obj, vec4_type additional_acceleration)
{
    my_obj->translation_acceleration.y += additional_acceleration.y;
    my_obj->translation_acceleration.z += additional_acceleration.z;
    my_obj->translation_acceleration.x += additional_acceleration.x;
}

// void Stop_physics_object(struct physics_object* my_obj)
// {
//     my_obj->translation_speed = (vec4_type){0.0f, 0.0f, 0.0f, 0.0f};
// }

void Physics_cycle(void)
{
    for (int i = 0; i < my_phys_objs.num_of_phys_objs; i++)
    {
        physics_object* curr_obj = &my_phys_objs.array[i];

        curr_obj->model_matrix.col[3] = Vector_plus_vector(Get_object_coordinates(curr_obj), Vector_by_scalar(curr_obj->translation_speed, DELTA_TIME));

        curr_obj->translation_speed.x += curr_obj->translation_acceleration.x * DELTA_TIME;
        curr_obj->translation_speed.y += curr_obj->translation_acceleration.y * DELTA_TIME;
        curr_obj->translation_speed.z += curr_obj->translation_acceleration.z * DELTA_TIME;

        if (0.0f < curr_obj->model_matrix.col[3].y)
        {
            curr_obj->translation_speed.y += GRAVITY_ACCELERATION * DELTA_TIME;
        }
        else
        {
            curr_obj->translation_speed.y = 0.0f;
            curr_obj->translation_acceleration.y = 0.0f;
        }
    }

    vec4_type temp = Vector_by_scalar(Get_camera_speed(), DELTA_TIME);
    Increase_camera_speed(Vector_by_scalar(Get_camera_acceleration(), DELTA_TIME));
    Translate_camera(temp);

    //printf("%3.6f.%3.6f.%3.6f.%3.6f\n", temp.x, temp.y, temp.z, temp.w);

    // struct loaded_model* model = Get_model_pointer_by_name("Second cube");
    // for (int i = 0; i < model->num_of_linked_objs; i++)
    // {
    //     float cube_center_x = (i + 1) * (10.0f + i);
    //     vec4_type player_coords = my_phys_objs.array[0].model_matrix.col[3];
    //     if (fabs(player_coords.x - cube_center_x) < 2.0f && player_coords.y < 1.5)
    //     {
    //         srand((unsigned int) clock());
    //
    //         printf("You lose!\n");
    //         printf("Your score is: %.1f\n", (i - 1.0f) * 10.0f + (rand() % 10) / 10.0f);
    //         printf("My grandma can play better than you.\n");
    //         exit(EXIT_FAILURE);
    //     }
    // }
}
