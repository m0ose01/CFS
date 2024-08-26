#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cfs.h>

// NOTE: 'ds' stands for 'data section' See CFS manual for details.

size_t fread_string(char buffer[], size_t size, size_t count, FILE *restrict stream)
{
	uint8_t string_size;
	fread(&string_size, sizeof(string_size), 1, stream);
	buffer[string_size - 1] = '\0';
	return fread(buffer, size, count, stream);
}

size_t fread_char(char buffer[], size_t size, size_t count, FILE *restrict stream)
{
	buffer[size - 1] = '\0';
	return fread(buffer, size - 1, count, stream);
}

// Read contents of a CFS file header into a struct.
void read_file_general_header(FILE *file, CFSFileGeneralHeader *header)
{
	// Read contents of file header into the struct one by one.
	// This is safer and more portable than reading the header directly into a packed struct.
	fread_char(header->file_id, sizeof(header->file_id), 1, file);
	fread_string(header->file_name, sizeof(header->file_name), 1, file);
	fread(&header->file_size, sizeof(header->file_size), 1, file);
	fread_char(header->file_creation_time, sizeof(header->file_creation_time), 1, file);
	fread_char(header->file_creation_date, sizeof(header->file_creation_date), 1, file);
	fread(&header->channel_count, sizeof(header->channel_count), 1, file);
	fread(&header->file_variable_count, sizeof(header->file_variable_count), 1, file);
	fread(&header->data_section_variable_count, sizeof(header->data_section_variable_count), 1, file);
	fread(&header->file_header_size_bytes, sizeof(header->file_header_size_bytes), 1, file);
	fread(&header->data_section_header_size_bytes, sizeof(header->data_section_header_size_bytes), 1, file);
	fread(&header->final_data_section_header_offset, sizeof(header->final_data_section_header_offset), 1, file);
	fread(&header->data_section_count, sizeof(header->data_section_count), 1, file);
	fread(&header->disk_block_size_rounding, sizeof(header->disk_block_size_rounding), 1, file);
	fread_string(header->file_comment, sizeof(header->file_comment), 1, file);
	fread(&header->pointer_table_offset, sizeof(header->pointer_table_offset), 1, file);
	fread(&header->reserved_space, sizeof(header->reserved_space), 1, file);
}

// Read contents of a CFS file channel header into a struct.
void read_file_channel_header(FILE *file, CFSFileChannelHeader *header)
{
	fread_string(header->channel_name, sizeof(header->channel_name), 1, file);
	fread_string(header->y_axis_units, sizeof(header->y_axis_units), 1, file);
	fread_string(header->x_axis_units, sizeof(header->x_axis_units), 1, file);
	fread(&header->data_type, sizeof(header->data_type), 1, file);
	fread(&header->data_kind, sizeof(header->data_kind), 1, file);
	fread(&header->space_between_elements_bytes, sizeof(header->space_between_elements_bytes), 1, file);
	fread(&header->next_channel, sizeof(header->next_channel), 1, file);
}

void read_variable_header(FILE *file, CFSVariableHeader *header)
{
	fread_string(header->description, sizeof(header->description), 1, file);
	fread(&header->type, sizeof(header->type), 1, file);
	fread_string(header->units, sizeof(header->units), 1, file);
	fread(&header->offset, sizeof(header->offset), 1, file);
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

int read_variable(FILE *cfs_file, CFSVariableHeader *header, CFSVariable *variable)
{
	fseek(cfs_file, header->offset, SEEK_CUR);
	uint8_t variable_size = (uint8_t)get_variable_size((CFSDataType)header->type);
	if (header->type == type_LSTR)
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
		case type_INT1:
		case type_WRD1:
			variable_size = sizeof(int8_t);
		break;
		case type_INT2:
		case type_WRD2:
			variable_size = sizeof(int16_t);
		break;
		case type_INT4:
		case type_RL4:
			variable_size = sizeof(float);
		break;
		case type_RL8:
			variable_size = sizeof(double);
		break;
		case type_LSTR:
			variable_size = sizeof(char *);
		break;
	}
	return variable_size;
}

