#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "./cfs.h"

// NOTE: 'ds' stands for 'data section' See CFS manual for details.

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: cfs.c <filename>\n");
		return 1;
	}
	char *file_name = argv[1];
	FILE *cfs_file = fopen(file_name, "r");

	CFSFile *file = malloc(sizeof(CFSFile));
	if (file == NULL)
	{
		return 1;
	}
	read_cfs_file(cfs_file, file);

	FILE *csv_file_test = fopen("csv_test.csv", "w");
	write_csv_int2(file, csv_file_test);
	fclose(csv_file_test);

	free_cfs_file(file);
	free(file);
}

// Read contents of a CFS file header into a struct.
void read_file_general_header(FILE *file, CFSFileGeneralHeader *header)
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
void read_file_channel_header(FILE *file, CFSFileChannelHeader *header)
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
	uint8_t string_size;
	fread(&string_size, sizeof(string_size), 1, file);
	fread(&header->description, sizeof(header->description) - 1, 1, file);
	header->description[string_size] = '\0';
	fread(&header->type, sizeof(header->type), 1, file);
	fread(&string_size, sizeof(string_size), 1, file);
	fread(&header->units, sizeof(header->units) - 1, 1, file);
	header->units[string_size] = '\0';
	fread(&header->offset, sizeof(header->offset), 1, file);
}

// Print out all info in the CFS file header.
void print_file_general_header(CFSFileGeneralHeader *header)
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
void print_file_channel_header(CFSFileChannelHeader *header)
{
	printf("Channel Name: '%s'\n", header->channel_name);
	printf("Y Axis Units: '%s'\n", header->y_axis_units);
	printf("X Axis Units: '%s'\n", header->x_axis_units);
	printf("Data Type: %u\n", header->data_type);
	printf("Data Kind: %u\n", header->data_kind);
	printf("Space Between Elements: %i Bytes\n", header->space_between_elements_bytes);
	printf("Next Channel Number: %i\n", header->next_channel);
}

void read_ds_general_header(FILE *cfs_file, CFSDSGeneralHeader *header)
{
	fread(&header->previous_data_section_offset, sizeof(header->previous_data_section_offset), 1, cfs_file);
	fread(&header->channel_data_offset, sizeof(header->channel_data_offset), 1, cfs_file);
	fread(&header->channel_data_size, sizeof(header->channel_data_size), 1, cfs_file);
	fread(&header->flags, sizeof(header->flags), 1, cfs_file);
	fread(&header->reserved_space, sizeof(header->reserved_space), 1, cfs_file);
}

void read_ds_channel_header(FILE *cfs_file, CFSDSChannelHeader *header)
{
	fread(&header->first_byte_offset, sizeof(header->first_byte_offset), 1, cfs_file);
	fread(&header->data_points_count, sizeof(header->data_points_count), 1, cfs_file);
	fread(&header->y_scale, sizeof(header->y_scale), 1, cfs_file);
	fread(&header->y_offset, sizeof(header->y_offset), 1, cfs_file);
	fread(&header->x_increment, sizeof(header->x_increment), 1, cfs_file);
	fread(&header->x_offset, sizeof(header->x_offset), 1, cfs_file);
}

// Print info for the header of a file variable header or data section variable header.
void print_variable_header(CFSVariableHeader *header)
{
	printf("Variable Description: '%s'\n", header->description);
	printf("Variable Type: %i\n", header->type);
	printf("Variable Units: %s\n", header->units);
	printf("Variable Offset: 0x%X\n", header->offset);
}

void print_variable(CFSVariable *variable)
{
	switch (variable->data_type)
	{
		case INT1:
			printf("Type: INT1\n");
			printf("Variable: %i\n", *(uint8_t *)variable->data);
		break;
		case WRD1:
			printf("Type: WRD1\n");
			printf("Variable: %u\n", *(int8_t *)variable->data);
		break;
		case INT2:
			printf("Type: INT2\n");
			printf("Variable: %i\n", *(int16_t *)variable->data);
		break;
		case WRD2:
			printf("Type: WRD2\n");
			printf("Variable: %u\n", *(uint16_t *)variable->data);
		break;
		case INT4:
			printf("Type: INT4\n");
			printf("Variable: %i\n", *(int32_t *)variable->data);
		break;
		case RL4:
			printf("Type: RL4\n");
			printf("Variable: %f\n", *(float *)variable->data);
		break;
		case RL8:
			printf("Type: RL8\n");
			printf("Variable: %f\n", *(double *)variable->data);
		break;
		case LSTR:
			printf("Type: LSTR\n");
			printf("Variable: %c\n", *(char *)variable->data);
		break;
	}

}

