#include <cfsdebug.h>

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
		case type_INT1:
			printf("Type: INT1\n");
			printf("Variable: %i\n", *(uint8_t *)variable->data);
		break;
		case type_WRD1:
			printf("Type: WRD1\n");
			printf("Variable: %u\n", *(int8_t *)variable->data);
		break;
		case type_INT2:
			printf("Type: INT2\n");
			printf("Variable: %i\n", *(int16_t *)variable->data);
		break;
		case type_WRD2:
			printf("Type: WRD2\n");
			printf("Variable: %u\n", *(uint16_t *)variable->data);
		break;
		case type_INT4:
			printf("Type: INT4\n");
			printf("Variable: %i\n", *(int32_t *)variable->data);
		break;
		case type_RL4:
			printf("Type: RL4\n");
			printf("Variable: %f\n", *(float *)variable->data);
		break;
		case type_RL8:
			printf("Type: RL8\n");
			printf("Variable: %f\n", *(double *)variable->data);
		break;
		case type_LSTR:
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

