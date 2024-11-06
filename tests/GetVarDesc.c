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
	cfs_short varSize;
	TDataType varType;
	TUnits units;
	TDesc about;
	GetVarDesc(handle, 0, FILEVAR, &varSize, &varType, units, about);


	if (assert_int_equal(varSize, sizeof(cfs_short)) != true)
	{
		return 5;
	}

	if (assert_int_equal(varType, INT2) != true)
	{
		return 6;
	}

	if (assert_str_equal(units, "SIMPLEW") != true)
	{
		return 7;
	}

	if (assert_str_equal(about, "CED example Program") != true)
	{
		return 8;
	}
}
