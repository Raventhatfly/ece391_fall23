#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "lib.h"

void filesystem_init(int* base_ptr);
void read_dentry_by_name(char* file_name);
void read_dentry_by_index(int index);
void read_data(); 

typedef  enum file_type{
    USER_ACCESS_RTC_FILE,
    FILE_DIRECTORY,
    REGULAR_FILE
}file_type_t;

typedef struct dentry{
    uint8_t file_name[32];
    uint32_t file_type;
    uint32_t inode_num;
    uint8_t reserved[24];
}dentry_t;

typedef struct boot_block{
    uint32_t dir_cnt;
    uint32_t inode_cnt;
    uint32_t data_cnt;
    uint8_t reserved[52];
    dentry_t direntries[63];
}boot_block_t;

typedef struct inode{
    uint32_t length;
    uint32_t data_block_num[1023];
}inode_t;


#endif
