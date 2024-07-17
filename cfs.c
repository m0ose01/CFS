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
	CFSGeneralHeader general_header;
	read_general_header(cfs_file, &general_header);
	print_general_header(&general_header);

	const int CHANNEL_COUNT = general_header.channel_count;

	CFSChannelHeader *channel_headers = malloc(sizeof(CFSChannelHeader) * CHANNEL_COUNT);
	if (channel_headers == NULL)
	{
		return 1;
	}
	for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
	{
		read_channel_header(cfs_file, &channel_headers[current_channel]);
		print_channel_header(&channel_headers[current_channel]);
	}

	// We add one because there is a 'system file var to calculate the size of var n-1?'
	const int FILE_VARIABLE_COUNT = general_header.file_variable_count + 1;

	CFSVariableHeader *file_variable_headers = malloc(sizeof(CFSVariableHeader) * FILE_VARIABLE_COUNT);
	for (int current_file_variable_header = 0; current_file_variable_header < FILE_VARIABLE_COUNT; current_file_variable_header++)
	{
		read_variable_header(cfs_file, &file_variable_headers[current_file_variable_header]);
		print_variable_header(&file_variable_headers[current_file_variable_header]);
	}

	const int DATA_SECTION_VARIABLE_COUNT = general_header.data_section_variable_count + 1;

	CFSVariableHeader *data_section_variable_headers = malloc(sizeof(CFSVariableHeader) * DATA_SECTION_VARIABLE_COUNT);

	for (int current_data_section_variable_header = 0; current_data_section_variable_header < DATA_SECTION_VARIABLE_COUNT; current_data_section_variable_header++)
	{
		read_variable_header(cfs_file, &data_section_variable_headers[current_data_section_variable_header]);
		print_variable_header(&data_section_variable_headers[current_data_section_variable_header]);
	}

	const int FILE_VARIABLE_AREA_OFFSET = ftell(cfs_file);

	for (int current_file_variable = 0; current_file_variable < FILE_VARIABLE_COUNT; current_file_variable++)
	{
		CFSVariableHeader current_file_variable_header = file_variable_headers[current_file_variable];
		uint8_t file_variable_size = get_variable_size_bytes(cfs_file, FILE_VARIABLE_AREA_OFFSET, &current_file_variable_header);
		void *file_variable = malloc(file_variable_size);
		read_variable(cfs_file, FILE_VARIABLE_AREA_OFFSET + current_file_variable_header.offset, file_variable_size, file_variable);
		print_variable(file_variable, current_file_variable_header.type);
		free(file_variable);
	}

	const long int POINTER_TABLE_OFFSET = general_header.pointer_table_offset;
	const int DATA_SECTION_COUNT = general_header.data_section_count;

	int32_t *pointer_table = malloc(sizeof(int32_t) * DATA_SECTION_COUNT);

	fseek(cfs_file, POINTER_TABLE_OFFSET, SEEK_SET);
	fread(pointer_table, sizeof(int32_t), DATA_SECTION_COUNT, cfs_file);

	CFSDataSectionHeader *data_section_headers = malloc(sizeof(CFSDataSectionHeader) * DATA_SECTION_COUNT);
	for (int current_data_section_header = 0; current_data_section_header < DATA_SECTION_COUNT; current_data_section_header++)
	{
		printf("%i\n", pointer_table[current_data_section_header]);
		fseek(cfs_file, pointer_table[current_data_section_header], SEEK_SET);
		read_data_section_header(cfs_file, &data_section_headers[current_data_section_header]);
	}

	for (int current_data_section_header = 0; current_data_section_header < DATA_SECTION_COUNT; current_data_section_header++)
	{
		print_data_section_header(&data_section_headers[current_data_section_header]);
	}

	free(channel_headers);

	free(file_variable_headers);

	free(data_section_variable_headers);

	free(pointer_table);

	free(data_section_headers);

	fclose(cfs_file);
}

