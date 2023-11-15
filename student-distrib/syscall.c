#include "syscall.h"
#include "x86_desc.h"
#include "filesystem.h"
#include "page.h"
#include "rtc.h"
#include "terminal.h"


int8_t process_id_arr[MAX_PROCESS] = {0};
/*
    * halt
    *   DESCRIPTION: halt the current process and return to the previous process
    *   INPUTS: status
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t halt (uint8_t status){
    /* printf("Halt!\n"); */
    int32_t ebp,esp;
    int32_t ret;
    int32_t i;

    ret = 0;
    int32_t pid = fetch_curr_pid();
    pcb_t* pcb = fetch_pcb_addr(pid);
    if(pcb->parent_pid == -1){
        printf("Fail to halt base process\n");
        process_id_arr[pcb->pid] = 0;
        execute((uint8_t*)("shell"));
        return -1;
    }
    /* 0~1 are stdin and stdout, cannot be closed */
    for (i=2; i<8; i++) {
        if (pcb->file_desc_arr[i].flags==1) {
            close(i);
            pcb->file_desc_arr[i].flags=0;
        }
    }
    process_id_arr[pid]=0;

    pcb_t* prev_pcb=fetch_pcb_addr(pcb->parent_pid);
    program_page_init(pcb->parent_pid);
    
    tss.ss0 = KERNEL_DS;
    tss.esp0 = KERNEL_STACK_ADDR - prev_pcb->pid * PCB_SIZE - 4; /* Prevent stack collision. Using 4 as alignment */ 
    
    ebp = pcb->ebp;    
    esp = pcb->esp;
    ret = (int32_t) status;
    if (status==255) ret=256;         /* exception */
    asm volatile(
        "movl %0, %%esp\n"
        "movl %1, %%ebp\n"
        "movl %2, %%eax\n"
        "leave\n;"
        "ret\n"
        : 
        : "r" (esp), "r" (ebp),"r"(ret)
        : "esp", "ebp", "eax"
    );
    return 0;
}
/*
    * execute
    *   DESCRIPTION: execute the input command
    *   INPUTS: command
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t execute (const uint8_t* command){
    /* commands and args */
    uint8_t cmd[MAX_CMD + 1] = {'\0'};
    uint8_t args[MAX_ARG_LEN + 1] = {'\0'};

    uint8_t buf[4];         
    dentry_t dentry;
    uint32_t program_entry;
    int32_t old_esp, old_ebp;
    int pid;
    int cmd_len = 0;
    int i = 0;
    int j = 0;
    // int curr_arg = 0;
    int file_len;
    int retval;

    if(command == NULL){
        return -1;
    }
    /* state 0, waiting for command at the leading spaces;
    *  state 1, reading the command
       state 2, waiting for the arg
       state 3, reading args
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
            args[j] = command[i];
            i++;
            j++;
            break;
        default:
            state = 0;
            break;
        }
        
    }

    if(cmd_len == 0){
        return -1;      /* no executable input */
    }
    retval = read_dentry_by_name(cmd,&dentry);
    // memset(cmd,'\0',MAX_CMD + 1);
    if(retval == -1){
        return -1;
    }
    if(read_data(dentry.inode_num,0,buf,4) != 4){
        return -1;
    }
    
    /* Executble check: ELF magic number */
    if(buf[0] != 0x7f || buf[1] != 0x45 || buf[2] != 0x4c || buf[3] != 0x46){
        printf(" ELF Magic: %x, %x, %x, %x\n", buf[0],buf[1],buf[2],buf[3]);
        printf("Unrecoginzed Executble!\n");
        return -1;
    }

    /* Allocate PID */
    if((pid = allocate_pid()) == -1){
        printf("Process ID allocation failed!\nOnly supports tow user programs.\n");
        return -1;
    }

    /* Set up prgram paging (4MB) */
    program_page_init(pid);

    /* load user program entry, bit 24 - 27 */
    read_data(dentry.inode_num,24, (uint8_t*) &program_entry, 4);

    /* User-Level Program Loader */
    file_len = get_file_size(dentry.inode_num);
    read_data(dentry.inode_num, 0, (uint8_t*)USER_PROGRAM_ADDR, file_len);

    /* Create PCB */
    pcb_t* execute_pcb = fetch_pcb_addr(pid);
    // execute_pcb->arg_cnt = curr_arg;     /* arg number */
    execute_pcb->pid = pid;
    if(pid == 0){                            /* TODO: further modification: set up total process counter. If total process is 0 then there is not parent */
        execute_pcb->parent_pid = -1;        /* No parent */
    }else{
        execute_pcb->parent_pid = fetch_curr_pid();
    }
    strcpy((int8_t *)execute_pcb->cmd, (int8_t *)cmd);
    strcpy((int8_t *)execute_pcb->args, (int8_t *)args);

    
    /* TODO: reinspect here */
    execute_pcb->file_desc_arr[0].file_op_table_ptr = &stdin_op_table;
    execute_pcb->file_desc_arr[0].flags = 1;
    execute_pcb->file_desc_arr[0].file_pos = 0;
    execute_pcb->file_desc_arr[0].inode = 0;

    execute_pcb->file_desc_arr[1].file_op_table_ptr = &stdout_op_table;
    execute_pcb->file_desc_arr[1].flags = 1;
    execute_pcb->file_desc_arr[1].file_pos = 0;
    execute_pcb->file_desc_arr[1].inode = 0;

    /* save ebp and esp register */
    asm volatile(
        "movl %%esp, %0\n"
        "movl %%ebp, %1\n"
        : "=r" (old_esp), "=r" (old_ebp)
    );
    execute_pcb->esp  = old_esp;
    execute_pcb->ebp = old_ebp;

    /* Context Switch */
    sti();
    tss.ss0 = KERNEL_DS;
    tss.esp0 = KERNEL_STACK_ADDR - pid * PCB_SIZE - 4;         /* Prevent stack collision. Using 4 as alignment */ 
    asm volatile(
        "pushl %0\n"
        "pushl %1\n"
        "pushfl\n"
        "pushl %2\n"
        "pushl %3\n"
        "iret\n"
        : 
        : "r" (USER_DS), "r" (USER_PROGRAM_ADDR - 4),"r" (USER_CS), "r" (program_entry)   
        : "memory"
    );
    
    return 0;
}
/*
    * read
    *   DESCRIPTION: read the input command
    *   INPUTS: fd, buf, nbytes
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t read (int32_t fd, void* buf, int32_t nbytes){
    // printf("Read!\n");
    if (fd < 0 || fd > 7) return -1; /* invalid fd */
    if (buf == NULL) return -1;      /* invalid buf */
    if (nbytes < 0) return -1;       /* invalid nbytes */
    int32_t cur_pid = fetch_curr_pid();
    pcb_t * cur_pcb= fetch_pcb_addr(cur_pid);
    if (cur_pcb->file_desc_arr[fd].flags==0) return -1; //invalid fd
    int32_t numread=cur_pcb->file_desc_arr[fd].file_op_table_ptr->read(fd,buf,nbytes);
    if (cur_pcb->file_desc_arr[fd].inode!=0) cur_pcb->file_desc_arr[fd].file_pos+=numread; //update file position only for regular file
    return numread;
}
/*
    * write
    *   DESCRIPTION: write the input command
    *   INPUTS: fd, buf, nbytes
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t write (int32_t fd, const void* buf, int32_t nbytes){
    // printf("Write!\n");
    if (fd < 0 || fd > 7) return -1; /* invalid fd */
    if (buf == NULL) return -1;      /* invalid buf */
    if (nbytes < 0) return -1;       /* invalid nbytes */
    int32_t cur_pid = fetch_curr_pid();
    pcb_t * cur_pcb= fetch_pcb_addr(cur_pid);
    if (cur_pcb->file_desc_arr[fd].flags==0) return -1; //invalid fd
    return cur_pcb->file_desc_arr[fd].file_op_table_ptr->write(fd,buf,nbytes);
}
/*
    * open
    *   DESCRIPTION: open the input command
    *   INPUTS: filename
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t open (const uint8_t* filename){
    /* printf("Open!\n");  */
    dentry_t dentry;
    if (read_dentry_by_name(filename,&dentry)==-1) return -1;
    int32_t cur_pid = fetch_curr_pid();
    pcb_t * cur_pcb= fetch_pcb_addr(cur_pid);
    int32_t fd;
    for (fd=2;fd<8;fd++)
    {
        if (cur_pcb->file_desc_arr[fd].flags==0) break;
    } //find the first available fd
    if (fd==8) return -1; //no available fd
    if (dentry.file_type==0) //rtc
    {
        cur_pcb->file_desc_arr[fd].file_op_table_ptr=&rtc_op_table;
        cur_pcb->file_desc_arr[fd].flags=1; //set flag to 1
        cur_pcb->file_desc_arr[fd].file_pos=0;
        cur_pcb->file_desc_arr[fd].inode=0; //inode is 0 for rtc
        return fd;
    }
    else if (dentry.file_type==1) //directory
    {
        cur_pcb->file_desc_arr[fd].file_op_table_ptr=&dir_op_table;
        cur_pcb->file_desc_arr[fd].flags=1; //set flag to 1
        cur_pcb->file_desc_arr[fd].file_pos=0;
        cur_pcb->file_desc_arr[fd].inode=0; //inode is 0 for directory
        return fd;
    }
    else if (dentry.file_type==2) //regular file
    {
        cur_pcb->file_desc_arr[fd].file_op_table_ptr=&file_op_table;
        cur_pcb->file_desc_arr[fd].flags=1; //set flag to 1
        cur_pcb->file_desc_arr[fd].file_pos=0;
        cur_pcb->file_desc_arr[fd].inode=dentry.inode_num; //inode is 0 for directory
        return fd;
    }
    else return -1; //invalid file type
}
/*
    * close
    *   DESCRIPTION: close the input command
    *   INPUTS: fd
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t close (int32_t fd){
    /* printf("Close!\n"); */
    if (fd <= 1 || fd > 7) return -1;                   /* invalid fd */
    int32_t cur_pid = fetch_curr_pid();
    pcb_t * cur_pcb= fetch_pcb_addr(cur_pid);
    if (cur_pcb->file_desc_arr[fd].flags==0) return -1; //invalid fd
    cur_pcb->file_desc_arr[fd].flags=0;
    return cur_pcb->file_desc_arr[fd].file_op_table_ptr->close(fd);
}
/*
    * getargs
    *   DESCRIPTION: get the input arguments
    *   INPUTS: buf, nbytes
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t getargs (uint8_t* buf, int32_t nbytes){
    int i;
    int curr_pid = fetch_curr_pid();
    pcb_t* curr_pcb = fetch_pcb_addr(curr_pid);
    if(buf == NULL || curr_pcb->args[0] == '\0'){
        return -1;
    }
    for(i=0;i<nbytes;i++){
        if(curr_pcb->args[i] == '\0'){
           buf[i] = '\0';
           return 0; 
        }
        else{
            buf[i] = curr_pcb->args[i];
        }
    }
    /* failure when nbytes written but no end of string encountered */
    return -1;
}
/*
    * vidmap
    *   DESCRIPTION: vidmap the input command
    *   INPUTS: screen_start
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t vidmap (uint8_t** screen_start){
    if (screen_start==NULL || screen_start<(uint8_t**)KERNEL_STACK_ADDR || screen_start>=(uint8_t**)USER_STACK_ADDR) return -1; //invalid screen_start
    set_map(VIDEO>>12,1);
    *screen_start=(uint8_t*)(0x84b8000);
    return 0;
}
/*
    * set_handler
    *   DESCRIPTION: set_handler the input command
    *   INPUTS: signum, handler_address
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
int32_t set_handler (int32_t signum, void* handler_address){
    printf("Set Handler!\n");
    return 0;
}
/*
    * sigreturn
    *   DESCRIPTION: sigreturn the input command
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
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
        if(process_id_arr[i] == 0){
            process_id_arr[i] = 1;
            return i;
        }
    }
    return -1;
}
/*
    * fetch_pcb_addr
    *   DESCRIPTION: fetch the address of the PCB
    *   INPUTS: pid
    *   OUTPUTS: none
    *   RETURN VALUE: The address of the PCB
    *   SIDE EFFECTS: none
*/
pcb_t * fetch_pcb_addr(int32_t pid){
    pcb_t * return_addr;
    return_addr = (pcb_t *) (KERNEL_STACK_ADDR - (pid + 1) * PCB_SIZE);
    return return_addr;
}
/*
    * fetch_curr_pid
    *   DESCRIPTION: fetch the current process ID
    *  INPUTS: none
    *  OUTPUTS: none
    * RETURN VALUE: The current process ID
    * SIDE EFFECTS: none
*/
int32_t fetch_curr_pid(){
    register int32_t espv asm ("esp");
    return (KERNEL_STACK_ADDR-espv)/PCB_SIZE;
}


/*
    * file_op_table_init
    *   DESCRIPTION: initialize the file operation table
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: none
*/

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
}
/*
    * null_read
    *   DESCRIPTION: null read
    *   INPUTS: fd, buf, nbytes
    *   OUTPUTS: none
    *   RETURN VALUE: -1
    *   SIDE EFFECTS: none
*/
int null_read(int32_t fd, void* buf, int32_t nbytes)
{
    return -1;
}
/*
    * null_write
    *   DESCRIPTION: null write
    *   INPUTS: fd, buf, nbytes
    *   OUTPUTS: none
    *   RETURN VALUE: -1
    *   SIDE EFFECTS: none
*/
int null_write(int32_t fd, const void* buf, int32_t nbytes)
{
    return -1;
}
