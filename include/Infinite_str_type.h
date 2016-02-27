#ifndef INFINITE_STR_TYPE_H
#define INFINITE_STR_TYPE_H
#include <stdio.h>

struct infinite_string
{
	int (*Add_char)(struct infinite_string*, const int);
	char* string;
	unsigned int alloced_size;
	unsigned int used_size;
};

struct infinite_string* Construct_infinite_string(void);
void Destruct_infinite_string(struct infinite_string* inf_str);
int Read_infinite_string(struct infinite_string *const inf_str);
int Read_infinite_string_from_file(struct infinite_string *const inf_str, FILE *const input_file);
void Fast_str_cleaning(struct infinite_string *const inf_str);
void Clean_inf_str(struct infinite_string *const inf_str);


#endif	//INFINITE_STR_TYPE_H
