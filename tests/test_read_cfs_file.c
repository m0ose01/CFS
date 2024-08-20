#include <cfs.h>
#include <stdio.h>

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
	int success = read_cfs_file(input_file, &cfs_file);
	return success;
}
