#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "cfs.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: cfs.c <filename>\n");
		return 1;
	}
	char *file_name = argv[1];
	FILE *cfs_file = fopen(file_name, "r");
	CFSHeader *cfs_header = read_cfs_header(cfs_file);
	print_cfs_header(cfs_header);

	free(cfs_header);
	fclose(cfs_file);
}

// Read contents of a CFS file header into a struct.
CFSHeader *read_cfs_header(FILE *file)
{
	CFSHeader *cfs_header = malloc(sizeof(CFSHeader));

	// Read contents of file header into the struct one by one.
	// This is safer and more portable than reading the header directly into a packed struct.
	fread(&cfs_header->file_id, sizeof(cfs_header->file_id), 1, file);
	fread(&cfs_header->file_name, sizeof(cfs_header->file_name), 1, file);
	fread(&cfs_header->file_size, sizeof(cfs_header->file_size), 1, file);
	fread(&cfs_header->file_creation_time, sizeof(cfs_header->file_creation_time), 1, file);
	fread(&cfs_header->file_creation_date, sizeof(cfs_header->file_creation_date), 1, file);
	fread(&cfs_header->channel_count, sizeof(cfs_header->channel_count), 1, file);
	fread(&cfs_header->file_variable_count, sizeof(cfs_header->file_variable_count), 1, file);
	fread(&cfs_header->data_section_variable_count, sizeof(cfs_header->data_section_variable_count), 1, file);
	fread(&cfs_header->file_header_size_bytes, sizeof(cfs_header->file_header_size_bytes), 1, file);
	fread(&cfs_header->data_section_header_size_bytes, sizeof(cfs_header->data_section_header_size_bytes), 1, file);
	fread(&cfs_header->final_data_section_header_offset, sizeof(cfs_header->final_data_section_header_offset), 1, file);
	fread(&cfs_header->data_section_count, sizeof(cfs_header->data_section_count), 1, file);
	fread(&cfs_header->disk_block_size_rounding, sizeof(cfs_header->disk_block_size_rounding), 1, file);
	fread(&cfs_header->file_comment, sizeof(cfs_header->file_comment), 1, file);
	fread(&cfs_header->pointer_table_offset, sizeof(cfs_header->pointer_table_offset), 1, file);
	fread(&cfs_header->reserved_space, sizeof(cfs_header->reserved_space), 1, file);

	return cfs_header;
}

// Print out all info in the CFS file header.
void print_cfs_header(CFSHeader *cfs_header)
{

	printf("File ID: %.*s\n", 8, cfs_header->file_id);
	printf("File Name: %s\n", cfs_header->file_name);
	printf("File Size: %dKB\n", cfs_header->file_size / 1000);
	printf("File Creation Time: %.*s\n", (int)sizeof(cfs_header->file_creation_time), cfs_header->file_creation_time);
	printf("File Creation Date: %.*s\n", (int)sizeof(cfs_header->file_creation_date), cfs_header->file_creation_date);
	printf("Channel Count: %d\n", cfs_header->channel_count);
	printf("File Variable Count: %d\n", cfs_header->file_variable_count);
	printf("Data Section Variable Count: %d\n", cfs_header->data_section_variable_count);
	printf("File Header Size: %dKB\n", cfs_header->file_header_size_bytes / 1000);
	printf("Data Section Header Size: %dKB\n", cfs_header->file_header_size_bytes / 1000);
	printf("Final Data Section Header Offset: 0x%X\n", cfs_header->final_data_section_header_offset);
	printf("Number of Data Sections: %d\n", cfs_header->data_section_count);
	printf("Disk Block Size Rounding: %d\n", cfs_header->disk_block_size_rounding);
	printf("File Comment: %s\n", cfs_header->file_comment);
	printf("Pointer Table Offset: 0x%X\n", cfs_header->pointer_table_offset);
}
