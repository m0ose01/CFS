#include <stdint.h>
#include <stdio.h>

typedef enum
{
	INT1 = 0,
	WRD1 = 1,
	INT2 = 2,
	WRD2 = 3,
	INT4 = 4,
	RL4 = 5,
	RL8 = 6,
	LSTR = 7,
} CFSDataType;

typedef enum
{
	EQUALSPACED = 0,
	MATRIX = 1,
	SUBSIDIARY = 2,
} CFSDataKind;

typedef struct
{
	char file_id[8];
	char file_name[14];
	int32_t file_size;
	char file_creation_time[8];
	char file_creation_date[8];
	int16_t channel_count;
	int16_t file_variable_count;
	int16_t data_section_variable_count;
	int16_t file_header_size_bytes;
	int16_t data_section_header_size_bytes;
	int32_t final_data_section_header_offset;
	uint16_t data_section_count;
	uint16_t disk_block_size_rounding;
	char file_comment[74];
	int32_t pointer_table_offset;
	uint8_t reserved_space[40];
} CFSFileGeneralHeader;

typedef struct
{
	char channel_name[22];
	char y_axis_units[10];
	char x_axis_units[10];
	uint8_t data_type;
	uint8_t data_kind;
	int16_t space_between_elements_bytes;
	int16_t next_channel;
} CFSFileChannelHeader;

// Header for EITHER file variables or data section variables.
typedef struct
{
	char description[22];
	uint16_t type;
	char units[10];
	int16_t offset; // Offset from the start of the data section variable area, or the file variable area.
} CFSVariableHeader;

typedef struct
{
	CFSDataType data_type;
	int32_t data_points_count;
	void *data;
} CFSChannelData;

typedef struct
{
	CFSDataType data_type;
	void *data;
} CFSVariable;

typedef struct
{
	int32_t previous_data_section_offset;
	int32_t channel_data_offset;
	int32_t channel_data_size;
	uint16_t flags;
	uint8_t reserved_space[16];

} CFSDSGeneralHeader;

typedef struct
{
	int32_t first_byte_offset;
	int32_t data_points_count;
	float y_scale;
	float y_offset;
	float x_increment;
	float x_offset;

} CFSDSChannelHeader;

void print_file_general_header(CFSFileGeneralHeader *cfs_header);
void print_file_channel_header(CFSFileChannelHeader *channel_header);
void print_variable_header(CFSVariableHeader *header);
void print_variable(CFSVariable *variable);
void print_ds_general_header(CFSDSGeneralHeader *header);
void print_ds_channel_header(CFSDSChannelHeader *header);
void read_ds_channel_header(FILE *cfs_file, CFSDSChannelHeader *header);
void read_file_general_header(FILE *file, CFSFileGeneralHeader *cfs_header);
void read_file_channel_header(FILE *file, CFSFileChannelHeader *channel_header);
void read_variable_header(FILE *file, CFSVariableHeader *header);
void read_variable(FILE *cfs_file, long int offset, uint8_t variable_size_bytes, void *dest);
void read_ds_general_header(FILE *cfs_file, CFSDSGeneralHeader *header);
int read_channel_data(FILE *cfs_file, long int offset, CFSDataType data_type, int16_t space_between_points_bytes, int data_points_count, void *dest);
int read_int2_channel_data(FILE *cfs_file, long int offset, int16_t space_between_points_bytes, int data_points_count, int16_t *dest);
int get_variable_size_string(FILE *cfs_file, long int variable_area_offset, CFSVariableHeader *variable_header);
int get_variable_size(CFSDataType *type);

