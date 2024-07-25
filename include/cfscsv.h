#ifndef CFSCSV
#define CFSCSV

#include <stdio.h>
#include <cfs.h>

int write_csv(CFSFile *file, FILE *csv_file);
int write_int2(void *data, int point, float y_scale, float y_offset, char *buffer);
int write_rl8(void *data, int point, float y_scale, float y_offset, char *buffer);

#endif
