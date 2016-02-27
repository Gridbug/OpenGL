#ifndef INF_FLOAT_ARR_TYPE_H
#define INF_FLOAT_ARR_TYPE_H
#include <stdio.h>

typedef float inf_float_arr_cell;

struct inf_float_arr
{
	int (*Add_cell)(struct inf_float_arr*, const inf_float_arr_cell);
	inf_float_arr_cell* array;
	unsigned int alloced_size;
	unsigned int used_size;
};

struct inf_float_arr* Construct_inf_float_arr(void);
void Destruct_inf_float_arr(struct inf_float_arr* inf_float_arr);

#endif	//INF_FLOAT_ARR_TYPE_H
