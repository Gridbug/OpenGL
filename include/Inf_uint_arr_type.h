#ifndef INF_UINT_ARR_TYPE_H
#define INF_UINT_ARR_TYPE_H
#include <stdio.h>

typedef unsigned int inf_uint_arr_cell;

struct inf_uint_arr
{
	int (*Add_cell)(struct inf_uint_arr*, const inf_uint_arr_cell);
	inf_uint_arr_cell* array;
	unsigned int alloced_size;
	unsigned int used_size;
};

struct inf_uint_arr* Construct_inf_uint_arr(void);
void Destruct_inf_uint_arr(struct inf_uint_arr* inf_uint_arr);

#endif	//INF_UINT_ARR_TYPE_H