void print_ds_general_header(CFSDSGeneralHeader *header)
{
	printf("Previous Data Section Offset: 0x%X\n", header->previous_data_section_offset);
	printf("Channel Data Offset: 0x%X\n", header->previous_data_section_offset);
	printf("Size of Channel Data Area: %i bytes\n", header->previous_data_section_offset);
	printf("Data Section Flag: %i\n", header->flags);
}

void print_ds_channel_header(CFSDSChannelHeader *header)
{
	printf("Offset to First Byte: 0x%X\n", header->first_byte_offset);
	printf("Data Points: %i\n", header->data_points_count);
	printf("Y Scale: %f\n", header->y_scale);
	printf("Y Offset: %f\n", header->y_offset);
	printf("X Increment: %f\n", header->x_increment);
	printf("X Offset: %f\n", header->x_offset);
}

int read_variable(FILE *cfs_file, CFSVariableHeader *header, CFSVariable *variable)
{
	fseek(cfs_file, header->offset, SEEK_CUR);
	uint8_t variable_size = (uint8_t)get_variable_size((CFSDataType)header->type);
	if (header->type == LSTR)
	{
		fread(&variable_size, sizeof(uint8_t), 1, cfs_file);
	}
	variable->data = malloc(variable_size);
	if (variable->data == NULL)
	{
		return -1;
	}
	fread(variable->data, variable_size, 1, cfs_file);
	variable->data_type = header->type;
	return 0;
}

int get_variable_size(CFSDataType type)
{
	int variable_size = 0;
	switch (type)
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
			variable_size = sizeof(char *);
		break;
	}
	return variable_size;
}

int read_channel_data(FILE *cfs_file, CFSFileChannelHeader *file_header, CFSDSChannelHeader *ds_header, CFSChannelData *channel_data)
{
	switch (file_header->data_type)
	{
		case INT2:
			return read_int2_channel_data(cfs_file, file_header, ds_header, channel_data);
		default:
			break;
	}
	return 0;
}

int read_int2_channel_data(FILE *cfs_file, CFSFileChannelHeader *file_header, CFSDSChannelHeader *ds_header, CFSChannelData *channel_data)
{
	const int SPACE_BETWEEN_POINTS = file_header->space_between_elements_bytes;
	const int VARIABLE_SIZE = sizeof(int16_t);
	const int POINTS_COUNT = ds_header->data_points_count;
	channel_data->data = malloc(VARIABLE_SIZE * POINTS_COUNT);
	if (channel_data->data == NULL)
	{
		return -1;
	}
	if (SPACE_BETWEEN_POINTS == VARIABLE_SIZE)
	{
		int points_read = fread(channel_data->data, VARIABLE_SIZE, POINTS_COUNT, cfs_file);
		channel_data->data_points_count = points_read;
		channel_data->data_type = file_header->data_type;
	}
	else
	{
		for (int current_point = 0; current_point < POINTS_COUNT; current_point++) // Note: I currently have no file to test this path
		{
			fread(&((int16_t *)(channel_data->data))[current_point], VARIABLE_SIZE, 1, cfs_file);
			fseek(cfs_file, SPACE_BETWEEN_POINTS - VARIABLE_SIZE, SEEK_CUR);
		}
	}
	return 0;
}

