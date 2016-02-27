#include "Inf_float_arr_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define INITIAL_SIZE 4
#define SIZE_STEP 2

static int Add_to_inf_float_arr(struct inf_float_arr* inf_float_arr, const inf_float_arr_cell new_cell)
{
	assert(inf_float_arr);

	if (inf_float_arr->used_size >= inf_float_arr->alloced_size)
	{
		inf_float_arr_cell* temp = (inf_float_arr_cell*) realloc(inf_float_arr->array, (inf_float_arr->alloced_size * SIZE_STEP) * sizeof(inf_float_arr_cell));
		if (NULL == temp)
		{
			return -1;
		}
		inf_float_arr->array = temp;
		inf_float_arr->alloced_size *= SIZE_STEP;
	}
	inf_float_arr->array[inf_float_arr->used_size] = new_cell;
	inf_float_arr->used_size++;

	return 0;
}

struct inf_float_arr* Construct_inf_float_arr(void)
{
	struct inf_float_arr* tmp_str = (struct inf_float_arr*) malloc(sizeof(struct inf_float_arr));
	tmp_str->Add_cell = Add_to_inf_float_arr;
	inf_float_arr_cell* tmp = (inf_float_arr_cell*) calloc(INITIAL_SIZE, sizeof(inf_float_arr_cell));
	if (NULL == tmp)
	{
		free(tmp_str);
		return NULL;
	}

	tmp_str->array = tmp;
	tmp_str->alloced_size = INITIAL_SIZE;
	tmp_str->used_size = 0;

	return tmp_str;
}

void Destruct_inf_float_arr(struct inf_float_arr* inf_float_arr)
{
	if (NULL == inf_float_arr)
	{
		return;
	}
	if (NULL != inf_float_arr->array)
	{
		// for(unsigned int i = 0; i < inf_float_arr->used_size; i++)	//for data types made of pointers
		// {
		// 	free(inf_float_arr->array[i]);
		// }
		free(inf_float_arr->array);
	}
	free(inf_float_arr);
}