// Read contents of a CFS file header into a struct.
void read_general_header(FILE *file, CFSGeneralHeader *header)
{
	// Read contents of file header into the struct one by one.
	// This is safer and more portable than reading the header directly into a packed struct.
	fread(&header->file_id, sizeof(header->file_id), 1, file);
	fread(&header->file_name, sizeof(header->file_name), 1, file);
	fread(&header->file_size, sizeof(header->file_size), 1, file);
	fread(&header->file_creation_time, sizeof(header->file_creation_time), 1, file);
	fread(&header->file_creation_date, sizeof(header->file_creation_date), 1, file);
	fread(&header->channel_count, sizeof(header->channel_count), 1, file);
	fread(&header->file_variable_count, sizeof(header->file_variable_count), 1, file);
	fread(&header->data_section_variable_count, sizeof(header->data_section_variable_count), 1, file);
	fread(&header->file_header_size_bytes, sizeof(header->file_header_size_bytes), 1, file);
	fread(&header->data_section_header_size_bytes, sizeof(header->data_section_header_size_bytes), 1, file);
	fread(&header->final_data_section_header_offset, sizeof(header->final_data_section_header_offset), 1, file);
	fread(&header->data_section_count, sizeof(header->data_section_count), 1, file);
	fread(&header->disk_block_size_rounding, sizeof(header->disk_block_size_rounding), 1, file);
	fread(&header->file_comment, sizeof(header->file_comment), 1, file);
	fread(&header->pointer_table_offset, sizeof(header->pointer_table_offset), 1, file);
	fread(&header->reserved_space, sizeof(header->reserved_space), 1, file);
}

// Read contents of a CFS file channel header into a struct.
void read_channel_header(FILE *file, CFSChannelHeader *header)
{
	fread(&header->channel_name, sizeof(header->channel_name), 1, file);
	fread(&header->y_axis_units, sizeof(header->y_axis_units), 1, file);
	fread(&header->x_axis_units, sizeof(header->x_axis_units), 1, file);
	fread(&header->data_type, sizeof(header->data_type), 1, file);
	fread(&header->data_kind, sizeof(header->data_kind), 1, file);
	fread(&header->space_between_elements_bytes, sizeof(header->space_between_elements_bytes), 1, file);
	fread(&header->next_channel, sizeof(header->next_channel), 1, file);
}

void read_variable_header(FILE *file, CFSVariableHeader *header)
{
	fread(&header->description, sizeof(header->description), 1, file);
	fread(&header->type, sizeof(header->type), 1, file);
	fread(&header->units, sizeof(header->units), 1, file);
	fread(&header->offset, sizeof(header->offset), 1, file);
}

// Print out all info in the CFS file header.
void print_general_header(CFSGeneralHeader *header)
{
	printf("File ID: %.*s\n", 8, header->file_id);
	printf("File Name: %s\n", header->file_name);
	printf("File Size: %dKB\n", header->file_size / 1000);
	printf("File Creation Time: %.*s\n", (int)sizeof(header->file_creation_time), header->file_creation_time);
	printf("File Creation Date: %.*s\n", (int)sizeof(header->file_creation_date), header->file_creation_date);
	printf("Channel Count: %d\n", header->channel_count);
	printf("File Variable Count: %d\n", header->file_variable_count);
	printf("Data Section Variable Count: %d\n", header->data_section_variable_count);
	printf("File Header Size: %dKB\n", header->file_header_size_bytes / 1000);
	printf("Data Section Header Size: %dKB\n", header->file_header_size_bytes / 1000);
	printf("Final Data Section Header Offset: 0x%X\n", header->final_data_section_header_offset);
	printf("Number of Data Sections: %d\n", header->data_section_count);
	printf("Disk Block Size Rounding: %d\n", header->disk_block_size_rounding);
	printf("File Comment: %s\n", header->file_comment);
	printf("Pointer Table Offset: 0x%X\n", header->pointer_table_offset);
}

// Print out all info in a CFS file channel header.
void print_channel_header(CFSChannelHeader *header)
{
	printf("Channel Name: '%s'\n", header->channel_name);
	printf("Y Axis Units: '%s'\n", header->y_axis_units);
	printf("X Axis Units: '%s'\n", header->x_axis_units);
	printf("Data Type: %u\n", header->data_type);
	printf("Data Kind: %u\n", header->data_kind);
	printf("Space Between Elements: %i Bytes\n", header->space_between_elements_bytes);
	printf("Next Channel Number: %i\n", header->next_channel);
}

