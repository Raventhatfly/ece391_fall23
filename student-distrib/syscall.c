#include "syscall.h"
#include "filesystem.h"
#include "page.h"
#include "rtc.h"
#include "terminal.h"
int32_t halt (uint8_t status){
    printf("Halt!\n");
    return 0;
}

int32_t execute (const uint8_t* command){
    printf("Execte!\n");
    uint8_t buf[4];         
    dentry_t dentry;
    uint32_t program_entry;
    int pid;
    int cmd_len = 0;
    int i = 0;
    int j = 0;
    int curr_arg = 0;
    /* 
    *  state 0, waiting for command at the leading spaces
    *  state 1, reading the command
    *  state 2, waiting for the next arg
    *  state 3, reading the current arg
    */
    int state = 0;  
    while(command[i] != '\0'){
        switch (state)
        {
        case 0:
            if(command[i] == ' '){
                i++;
            }else{
                state = 1;
            }
            break;
        case 1:
            if(command[i] == ' '){
                state = 2;
                i++;
            }else{
                if(cmd_len >= MAX_CMD)    return -1;
                cmd[cmd_len] = command[i];
                cmd_len++; i++;
            }
            break;
        case 2:
            if(command[i] != ' '){
                state = 3;
            }else{
                i++;
            }
            break;
        case 3: 
            if(command[i] == ' '){
                state = 2;
                curr_arg++;
                if(curr_arg >= MAX_ARGS)    return -1;
                j = 0;
                i++;
            }else if(j >= MAX_ARG_LEN){
                return -1;
            }else{
                args[curr_arg][j] = command[i];
                j++; i++;
            }
            break;
        default:
            state = 0;
            break;
        }
        
    }
    /* 
     * Boundary condition. If arg not ended with space then increment curr_arg 
     * so that it becomes the number of total args.
     */
    if(state == 3 && j != 0){
        curr_arg++;         
    }

    if(cmd_len == 0){
        return -1;      /* no executable input */
    }
    if(read_dentry_by_name(cmd,&dentry) == -1){
        return -1;
    }
    if(read_data(dentry.inode_num,0,buf,4) != 4){
        return -1;
    }
    
    /* Executble check: ELF magic number */
    if(buf[0] != 0x7f || buf[1] != 0x4f || buf[2] != 0x4c || buf[3] != 0x46){
        return -1;
    }

    /* Allocate PID */
    if((pid = allocate_pid()) == -1){
        return -1;
    }

    /* Set up paging */
    program_page_init(pid);

    /* User-Level Program Loader, 24-27 */
    read_data(dentry.inode_num,24, (uint8_t*) &program_entry, 4);

    /* TODO: Copy file data into memory */

    /* Set up PCB */
    pcb_t* execute_pcb = fetch_pcb_addr(pid);
    execute_pcb->arg_cnt = curr_arg;     /* arg number */
    execute_pcb->pid = pid;
    if(pid == 0){
        execute_pcb->parent_pid = -1;        /* No parent */
    }else{
        execute_pcb->parent_pid = fetch_curr_pid();
    }
    strcpy(execute_pcb->cmd, cmd);
    for(i = 0; i < curr_arg; i++ ){
        strcpy(execute_pcb->args[i], args[i]);
    }
    /* TODO: reinspect here */
    execute_pcb->file_desc_arr[0].file_op_table_ptr = &stdin_op_table;
    execute_pcb->file_desc_arr[0].flags = 1;
    execute_pcb->file_desc_arr[0].file_pos = 0;
    execute_pcb->file_desc_arr[0].inode = 0;

    execute_pcb->file_desc_arr[1].file_op_table_ptr = &stdout_op_table;
    execute_pcb->file_desc_arr[0].flags = 1;
    execute_pcb->file_desc_arr[0].file_pos = 0;
    execute_pcb->file_desc_arr[0].inode = 0;
    

    return 0;
}

int32_t read (int32_t fd, void* buf, int32_t nbytes){
    printf("Read!\n");
    if (fd < 0 || fd > 7) return -1; /* invalid fd */
    if (buf == NULL) return -1;      /* invalid buf */
    if (nbytes < 0) return -1;       /* invalid nbytes */
    int32_t cur_pid = fetch_curr_pid();
    pcb_t * cur_pcb= fetch_pcb_addr(cur_pid);
    if (cur_pcb->file_desc_arr[fd].flags==0) return -1; //invalid fd
    
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


/*
    * allocate_pid
    *   DESCRIPTION: allocate a process ID
    *   INPUTS: none
    *   OUTPUTS: set process_id_arr
    *   RETURN VALUE: The available process ID. Return -1 if not availalble.
    *   SIDE EFFECTS: none
*/
int32_t allocate_pid(){
    int i;
    for(i = 0;i < MAX_PROCESS; i++){
        if(process_id_arr[i] != 0){
            process_id_arr[i] = 1;
            return i;
        }
    }
    return -1;
}

pcb_t * fetch_pcb_addr(int32_t pid){
    pcb_t * return_addr;
    return_addr = (pcb_t *) (KERNEL_STACK_ADDR - (pid + 1) * PCB_SIZE);
    return return_addr;
}

int32_t fetch_curr_pid(){
    register int32_t espv asm ("esp");
    return (KERNEL_STACK_ADDR-espv)/PCB_SIZE;
}


    

void file_op_table_init()
{
    rtc_op_table.open = rtc_open;
    rtc_op_table.read = rtc_read;
    rtc_op_table.write = rtc_write;
    rtc_op_table.close = rtc_close;

    dir_op_table.open = dir_open;
    dir_op_table.read = dir_read;
    dir_op_table.write = dir_write;
    dir_op_table.close = dir_close;

    file_op_table.open = file_open;
    file_op_table.read = file_read;
    file_op_table.write = file_write;
    file_op_table.close = file_close;

    stdin_op_table.open = terminal_open;
    stdin_op_table.read = terminal_read;
    stdin_op_table.write =null_write;
    stdin_op_table.close =terminal_close ;

    stdout_op_table.open =terminal_open ;
    stdout_op_table.read =null_read ;
    stdout_op_table.write =terminal_write ;
    stdout_op_table.close = terminal_close;

    null_op_table.open = null_open;
    null_op_table.read = null_read;
    null_op_table.write = null_write;
    null_op_table.close = null_close;
}
int null_read(int32_t fd, void* buf, int32_t nbytes)
{
    return -1;
}
int null_write(int32_t fd, const void* buf, int32_t nbytes)
{
    return -1;
}
int null_open(const uint8_t *fname)
{
    return -1;
}
int null_close(int32_t fd)
{
    return -1;
}