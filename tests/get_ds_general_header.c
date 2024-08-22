#include <cfs.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

	CFSDSGeneralHeader *ds_general_header_0 = get_ds_general_header(&cfs_file, 0);

	uint8_t *blank_memory_block = calloc(sizeof(ds_general_header_0->reserved_space), 1);

	if (memcmp(blank_memory_block, ds_general_header_0->reserved_space, 16) != 0)
	{
		printf("ERROR: Reserved space is not all blank.\n");
		free(blank_memory_block);
		return 3;
	}
	free(blank_memory_block);
}
