#include "filesystem.h"

int* file_base;
int* inode_addr;
int* data_addr;
boot_block_t* boot_block_ptr;
int dir_cnt;
int inode_cnt;
int data_cnt;


void filesystem_init(int* base_ptr){
    file_base = base_ptr;
    boot_block_ptr = (boot_block_t*) base_ptr;
    dir_cnt = boot_block_ptr->dir_cnt;
    inode_cnt = boot_block_ptr->inode_cnt;
    data_cnt = boot_block_ptr->data_cnt;
}

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){
    int i;
    int fname_len, file_name_len;
    fname_len = strlen(fname);
    if(fname != NULL && fname_len <= 32){    
        for(i = 0; i < boot_block_ptr->dir_cnt; i++){            
            file_name_len = strlen(boot_block_ptr->direntries[i].file_name);
            if(file_name_len != fname_len){
                continue;
            }
            if(strncpy(boot_block_ptr->direntries[i].file_name, fname, fname_len)==0){
                return read_dentry_by_index(i, dentry);
            }
        }
    }
    return -1;      /* return -1 on failure */
}

int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    if(index >= boot_block_ptr->dir_cnt){
        return -1;
    }
    *dentry = boot_block_ptr->direntries[index];
    return 0;      /* return -1 on failure */
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    uint32_t curr_data_block;
    uint32_t block_offset;
    uint32_t length_remain;
    uint32_t buf_offset;
    inode_t* index_node; 

    index_node  = (inode_t*) file_base[(inode + 1) * BLOCK_SIZE];
    curr_data_block = offset / BLOCK_SIZE;
    block_offset = offset - curr_data_block * BLOCK_SIZE;
    length_remain = length;
    buf_offset = 0;

    if(inode >= inode_cnt){
        return -1;
    }
    /*
    test if the offset is larger than the length of the file
    */
    if (index_node->length < offset){
        return 0;
    }
    while(length_remain >= BLOCK_SIZE){
        memcpy(buf+buf_offset, file_base[(index_node->data_block_num[curr_data_block] + 1 + inode_cnt) * BLOCK_SIZE + block_offset]
            , BLOCK_SIZE - block_offset);
        length_remain -= BLOCK_SIZE;
        buf_offset += (BLOCK_SIZE-block_offset);
        curr_data_block++;
        block_offset = 0;
        if(curr_data_block>=index_node->data_block_num){
            return 0;
        }
    }
    memcpy(buf+buf_offset,file_base[(index_node->data_block_num[curr_data_block] + 1 + inode_cnt) * BLOCK_SIZE + block_offset],length_remain);
    return length;
}
