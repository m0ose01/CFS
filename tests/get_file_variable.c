#include <cfs.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "test.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("ERROR: File not supplied as argument.\n");
		return 1;
	}
	char *input_file_name = argv[1];
	FILE *input_file = fopen(input_file_name, "rb");
	if (input_file == NULL)
	{
		printf("ERROR: File %s does not exist.\n", input_file_name);
		return 2;
	}

	CFSFile cfs_file;
	read_cfs_file(input_file, &cfs_file);

	CFSVariable *file_variable_0 = get_file_variable(&cfs_file, 0);

	int16_t file_variable_0_value = *(int16_t *)file_variable_0->data;

	assert_int_equal(file_variable_0_value, 210);
}
