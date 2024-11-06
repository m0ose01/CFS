#include <cfsapi.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "test.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("ERROR: File not supplied as argument.\n");
		return 1;
	}
	char *input_file_name = argv[1];

	cfs_short handle = OpenCFSFile(input_file_name, 0, 1);
	void *file_variable_0_value;
	GetVarVal(handle, 0, FILEVAR, 0, file_variable_0_value);

	if (assert_int_equal(*(cfs_short *)file_variable_0_value, 210) != true)
	{
		return 5;
	}
}
