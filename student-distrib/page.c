#include "page.h"
#include "lib.h"
#include "types.h"

/*
    *  void buddy_init()
    *    DESCRIPTION: initialize the buddy system
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: initialize the buddy system
*/
void buddy_init()
{
    uint32_t i,j,num=1;
    for (i=0;i<=10;i++)
    {
        buddy_head[i] = NULL;
        size[i] = -1;
        for (j=num;j<2*num;j++)
        {
            buddy_list[j].level = 10-i;
            buddy_list[j].start_addr = ((1024/num)*(j-num));
            buddy_list[j].prev = NULL;
            buddy_list[j].next = NULL;
            buddy_list[j].index = j;
        }
        num *= 2;
    }
    buddy_head[10]=&buddy_list[1];
}
/*
    *  void add(int index,int level)
    *    DESCRIPTION: add the node to the buddy system
    *    INPUT: index -- the index of the node
    *           level -- the level of the node
    *    OUTPUT: none
    *    SIDE EFFECT: add the node to the buddy system
*/
void add(int index,int level)
{
   //printf("add level: %d %d\n",index,level);
    buddy_node* temp;
    if (level<0 || level>10) return;
    if (buddy_head[level]==NULL)
    {
        buddy_head[level]=&buddy_list[index];
        buddy_list[index].prev=NULL;
        buddy_list[index].next=NULL;
        return;
    }
    buddy_head[level]->prev=&buddy_list[index];
    buddy_list[index].next=buddy_head[level];
    buddy_list[index].prev=NULL;
    buddy_head[level]=&buddy_list[index];
}
/*
    *  void split(int level)
    *    DESCRIPTION: split the node to the buddy system
    *    INPUT: level -- the level of the node
    *    OUTPUT: none
    *    SIDE EFFECT: split the node to the buddy system
*/
void split(int level)
{
    buddy_node* temp;
    if (level<0 || level>10) return;
    if (buddy_head[level]==NULL) return;
    temp = buddy_head[level];
    buddy_head[level] = buddy_head[level]->next;
    if (buddy_head[level]!=NULL) buddy_head[level]->prev = NULL;
    temp->prev = NULL;
    temp->next = NULL;
    if (level==0) return;
    add(temp->index*2,level-1);
    add(temp->index*2+1,level-1);
}
/*
    *  void page_init()
    *    DESCRIPTION: initialize the page
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: set the page directory and page table
*/
void page_init() {
    int i=0;
    for (i=0; i<ENTRIES; i++) {     /* initialize all entries of table and dict to 0 */
        page_directory_entries[i] = 0;
        page_table_entries[i] = 0;
        video_table_entries[i] = 0;
    }
    int video_idx = VIDEO >> 12;    /* get the index of video memory, move 12 bit to the right for reduce 0 */
    for (i=0;i<=3;i++)
    {
        page_table_entries[video_idx] = ((video_idx<<12) & 0xfffff000) | PRESENT_MASK; /* set the video memory*/
        page_table_entries[video_idx] |= R_AND_W_MASK;
        video_idx++;
    }

    page_directory_entries[0] = ((uint32_t)page_table_entries & 0xfffff000)| PRESENT_MASK; /* set the page directory entries */
    page_directory_entries[0] |= R_AND_W_MASK; 

    page_directory_entries[1] = KERNEL | MB_BITMASK; /* set the kernel memory */

    uint32_t pg_dir_index = (USER_START_PLACE+FOUR_MBYTE) >> ADDR_SEARCH; /* get the index of page directory */
    page_directory_entries[pg_dir_index] = 0; /* initialize the page directory entries */
    page_directory_entries[pg_dir_index] |= PRESENT_MASK;
    page_directory_entries[pg_dir_index] |= R_AND_W_MASK;
    page_directory_entries[pg_dir_index] |= USER_MASK;
    page_directory_entries[pg_dir_index] |= ((uint32_t)video_table_entries & 0xfffff000);
    /* use asm to set the control register bits to their proper values */
    /* so tells the hardware where the page directory is located and enables hardware support for paging */

    pg_dir_index = (MALLOC_START_PLACE) >> ADDR_SEARCH; /* get the index of page directory */
    page_directory_entries[pg_dir_index] = 0; /* initialize the page directory entries */
    page_directory_entries[pg_dir_index] |= PRESENT_MASK;
    page_directory_entries[pg_dir_index] |= R_AND_W_MASK;
    page_directory_entries[pg_dir_index] |= USER_MASK;
    page_directory_entries[pg_dir_index] |= ((uint32_t)malloc_table_entries & 0xfffff000);
    /* use asm to set the control register bits to their proper values */
    /* so tells the hardware where the page directory is located and enables hardware support for paging */
    for (i=0;i<ENTRIES;i++)
    {
        malloc_table_entries[i] = ((i<<12) & 0xfffff000) | PRESENT_MASK; /* set the video memory*/
        malloc_table_entries[i] |= R_AND_W_MASK;
        malloc_table_entries[i] |= USER_MASK;
    }
    buddy_init();

    asm volatile("              \n\
    movl %0,%%eax               \n\
    movl %%eax,%%cr3            \n\
    movl %%cr4,%%eax            \n\
    orl $0x00000010,%%eax       \n\
    movl %%eax,%%cr4            \n\
    movl %%cr0,%%eax            \n\
    orl $0x80000000,%%eax       \n\
    movl %%eax,%%cr0            \n\
    "
    :
    : "d"(page_directory_entries)
    : "eax");
}

