#include "filesystem.h"

static int file_base;
void filesystem_init(int* base_ptr){
    file_base = base_ptr;
    printf("File System Base Pointer %x\n", file_base);
    return;
}
void read_dentry_by_name(char* file_name){
    return;
}
void read_dentry_by_index(int index){
    return;
}
void read_data(){
    return;
}