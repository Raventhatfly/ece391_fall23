#include "syscall.h"
#include "filesystem.h"

int32_t halt (uint8_t status){
    printf("Halt!\n");
    return 0;
}

int32_t execute (const uint8_t* command){
    printf("Execte!\n");
    uint8_t cmd[MAX_CMD];
    uint8_t buf[4];         
    dentry_t dentry;
    int cmd_len = 0;
    int i = 0;
    int leading = 1;
    

    
    while(command[i] != '\0'){
        /* Get rid of leading spaces */
        if(command[i] == ' '){
            i++;
            continue;
        }
        leading = 0;    /* leading sapce end */
        if(command[i] != ' '){
            cmd[cmd_len] = command[i];
            cmd_len++;
        }else{
            /* TODO: do not support args passed into the executale yet */
            break;
        }
    }
    cmd[cmd_len] = '\0';
    if(cmd_len == 0){
        return -1;      /* no executable input */
    }
    read_dentry_by_name(cmd,&dentry);
    read_data(dentry.inode_num,0,buf,4);
    // if(buf[0] == ){

    // }
    return 0;
}

int32_t read (int32_t fd, void* buf, int32_t nbytes){
    printf("Read!\n");
    return 0;
}

int32_t write (int32_t fd, const void* buf, int32_t nbytes){
    printf("Write!\n");
    return 0;
}

int32_t open (const uint8_t* filename){
    printf("Open!\n");
    return 0;
}

int32_t close (int32_t fd){
    printf("Close!\n");
    return  0;
}

int32_t getargs (uint8_t* buf, int32_t nbytes){
    printf("getargs!\n");
    return 0;
}

int32_t vidmap (uint8_t** screen_start){
    printf("VidMap!\n");
    return 0;
}

int32_t set_handler (int32_t signum, void* handler_address){
    printf("Set Handler!\n");
    return 0;
}

int32_t sigreturn (void){
    printf("Sig Return!\n");
    return 0;
}