/*
    *  void program_page_init(uint32_t program_addr)
    *    DESCRIPTION: initialize the page for program
    *    INPUT: program_addr -- the address of the program
    *    OUTPUT: none
    *    SIDE EFFECT: set the page directory and page table for program
*/
void program_page_init(uint32_t program_id){
    uint32_t pg_dir_index = USER_START_PLACE >> ADDR_SEARCH; /* get the index of page directory */
    page_directory_entries[pg_dir_index] = 0; /* initialize the page directory entries */
    page_directory_entries[pg_dir_index] |= PRESENT_MASK;
    page_directory_entries[pg_dir_index] |= R_AND_W_MASK;
    page_directory_entries[pg_dir_index] |= USER_MASK;
    page_directory_entries[pg_dir_index] |= MB_BITMASK; /* set the page directory entries */
    page_directory_entries[pg_dir_index] |= (EIGHT_MBYTE + program_id * FOUR_MBYTE); /* set physical base address*/
    uint32_t page_dir_addr = (uint32_t) &page_directory_entries; /* update CR3 register to use the new page directory and flush the TLB*/
    asm volatile("     \n\
    movl %0, %%eax     \n\
    movl %%eax, %%cr3  \n\
    "    
    :
    : "r" (page_dir_addr)
    : "%eax");
}
/*
    *  void set_map(uint32_t act_mem, uint32_t present)
    *    DESCRIPTION: set the map for video memory
    *    INPUT: act_mem -- the actual memory
    *           present -- the present
    *    OUTPUT: none
    *    SIDE EFFECT: set the map for video memory
*/
void set_map(uint32_t act_mem, uint32_t present)
{
    uint32_t pg_dir_index=VIDEO>>12; //get the index of video memory, move 12 bit to the right for 4kb page
    video_table_entries[pg_dir_index] = 0; /* initialize the page directory entries */
    video_table_entries[pg_dir_index] |= present;
    video_table_entries[pg_dir_index] |= R_AND_W_MASK;
    video_table_entries[pg_dir_index] |= USER_MASK;
    video_table_entries[pg_dir_index] |= ((act_mem<<12) & 0xfffff000);
    uint32_t page_dir_addr = (uint32_t) &page_directory_entries; /* update CR3 register to use the new page directory and flush the TLB*/
    asm volatile("     \n\
    movl %0, %%eax     \n\
    movl %%eax, %%cr3  \n\
    "    
    :
    : "r" (page_dir_addr)
    : "%eax");
}
/*
    *  void* malloc(uint32_t num)
    *    DESCRIPTION: malloc the memory
    *    INPUT: num -- the number of memory
    *    OUTPUT: none
    *    SIDE EFFECT: malloc the memory
*/
void* malloc(uint32_t length)
{
    int i,num,level;
    length=(length-1)/4096+1; //get the number of pages needed
    i=0;
    num = 1;
    while (num < length)
    {
        num *= 2;
        i++;
    }
    level=i;
    while (buddy_head[i]==NULL && i<=10) i++;
    if (i>10) return NULL;
    while (i>level)
    {
        split(i);
        i--;
    }
    buddy_node* temp = buddy_head[i];
    buddy_head[i] = buddy_head[i]->next;
    if (buddy_head[i]!=NULL) buddy_head[i]->prev = NULL;
    temp->prev = NULL;
    temp->next = NULL;
    size[temp->start_addr]=level;
    return (void*)(temp->start_addr*4096+MALLOC_START_PLACE);
}
/*
    *  void free(void* ptr)
    *    DESCRIPTION: free the memory
    *    INPUT: ptr -- the pointer of memory
    *    OUTPUT: none
    *    SIDE EFFECT: free the memory
*/
int free(void* ptr)
{
    int num=(int)(ptr-MALLOC_START_PLACE)/4096;
    int level=size[num];
    int index,i;
    if (level==-1) return -1;
    size[num]=-1;
    index=1;
    for (i=0;i<10-level;i++) index*=2;
    index+=num/(1024/index);
    while ((buddy_list[index^1].next!=NULL || buddy_list[index^1].prev!=NULL || buddy_head[level]==&buddy_list[index^1]) && index>1)
    {
        if (buddy_head[level]==&buddy_list[index^1])
        {
            buddy_head[level]=buddy_head[level]->next;
            if (buddy_head[level]!=NULL) buddy_head[level]->prev = NULL;
        }
        else
        {
            buddy_list[index^1].prev->next = buddy_list[index^1].next;
            if (buddy_list[index^1].next!=NULL) buddy_list[index^1].next->prev = buddy_list[index^1].prev;
        }
        buddy_list[index^1].prev = NULL;
        buddy_list[index^1].next = NULL;
        index/=2;
        level++;
    }
    add(index,level);
    return 0;
}


/*
    *  void display_memory()
    *    DESCRIPTION: display the memory
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: display the memory
*/
void display_memory()
{
    int i,num;
    printf("-------------------------\n");
    printf("Memory display:\n");
    for (i=0;i<=10;i++)
    {
        printf("Level %d:\n",i);
        num = 0;
        buddy_node* temp = buddy_head[i];
        while (temp!=NULL)
        {
            num++;
            temp = temp->next;
        }
        printf("The number of blocks: %d\n",num);
    }
}
