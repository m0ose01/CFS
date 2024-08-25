#include <cfs.h>
#include <cfsapi.h>
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

	cfs_short handle = OpenCFSFile(input_file_name, 0, 1);
	if (handle < 0)
	{
		printf("ERROR: Got invalid file handle %d\n", handle);
		return 3;
	}

	CloseCFSFile(handle);
}
