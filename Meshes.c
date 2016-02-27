#include "Meshes.h"
#include "Load_utils.h"

#include <stdio.h>
#include <stdlib.h>


static struct all_loaded_models my_models;

static const GLuint vertex_pos_location = 0;
static const GLuint vertex_color_location = 1;

struct loaded_model* Load_model_ff_and_name_it(const char *const file_name, const char *const new_model_name)     //ff - means "from file"
{
    //We already have one allocated cell, this allocation is for future use
    struct loaded_model* rc = (struct loaded_model*) realloc(my_models.array, sizeof(struct loaded_model) * ((unsigned int) my_models.num_of_models + 1));
    if (NULL == rc)
    {
        fprintf(stderr, "ERROR: Failed to realloc memory for model loading%s\n");
        return NULL;
    }
    my_models.array = rc;


    int curr_mod = my_models.num_of_models; //current_model

    ivec4_type rc2 = Load_obj_mesh_from_file(file_name, &my_models.array[curr_mod].model_vertices, &my_models.array[curr_mod].model_indices);
    if (-1 == rc2.x)
    {
        return NULL;
    }
    my_models.array[curr_mod].num_of_vertices = rc2.x;
    my_models.array[curr_mod].num_of_indices = rc2.y;

    for (int i = 0; i < rc2.x; i++)          //Temp workaround, waiting textures
    {
        my_models.array[curr_mod].model_vertices[i].rgba[i % 3] = 1.0f;
        my_models.array[curr_mod].model_vertices[i].rgba[3] = 1.0f;
    }

    glGenVertexArrays(1, &my_models.array[curr_mod].vao_id);
    GLuint vao_id = my_models.array[curr_mod].vao_id;
    glBindVertexArray(vao_id);

    glGenBuffers(1, &my_models.array[curr_mod].vertices_buff_id);
    glBindBuffer(GL_ARRAY_BUFFER, my_models.array[curr_mod].vertices_buff_id);
    glBufferData(GL_ARRAY_BUFFER, (int) sizeof(vertex) * rc2.x, my_models.array[curr_mod].model_vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &my_models.array[curr_mod].indices_buff_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_models.array[curr_mod].indices_buff_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int) sizeof(GLuint) * rc2.y, my_models.array[curr_mod].model_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(vertex_pos_location, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(vertex_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*) (sizeof(float) * 4));

    glEnableVertexAttribArray(vertex_pos_location);
    glEnableVertexAttribArray(vertex_color_location);

    my_models.array[curr_mod].model_name = (char*) calloc(strlen(new_model_name) + 1, sizeof(char));
    strcpy(my_models.array[curr_mod].model_name, new_model_name);

    my_models.array[curr_mod].links_to_phys_objects = (physics_object**) calloc(1, sizeof(physics_object*));
    if (NULL == my_models.array[curr_mod].links_to_phys_objects)
    {
        fprintf(stderr, "ERROR: Failed to allocate memory for new phys links.%s\n");
        return NULL;
    }
    my_models.array[curr_mod].num_of_linked_objs = 0;

    my_models.num_of_models++;

    return &my_models.array[curr_mod];
}

struct loaded_model* Get_model_pointer_by_name(const char *const model_name)
{
    for (int i = 0; i < my_models.num_of_models; i++)
    {
        if (0 == strcmp(my_models.array[i].model_name, model_name))
        {
            return &my_models.array[i];
        }
    }

    fprintf(stderr, "ERROR: Failed to find model %s (Bad name?)\n", model_name);
    return NULL;
}

void Initialize_mesh_struct(void)
{
    my_models.array = (struct loaded_model*) calloc(1, sizeof(struct loaded_model));
    if (NULL == my_models.array)
    {
        fprintf(stderr, "ERROR: Failed to allocate meshes array.\n");
        exit(EXIT_FAILURE);
    }

    my_models.num_of_models = 0;
}

 void Repair_links_of_physics_objects(void)
{
    for (int i = 0; i < my_models.num_of_models; i++)
    {
        for (int j = 0; j < my_models.array[i].num_of_linked_objs; j++)
        {
            my_models.array[i].links_to_phys_objects[j]->link_to_model = &my_models.array[i];
        }
    }
}