int read_cfs_file(FILE *cfs_file, CFSFile *file)
{
	file->header = malloc(sizeof(CFSFileHeader));
	if (file->header == NULL)
	{
		return -1;
	}

	file->header->general_header = malloc(sizeof(CFSFileGeneralHeader));
	if (file->header->general_header == NULL)
	{
		return -1;
	}

	read_file_general_header(cfs_file, file->header->general_header);
	
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	const int DS_COUNT = file->header->general_header->data_section_count;
	const int DS_VAR_COUNT = file->header->general_header->data_section_variable_count + 1; // We add +1 as there is an extra 'system file variable'
	const int FILE_VAR_COUNT = file->header->general_header->file_variable_count + 1; // See above
	const int POINTER_TABLE_OFFSET = file->header->general_header->pointer_table_offset;

	file->header->channel_headers = malloc(sizeof(CFSFileChannelHeader) * CHANNEL_COUNT);
	if (file->header->channel_headers == NULL)
	{
		return -1;
	}

	for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
	{
		read_file_channel_header(cfs_file, &file->header->channel_headers[current_channel]);
	}

	file->header->file_variable_headers = malloc(sizeof(CFSVariableHeader) * FILE_VAR_COUNT);
	if (file->header->file_variable_headers == NULL)
	{
		return -1;
	}

	for (int current_file_var = 0; current_file_var < FILE_VAR_COUNT; current_file_var++)
	{
		read_variable_header(cfs_file, &file->header->file_variable_headers[current_file_var]);
	}

	file->header->ds_variable_headers = malloc(sizeof(CFSVariableHeader) * DS_VAR_COUNT);
	if (file->header->ds_variable_headers == NULL)
	{
		return -1;
	}

	for (int current_ds_var = 0; current_ds_var < DS_VAR_COUNT; current_ds_var++)
	{
		read_variable_header(cfs_file, &file->header->ds_variable_headers[current_ds_var]);
	}

	file->header->file_variables = malloc(sizeof(CFSVariable) * FILE_VAR_COUNT);
	if (file->header->file_variables == NULL)
	{
		return -1;
	}

	const int FILE_VAR_AREA_OFFSET = ftell(cfs_file);

	for (int current_file_var = 0; current_file_var < FILE_VAR_COUNT; current_file_var++)
	{
		fseek(cfs_file, FILE_VAR_AREA_OFFSET, SEEK_SET);
		CFSVariableHeader *current_file_var_header = &file->header->file_variable_headers[current_file_var];
		int err = read_variable(cfs_file, current_file_var_header, &file->header->file_variables[current_file_var]);
		if (err != 0)
		{
			return err;
		}
	}

	file->pointer_table = malloc(sizeof(int32_t) * DS_COUNT);
	if (file->pointer_table == NULL)
	{
		return -1;
	}

	fseek(cfs_file, POINTER_TABLE_OFFSET, SEEK_SET);
	fread(file->pointer_table, sizeof(int32_t) * DS_COUNT, 1, cfs_file);

	file->data_sections = malloc(sizeof(CFSDataSection));
	if (file->data_sections == NULL)
	{
		return -1;
	}

	file->data_sections->header = malloc(sizeof(CFSDSHeader));
	if (file->data_sections->header == NULL)
	{
		return -1;
	}

	file->data_sections->header->general_header = malloc(sizeof(CFSDSGeneralHeader) * DS_COUNT);
	if (file->data_sections->header->general_header == NULL)
	{
		return -1;
	}

	file->data_sections->header->channel_headers = malloc(sizeof(CFSDSChannelHeader) * DS_COUNT * CHANNEL_COUNT);
	if (file->data_sections->header->general_header == NULL)
	{
		return -1;
	}

	file->data_sections->header->ds_variables = malloc(sizeof(CFSVariable) * DS_COUNT * DS_VAR_COUNT);
	if (file->data_sections->header->ds_variables == NULL)
	{
		return -1;
	}

	file->data_sections->channel_data = malloc(sizeof(CFSChannelData) * DS_COUNT * CHANNEL_COUNT);
	if (file->data_sections->channel_data == NULL)
	{
		return -1;
	}

	for (int current_ds = 0; current_ds < DS_COUNT; current_ds++)
	{
		int32_t current_ds_offset = file->pointer_table[current_ds];
		fseek(cfs_file, current_ds_offset, SEEK_SET);
		read_ds_general_header(cfs_file, &file->data_sections->header->general_header[current_ds]);

		for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
		{
			int idx = current_ds + (current_channel * DS_COUNT);
			read_ds_channel_header(cfs_file, &file->data_sections->header->channel_headers[idx]);
		}

		for (int current_ds_variable = 0; current_ds_variable < DS_VAR_COUNT; current_ds_variable++)
		{
			int idx = current_ds + (current_ds_variable * DS_COUNT);
			CFSVariableHeader *current_ds_variable_header = &file->header->ds_variable_headers[current_ds_variable];
			read_variable(cfs_file, current_ds_variable_header, &file->data_sections->header->ds_variables[idx]);
		}

		for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
		{
			int idx = current_ds + (current_channel * DS_COUNT);
			CFSFileChannelHeader *current_file_channel_header = &file->header->channel_headers[current_channel];
			CFSDSChannelHeader *current_ds_channel_header = &file->data_sections->header->channel_headers[idx];
			CFSDSGeneralHeader *current_ds_general_header = &file->data_sections->header->general_header[current_ds];

			int channel_data_offset = current_ds_general_header->channel_data_offset + current_ds_channel_header->first_byte_offset;
			fseek(cfs_file, channel_data_offset, SEEK_SET);
			
			read_channel_data(cfs_file, current_file_channel_header, current_ds_channel_header, &file->data_sections->channel_data[idx]);
		}
	}

	return 0;
}

