#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "lib.h"

void filesystem_init(int* base_ptr);
void read_dentry_by_name(char* file_name);
void read_dentry_by_index(int index);
void read_data(); 


#endif