#include "Infinite_str_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>


#define INITIAL_SIZE 4
#define SIZE_STEP 2


static int Add_to_infinite_string(struct infinite_string* inf_str, const int new_char)
{
	assert(inf_str);

	if (inf_str->used_size >= inf_str->alloced_size)
	{
		char* temp = (char*) realloc(inf_str->string, (inf_str->alloced_size * SIZE_STEP) * sizeof(char));
		if (NULL == temp)
		{
			return -1;
		}
		inf_str->string = temp;
		inf_str->alloced_size *= SIZE_STEP;
	}
	inf_str->string[inf_str->used_size] = (char) new_char;
	inf_str->used_size++;

	return 0;
}

int Read_infinite_string(struct infinite_string *const inf_str)
{
	return Read_infinite_string_from_file(inf_str, stdin);
}

struct infinite_string* Construct_infinite_string(void)
{
	struct infinite_string* tmp_str = (struct infinite_string*) malloc(sizeof(struct infinite_string));
	tmp_str->Add_char = Add_to_infinite_string;
	char* tmp = (char*) calloc(INITIAL_SIZE, sizeof(tmp_str->string));
	if (NULL == tmp)
	{
		free(tmp_str);
		return NULL;
	}

	tmp_str->string = tmp;
	tmp_str->alloced_size = INITIAL_SIZE;
	tmp_str->used_size = 0;

	return tmp_str;
}

void Destruct_infinite_string(struct infinite_string* inf_str)
{
	if (NULL == inf_str)
	{
		return;
	}
	if (NULL != inf_str->string)
	{
		free(inf_str->string);
	}
	free(inf_str);
}

int Read_infinite_string_from_file(struct infinite_string *const inf_str, FILE *const input_file)
{
	assert(inf_str);
	assert(input_file);

	int input = 0;
	int count_digits = 0;

	while (!isspace(input))
	{
		input = fgetc(input_file);
		if (EOF == input)
		{
			break;
		}
		else if ((0 == count_digits) && (isspace(input)))
		{
			do
			{
				input = fgetc(input_file);
				if (EOF == input)
				{
					return 0;
				}
			}
			while(isspace(input));
		}

		if (!isspace(input))
		{
			int rc = inf_str->Add_char(inf_str, input);
			if (0 > rc)
			{
				return -1;
			}
			count_digits++;
		}
	}
	
	int rc = inf_str->Add_char(inf_str, '\0');
	if (0 > rc)
	{
		return -1;
	}

	return count_digits;
}

void Fast_str_cleaning(struct infinite_string *const inf_str)
{
	if (NULL != inf_str)
	{
		inf_str->string[0] = '\0';
		inf_str->used_size = 0;
	}
}

void Clean_inf_str(struct infinite_string *const inf_str)
{
	if (NULL != inf_str)
	{
		for (unsigned int i = 0; i < inf_str->used_size; i++)
		{
			inf_str->string[i] = '\0';
		}
		inf_str->used_size = 0;
	}
}
