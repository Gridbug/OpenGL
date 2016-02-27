#include "Inf_uint_arr_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define INITIAL_SIZE 4
#define SIZE_STEP 2

static int Add_to_inf_uint_arr(struct inf_uint_arr* inf_uint_arr, const inf_uint_arr_cell new_cell)
{
	assert(inf_uint_arr);

	if (inf_uint_arr->used_size >= inf_uint_arr->alloced_size)
	{
		inf_uint_arr_cell* temp = (inf_uint_arr_cell*) realloc(inf_uint_arr->array, (inf_uint_arr->alloced_size * SIZE_STEP) * sizeof(inf_uint_arr_cell));
		if (NULL == temp)
		{
			return -1;
		}
		inf_uint_arr->array = temp;
		inf_uint_arr->alloced_size *= SIZE_STEP;
	}
	inf_uint_arr->array[inf_uint_arr->used_size] = new_cell;
	inf_uint_arr->used_size++;

	return 0;
}

struct inf_uint_arr* Construct_inf_uint_arr(void)
{
	struct inf_uint_arr* tmp_str = (struct inf_uint_arr*) malloc(sizeof(struct inf_uint_arr));
	tmp_str->Add_cell = Add_to_inf_uint_arr;
	inf_uint_arr_cell* tmp = (inf_uint_arr_cell*) calloc(INITIAL_SIZE, sizeof(inf_uint_arr_cell));
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

void Destruct_inf_uint_arr(struct inf_uint_arr* inf_uint_arr)
{
	if (NULL == inf_uint_arr)
	{
		return;
	}
	if (NULL != inf_uint_arr->array)
	{
		// for(unsigned int i = 0; i < inf_uint_arr->used_size; i++)	//for data types made of pointers
		// {
		// 	free(inf_uint_arr->array[i]);
		// }
		free(inf_uint_arr->array);
	}
	free(inf_uint_arr);
}