void free_cfs_file(CFSFile *file)
{
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	const int DS_COUNT = file->header->general_header->data_section_count;
	const int DS_VAR_COUNT = file->header->general_header->data_section_variable_count + 1; // We add +1 as there is an extra 'system file variable'
	const int FILE_VAR_COUNT = file->header->general_header->file_variable_count + 1; // See above

	free(file->header->general_header);
	free(file->header->channel_headers);
	free(file->header->file_variable_headers);
	free(file->header->ds_variable_headers);
	for (int current_filevar = 0; current_filevar < FILE_VAR_COUNT; current_filevar++)
	{
		free(file->header->file_variables[current_filevar].data);
	}
	free(file->header->file_variables);
	free(file->header);

	free(file->pointer_table);

	free(file->data_sections->header->general_header);
	free(file->data_sections->header->channel_headers);
	for (int current_ds = 0; current_ds < DS_COUNT; current_ds++)
	{
		for (int current_ds_var = 0; current_ds_var < DS_VAR_COUNT; current_ds_var++)
		{
			int idx = current_ds + (current_ds_var * DS_COUNT);
			free(file->data_sections->header->ds_variables[idx].data);
		}
	}
	free(file->data_sections->header->ds_variables);
	free(file->data_sections->header);

	for (int current_ds = 0; current_ds < DS_COUNT; current_ds++)
	{
		for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
		{
			int idx = current_ds + (current_channel * DS_COUNT);
			free(file->data_sections->channel_data[idx].data);
		}
	}
	free(file->data_sections->channel_data);
	free(file->data_sections);
}

int write_csv_int2(CFSFile *file, FILE *csv_file)
{
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	const int DS_COUNT = file->header->general_header->data_section_count;

	fprintf(csv_file, "channel,data_section,x,y\n");
	for (int current_ds = 0; current_ds < DS_COUNT; current_ds++)
	{
		for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
		{
			if (file->data_sections->channel_data[current_channel].data_type != INT2)
			{
				continue;
			}

			int idx = current_ds + (current_channel * DS_COUNT);
			CFSDSChannelHeader *current_ds_channel_header = &file->data_sections->header->channel_headers[idx];
			const float y_scale = current_ds_channel_header->y_scale;
			const float y_offset = current_ds_channel_header->y_scale;
			const float x_offset = current_ds_channel_header->x_offset;
			const float x_increment = current_ds_channel_header->x_increment;

			const int POINTS_COUNT = file->data_sections->channel_data[idx].data_points_count; // We must read points from the channel data itself, in case some points were not written successfully.
			CFSChannelData *current_channel_data = &file->data_sections->channel_data[idx];
			for (int current_point = 0; current_point < POINTS_COUNT; current_point++)
			{
				float current_x = (x_offset + current_point) * x_increment;
				float current_y = y_scale * (((int16_t *)current_channel_data->data)[current_point] + y_offset);
				fprintf(csv_file, "%i,%i,%f,%f\n", current_channel,current_ds,current_x,current_y);
			}
		}
	}
	return 0;
}
