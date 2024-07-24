#ifndef CFSDEBUG
#define CFSDEBUG

#include <cfs.h>

void print_file_general_header(CFSFileGeneralHeader *cfs_header);
void print_file_channel_header(CFSFileChannelHeader *channel_header);
void print_variable_header(CFSVariableHeader *header);
void print_variable(CFSVariable *variable);
void print_ds_general_header(CFSDSGeneralHeader *header);
void print_ds_channel_header(CFSDSChannelHeader *header);

#endif
