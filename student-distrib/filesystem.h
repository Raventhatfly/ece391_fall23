#ifndef FILESYSTEM_H
#define FILESYSTEM_H

extern int* filesys_base;
void read_dentry_by_name(char* file_name);
void read_dentry_by_index(int index);
void read_data(); 


#endif