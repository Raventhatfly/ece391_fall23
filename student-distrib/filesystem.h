#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "lib.h"

#define FILENAME_LEN 32
#define BLOCK_SIZE 4096

typedef struct dentry{
    uint8_t file_name[FILENAME_LEN];
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
typedef struct data_block
{
    uint8_t data[BLOCK_SIZE];
}data_block_t;

void filesystem_init(int* base_ptr);
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
int32_t directory_read(uint32_t index,uint8_t* buf, dentry_t* dentry);
int32_t file_open(const uint8_t *fname);
int32_t file_read(int32_t fd);
int32_t file_write(int32_t fd);
int32_t file_close(int32_t fd);
int32_t dir_open(const uint8_t *fname);
int32_t dir_read(int32_t fd,uint8_t* buf);
int32_t dir_write(int32_t fd);
int32_t dir_close(int32_t fd);
typedef  enum file_type{
    USER_ACCESS_RTC_FILE,
    FILE_DIRECTORY,
    REGULAR_FILE
}file_type_t;




#endif
