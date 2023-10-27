#include "filesystem.h"

int* file_base;
int* inode_addr;
int* data_addr;
boot_block_t* boot_block_ptr;
int dir_cnt;
int inode_cnt;
int data_cnt;

/*
    void filesystem_init(int* base_ptr);
    Inputs: int* base_ptr
    Return Value: none
    Function: initialize the file system
    Side effect: none
*/

void filesystem_init(int* base_ptr){
    file_base = base_ptr;
    boot_block_ptr = (boot_block_t*) base_ptr;
    dir_cnt = boot_block_ptr->dir_cnt;
    inode_cnt = boot_block_ptr->inode_cnt;
    data_cnt = boot_block_ptr->data_cnt;
    inode_addr = file_base + BLOCK_SIZE;
    data_addr = inode_addr + boot_block_ptr->dir_cnt * BLOCK_SIZE;
}
/*
    int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
    Inputs: const uint8_t* fname, dentry_t* dentry
    Return Value: 0 on success, -1 on failure
    Function: read the dentry by name
    Side effect: none
*/
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){
    int i;
    int fname_len, file_name_len;
    fname_len = strlen((int8_t*)fname);
    uint8_t file_name_ending[FILENAME_LEN+1];
    if(fname != NULL && fname_len <= 32){    
        for(i = 0; i < boot_block_ptr->dir_cnt; i++){    
            strncpy((int8_t*)file_name_ending, (int8_t*)(boot_block_ptr->direntries[i].file_name), FILENAME_LEN);
            file_name_ending[FILENAME_LEN] = '\0';        
            file_name_len = strlen((int8_t*)(file_name_ending));
            if(file_name_len != fname_len){
                continue;
            }
            if(strncmp((int8_t*)boot_block_ptr->direntries[i].file_name, (int8_t*)fname, fname_len)==0){
                return read_dentry_by_index(i, dentry);
            }
        }
    }
    return -1;      /* return -1 on failure */
}

/*
    int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
    Inputs: uint32_t index, dentry_t* dentry
    Return Value: 0 on success, -1 on failure
    Function: read the dentry by index
    Side effect: none
*/
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    if(index >= boot_block_ptr->dir_cnt){
        return -1;
    }
    *dentry = boot_block_ptr->direntries[index];
    return 0;      /* return -1 on failure */
}

/*
    int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
    Inputs: uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length
    Return Value: 0 on success, -1 on failure
    Function: read the data
    Side effect: none
*/
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    uint32_t curr_data_block;
    uint32_t block_offset;
    uint32_t length_remain;
    uint32_t buf_offset;
    inode_t* index_node; 

    index_node  = (((inode_t*)((boot_block_t*)file_base+1))+inode);
    /* calculate the current data block, block offset, length remain and buffer offset */
    curr_data_block = offset / BLOCK_SIZE;
    block_offset = offset - curr_data_block * BLOCK_SIZE;
   
    buf_offset = 0;

    if(inode >= 63){
        return -1;
    }
    /* test if the offset is larger than the length of the file */
    if (index_node->length < offset){
        return 0;
    }
    if (index_node->length<offset+length)
        length=index_node->length-offset;
    length_remain = length;
    data_block_t* data_block_ptr = (data_block_t*)(((inode_t*)((boot_block_t*)file_base+1))+inode_cnt);
    while(length_remain >= BLOCK_SIZE){
        //memcpy(buf+buf_offset, (void*)(&(data_block_ptr[index_node->data_block_num[curr_data_block]])) + block_offset
          //  , BLOCK_SIZE - block_offset);       /* copy data into the buffer */
         memcpy(buf+buf_offset,((void*)(data_block_ptr+index_node->data_block_num[curr_data_block]))+block_offset
            , BLOCK_SIZE - block_offset);  
        length_remain -= BLOCK_SIZE-block_offset;            /* update the length remain */
        buf_offset += (BLOCK_SIZE-block_offset);
        curr_data_block++;
        block_offset = 0;
        /* test if the data block is larger than 1023 */
        if(curr_data_block>=1023){
            return 0;
        }
    }
    /* copy the remaining data */
    if (curr_data_block<1023)
        memcpy(buf+buf_offset,((void*)(data_block_ptr+index_node->data_block_num[curr_data_block]))+block_offset
            , length_remain);    
    return length;
}
int32_t directory_read(uint32_t index,uint8_t* buf, dentry_t* dentry)
{
    uint8_t zeros[32] = {'\0'};
    memcpy(buf, zeros, 32);
    int length;
    if(index>=boot_block_ptr->dir_cnt) return -1;
    read_dentry_by_index(index, dentry); //get the dentry by index
    uint8_t file_name_ending[FILENAME_LEN+1];
    strncpy((int8_t*)file_name_ending, (int8_t*)(dentry->file_name), FILENAME_LEN);
    file_name_ending[FILENAME_LEN] = '\0';
    length = strlen((int8_t*)(file_name_ending));
    if (length>32) length=32;
    memcpy(buf, dentry->file_name, length); //copy the file name to the buffer
    return length;
}
/*
    int32_t file_open(const uint8_t *fname);
    Inputs: const uint8_t *fname
    Return Value: 0 on success, -1 on failure
    Function: open the file
    Side effect: none
*/
int32_t file_open(const uint8_t *fname)
{
    dentry_t dentry;
    return read_dentry_by_name(fname,&dentry)==-1; //return -1 on failure
}
/*
    int32_t file_read(int32_t fd);
    Inputs: int32_t fd
    Return Value: 0 on success, -1 on failure
    Function: read the file
    Side effect: none
*/
int32_t file_read(int32_t fd, void* buf, int32_t nbytes)
{
    return 0;
}
/*
    int32_t file_write(int32_t fd);
    Inputs: int32_t fd
    Return Value: 0 on success, -1 on failure
    Function: write the file
    Side effect: none
*/
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes)
{
    return 0;
}
/*
    int32_t file_close(int32_t fd);
    Inputs: int32_t fd
    Return Value: 0 on success, -1 on failure
    Function: close the file
    Side effect: none
*/
int32_t file_close(int32_t fd)
{
    return 0;
}
/*
    int32_t dir_open(const uint8_t *fname);
    Inputs: const uint8_t *fname
    Return Value: 0 on success, -1 on failure
    Function: open the directory
    Side effect: none
*/
int32_t dir_open(const uint8_t *fname)
{
    return 0;
}
/*
    int32_t dir_read(int32_t fd,uint8_t* buf);
    Inputs: int32_t fd,uint8_t* buf
    Return Value: 0 on success, -1 on failure
    Function: read the directory
    Side effect: none
*/
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes)
{
    return 0;
}
/*
    int32_t dir_write(int32_t fd);
    Inputs: int32_t fd
    Return Value: 0 on success, -1 on failure
    Function: write the directory
    Side effect: none
*/
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes)
{
    return 0;
}
/*
    int32_t dir_close(int32_t fd);
    Inputs: int32_t fd
    Return Value: 0 on success, -1 on failure
    Function: close the directory
    Side effect: none
*/
int32_t dir_close(int32_t fd)
{
    return 0;
}
