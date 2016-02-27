#include "Load_utils.h"
#include "Inf_float_arr_type.h"
#include "Inf_uint_arr_type.h"
#include "Infinite_str_type.h"

#include <stdio.h>
#include <stdlib.h>

ivec4_type Load_obj_mesh_from_file(const char *const file_name, vertex** vertices_array, GLuint** indices_array)
{
    ivec4_type res = {-1, 0, 0, 0};

    FILE* mesh_file = fopen(file_name, "r");
    if (NULL == mesh_file)
    {
        fprintf(stderr, "ERROR: Failed to open mesh file \"%s\". (Bad file name?)\n", file_name);
        return res;
    }

    struct infinite_string* input_str = Construct_infinite_string();
	if (NULL == input_str)
	{
		fprintf(stderr, "ERROR: Failed to construct inf str.\n");
        return res;
	}

    struct inf_uint_arr* my_uint_arr = Construct_inf_uint_arr();
    if (NULL == my_uint_arr)
	{
		fprintf(stderr, "ERROR: Failed to construct inf uint arr.\n");
        Destruct_infinite_string(input_str);
        return res;
	}

    struct inf_float_arr* my_float_arr = Construct_inf_float_arr();
    if (NULL == my_float_arr)
	{
		fprintf(stderr, "ERROR: Failed to construct inf float arr.\n");
        Destruct_inf_uint_arr(my_uint_arr);
        Destruct_infinite_string(input_str);
        return res;
	}

    int count_lines = 0;
    while (1)
	{
		int rc = Read_infinite_string_from_file(input_str, mesh_file);			//Get key from mesh_file
		if (-1 == rc)
		{
			fprintf(stderr, "ERROR: Failed to read inf str from mesh_file. Line #%d\n", count_lines);
            Destruct_inf_uint_arr(my_uint_arr);
            Destruct_inf_float_arr(my_float_arr);
			Destruct_infinite_string(input_str);

            return res;
		}
		else if (0 == rc)		//we read 0 chars
		{
			break;
		}

        switch (input_str->string[0])
        {
            case 'v':
            {
                float val[3] = {0.0f};
        		rc = fscanf(mesh_file, "%f %f %f", &val[0], &val[1], &val[2]);						//Get values from mesh_file
        		if (3 != rc)
        		{
        			if (EOF == rc)
        			{
        				break;
        			}
        			else
        			{
        				fprintf(stderr, "ERROR: Failed to read values from mesh_file. Line #%d\n", count_lines);
                        Destruct_inf_uint_arr(my_uint_arr);
                        Destruct_inf_float_arr(my_float_arr);
        				Destruct_infinite_string(input_str);

                        return res;
        			}
        		}

                my_float_arr->Add_cell(my_float_arr, val[0]);
                my_float_arr->Add_cell(my_float_arr, val[1]);
                my_float_arr->Add_cell(my_float_arr, val[2]);

                break;
            }
            case 'f':
            {
                unsigned int val[3] = {0};
        		rc = fscanf(mesh_file, "%d %d %d", &val[0], &val[1], &val[2]);						//Get values from mesh_file
        		if (3 != rc)
        		{
        			if (EOF == rc)
        			{
        				break;
        			}
        			else
        			{
        				fprintf(stderr, "ERROR: Failed to read values from mesh_file. Line #%d\n", count_lines);
                        Destruct_inf_uint_arr(my_uint_arr);
                        Destruct_inf_float_arr(my_float_arr);
        				Destruct_infinite_string(input_str);

                        return res;
        			}
        		}

                my_uint_arr->Add_cell(my_uint_arr, val[0] - 1);
                my_uint_arr->Add_cell(my_uint_arr, val[1] - 1);
                my_uint_arr->Add_cell(my_uint_arr, val[2] - 1);

                break;
            }
        }


		rc = fgetc(mesh_file);
		while ('\n' != rc && EOF != rc)		//eating useless spaces?
		{
			rc = fgetc(mesh_file);
		}

		count_lines++;
		Fast_str_cleaning(input_str);
	}

    fclose(mesh_file);
    Destruct_infinite_string(input_str);

    *indices_array = (GLuint*) calloc(my_uint_arr->used_size, sizeof(GLuint));
    if (NULL == (*indices_array))
    {
        fprintf(stderr, "ERROR: Failed copy indices from inf arr while loading mesh file.\n");
        Destruct_inf_uint_arr(my_uint_arr);
        Destruct_inf_float_arr(my_float_arr);
        Destruct_infinite_string(input_str);
        return res;
    }
    for (unsigned int i = 0; i < my_uint_arr->used_size; i++)
    {
        (*indices_array)[i] = my_uint_arr->array[i];
    }

    *vertices_array = (vertex*) calloc(my_float_arr->used_size / 3, sizeof(vertex));
    if (NULL == (*vertices_array))
    {
        fprintf(stderr, "ERROR: Failed copy vertices from inf arr while loading mesh file.\n");
        Destruct_inf_uint_arr(my_uint_arr);
        Destruct_inf_float_arr(my_float_arr);
        Destruct_infinite_string(input_str);
        return res;
    }
    for (unsigned int i = 0; i < my_float_arr->used_size / 3; i++)
    {
        (*vertices_array)[i].xyzw[0] = my_float_arr->array[i * 3];
        (*vertices_array)[i].xyzw[1] = my_float_arr->array[i * 3 + 1];
        (*vertices_array)[i].xyzw[2] = my_float_arr->array[i * 3 + 2];
        (*vertices_array)[i].xyzw[3] = 1.0f;
    }

    res.x = (int) my_float_arr->used_size / 3;
    res.y = (int) my_uint_arr->used_size;

    Destruct_inf_uint_arr(my_uint_arr);
    Destruct_inf_float_arr(my_float_arr);

    return res;
}
