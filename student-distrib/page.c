#include "page.h"
#include "lib.h"
#include "types.h"

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
        // page_table_entries[video_idx] |= USER_MASK;
        video_idx++;
    }

    page_directory_entries[0] = ((uint32_t)page_table_entries & 0xfffff000)| PRESENT_MASK; /* set the page directory entries */
    page_directory_entries[0] |= R_AND_W_MASK; 
    //page_directory_entries[0] |= USER_MASK; 

    page_directory_entries[1] = KERNEL | MB_BITMASK; /* set the kernel memory */

    uint32_t pg_dir_index = (USER_START_PLACE+FOUR_MBYTE) >> ADDR_SEARCH; /* get the index of page directory */
    page_directory_entries[pg_dir_index] = 0; /* initialize the page directory entries */
    page_directory_entries[pg_dir_index] |= PRESENT_MASK;
    page_directory_entries[pg_dir_index] |= R_AND_W_MASK;
    page_directory_entries[pg_dir_index] |= USER_MASK;
    page_directory_entries[pg_dir_index] |= ((uint32_t)video_table_entries & 0xfffff000);
    /* use asm to set the control register bits to their proper values */
    /* so tells the hardware where the page directory is located and enables hardware support for paging */
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
