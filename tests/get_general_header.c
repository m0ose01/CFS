#include <cfs.h>
#include <stdio.h>
#include <string.h>

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

	CFSFileGeneralHeader *general_header = cfs_file.header->general_header;
	if (strncmp(general_header->file_id, "CEDFILE\"", sizeof(general_header->file_id) - 1) != 0)
	{
		printf("ERROR: File not correctly read as CFS File.\n");
		return 3;
	}

	if (general_header->channel_count != 2)
	{
		printf("ERROR: Incorrect number of channels read.\n");
		return 4;
	}

	if (general_header->data_section_count != 3)
	{
		printf("ERROR: Incorrect number of data_sections read.\n");
		return 5;
	}
	return 0;
}
