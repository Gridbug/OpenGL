#ifndef MESHES_H
#define MESHES_H

#include "My_math_utils.h"
#include "GL/gl3w.h"
#include "Physics.h"

typedef GLuint type_of_indices;

struct loaded_model
{
    GLuint vao_id;      //description for Opengl about this model

    GLuint vertices_buff_id;
    struct vertex_struct* model_vertices;             //vertex: position, color, normals
    int num_of_vertices;

    GLuint indices_buff_id;
    type_of_indices* model_indices;     //faces of the model
    int num_of_indices;

    char* model_name;

    struct physics_object** links_to_phys_objects;    //array of linked to this model physics objects
    int num_of_linked_objs;     //number of physics objects linked to this model
};

struct all_loaded_models
{
    struct loaded_model* array;
    int num_of_models;
    //int allocated_size;
    //int used_size;
};

struct loaded_model* Load_model_ff_and_name_it(const char *const file_name, const char *const new_model_name);     //ff - means "from file"
struct loaded_model* Get_model_pointer_by_name(const char *const model_name);
void Initialize_mesh_struct(void);

void Repair_links_of_physics_objects(void);

#endif  //MESHES_H