int read_channel_data(FILE *cfs_file, CFSFileChannelHeader *file_header, CFSDSChannelHeader *ds_header, CFSChannelData *channel_data)
{
	switch (file_header->data_type)
	{
		case type_INT2:
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
	int points_read = 0;
	if (SPACE_BETWEEN_POINTS == VARIABLE_SIZE)
	{
		points_read = fread(channel_data->data, VARIABLE_SIZE, POINTS_COUNT, cfs_file);
	}
	else
	{
		for (int current_point = 0; current_point < POINTS_COUNT; current_point++) // Note: I currently have no file to test this path
		{
			points_read += fread(&((int16_t *)(channel_data->data))[current_point], VARIABLE_SIZE, 1, cfs_file);
			fseek(cfs_file, SPACE_BETWEEN_POINTS - VARIABLE_SIZE, SEEK_CUR);
		}
	}
	channel_data->data_points_count = points_read;
	channel_data->data_type = file_header->data_type;
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

	if (strncmp(file->header->general_header->file_id, "CEDFILE\"", sizeof(file->header->general_header->file_id) - 1) != 0)
	{
		printf("File is not a valid CFS v2 file.\n");
		return -2;
	}
	
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	const int DS_COUNT = file->header->general_header->data_section_count;
	const int DS_VAR_COUNT = file->header->general_header->data_section_variable_count;
	const int FILE_VAR_COUNT = file->header->general_header->file_variable_count;
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
	const int DS_VAR_COUNT = file->header->general_header->data_section_variable_count;
	const int FILE_VAR_COUNT = file->header->general_header->file_variable_count;

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

CFSFileChannelHeader *get_file_channel_header(CFSFile *file, int channel)
{
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	if (channel >= CHANNEL_COUNT)
	{
		return NULL;
	}
	return &file->header->channel_headers[channel];
}

CFSDSChannelHeader *get_ds_channel_header(CFSFile *file, int channel, int data_section)
{
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	const int DS_COUNT = file->header->general_header->data_section_count;
	if (channel >= CHANNEL_COUNT)
	{
		return NULL;
	}
	if (data_section >= DS_COUNT)
	{
		return NULL;
	}
	int idx = data_section + (channel * DS_COUNT);
	return &file->data_sections->header->channel_headers[idx];
}

CFSDSGeneralHeader *get_ds_general_header(CFSFile *file, int data_section)
{
	const int DS_COUNT = file->header->general_header->data_section_count;
	if (data_section >= DS_COUNT)
	{
		return NULL;
	}
	return &file->data_sections->header->general_header[data_section];
}

CFSVariableHeader *get_file_variable_header(CFSFile *file, int file_variable)
{
	const int FILE_VAR_COUNT = file->header->general_header->file_variable_count;
	if (file_variable >= FILE_VAR_COUNT)
	{
		return NULL;
	}
	return &file->header->file_variable_headers[file_variable];
}

CFSVariableHeader *get_ds_variable_header(CFSFile *file, int ds_variable)
{
	const int DS_VAR_COUNT = file->header->general_header->data_section_variable_count;
	if (ds_variable >= DS_VAR_COUNT)
	{
		return NULL;
	}
	return &file->header->ds_variable_headers[ds_variable];
}

CFSVariable *get_file_variable(CFSFile *file, int file_variable)
{
	const int FILE_VAR_COUNT = file->header->general_header->file_variable_count;
	if (file_variable >= FILE_VAR_COUNT)
	{
		return NULL;
	}
	return &file->header->file_variables[file_variable];
}

CFSVariable *get_ds_variable(CFSFile *file, int data_section, int ds_variable)
{
	const int DS_COUNT = file->header->general_header->data_section_variable_count;
	const int DS_VAR_COUNT = file->header->general_header->data_section_variable_count;
	if (data_section >= DS_COUNT)
	{
		return NULL;
	}
	if (ds_variable >= DS_VAR_COUNT)
	{
		return NULL;
	}
	int idx = data_section + (ds_variable * DS_COUNT);
	return &file->data_sections->header->ds_variables[idx];
}

CFSChannelData *get_channel_data(CFSFile *file, int channel, int data_section)
{
	int CHANNEL_COUNT = file->header->general_header->channel_count;
	int DS_COUNT = file->header->general_header->data_section_count;
	if (channel >= CHANNEL_COUNT)
	{
		return NULL;
	}
	if (data_section >= DS_COUNT)
	{
		return NULL;
	}
	int idx = data_section + (channel * DS_COUNT);
	return &file->data_sections->channel_data[idx];
}