void read_data_section_header(FILE *cfs_file, CFSDataSectionHeader *header)
{
	fread(&header->previous_data_section_offset, sizeof(header->previous_data_section_offset), 1, cfs_file);
	fread(&header->channel_data_offset, sizeof(header->channel_data_offset), 1, cfs_file);
	fread(&header->channel_data_size, sizeof(header->channel_data_size), 1, cfs_file);
	fread(&header->flags, sizeof(header->flags), 1, cfs_file);
	fread(&header->reserved_space, sizeof(header->reserved_space), 1, cfs_file);
}

// Print info for the header of a file variable header or data section variable header.
void print_variable_header(CFSVariableHeader *header)
{
	printf("Variable Description: '%s'\n", header->description);
	printf("Variable Type: %i\n", header->type);
	printf("Variable Units: %s\n", header->units);
	printf("Variable Offset: 0x%X\n", header->offset);
}

void print_variable(void *variable, CFSDataType variable_type)
{
	switch (variable_type)
	{
		case INT1:
			printf("Type: INT1\n");
			printf("File Variable: %i\n", *(uint8_t *)variable);
		break;
		case WRD1:
			printf("Type: WRD1\n");
			printf("File Variable: %u\n", *(int8_t *)variable);
		break;
		case INT2:
			printf("Type: INT2\n");
			printf("File Variable: %i\n", *(int16_t *)variable);
		break;
		case WRD2:
			printf("Type: WRD2\n");
			printf("File Variable: %u\n", *(uint16_t *)variable);
		break;
		case INT4:
			printf("Type: INT4\n");
			printf("File Variable: %i\n", *(int32_t *)variable);
		break;
		case RL4:
			printf("Type: RL4\n");
			printf("File Variable: %f\n", *(float *)variable);
		break;
		case RL8:
			printf("Type: RL8\n");
			printf("File Variable: %f\n", *(float *)variable);
		break;
		case LSTR:
			printf("Type: LSTR\n");
			printf("File Variable: %c\n", *(char *)variable);
		break;
	}

}

void print_data_section_header(CFSDataSectionHeader *header)
{
	printf("Previous Data Section Offset: 0x%X\n", header->previous_data_section_offset);
	printf("Channel Data Offset: 0x%X\n", header->previous_data_section_offset);
	printf("Size of Channel Data Area: %i bytes\n", header->previous_data_section_offset);
	printf("Data Section Flag: %i\n", header->flags);
}

// Read a file/data section variable into the variable pointed to by /variable'. The user must ensure
// that sufficient space is allocated for the variable.
void read_variable(FILE *cfs_file, long int offset, uint8_t variable_size_bytes, void *dest)
{
	fseek(cfs_file, offset, SEEK_SET);
	int x = fread(dest, variable_size_bytes, 1, cfs_file);
}

int get_variable_size_bytes(FILE *cfs_file, long int variable_area_offset, CFSVariableHeader *variable_header)
{
	fseek(cfs_file, variable_area_offset, SEEK_SET);
	fseek(cfs_file, variable_header->offset, SEEK_CUR);
	long int file_variable_start = ftell(cfs_file);

	CFSDataType variable_type = variable_header->type;
	uint8_t variable_size = 0;
	switch (variable_type)
	{
		case INT1:
		case WRD1:
			variable_size = sizeof(int8_t);
			break;
		case INT2:
		case WRD2:
			variable_size = sizeof(int16_t);
			break;
		case INT4:
		case RL4:
			variable_size = sizeof(float);
			break;
		case RL8:
			variable_size = sizeof(double);
			break;
		case LSTR:
			// In the case of the 'LSTR' data type, the size of the string
			// is stored in the first byte of the variable.
			fread(&variable_size, sizeof(uint8_t), 1, cfs_file);
			break;
	}
	return variable_size;
}
