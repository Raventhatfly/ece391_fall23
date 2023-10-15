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
        page_directory_entries[i] = INITIAL_STATE;
    }
    for (i=0; i<ENTRIES; i++) {
    page_table_entries[i] = INITIAL_STATE;
    }
    int video_idx = VIDEO >> 12;    /* get the index of video memory, move 12 bit to the right for reduce 0 */
    page_table_entries[video_idx] = VIDEO | TWO_BITMASK; /* set the video memory,7 is used as the mask of last 2 bit*/
    page_directory_entries[0] = ((uint32_t)page_table_entries)| TWO_BITMASK; 
    page_directory_entries[1] = KERNEL | MB_BITMASK; /* set the kernel memory */
    /* use asm to set the control register bits to their proper values */
    /* so tells the hardware where the page directory is located and enables hardware support for paging */
    asm volatile("              \n\
    movl %0,%%eax               \n\
    movl %%eax,%%cr3            \n\
    movl %%cr4,%%eax            \n\
    orl $0x00000010,%%eax       \n\
    movl %%eax,%%cr4            \n\
    movl %%cr0,%%eax            \n\
    orl $0x80000001,%%eax       \n\
    movl %%eax,%%cr0            \n\
    "
    :
    : "d"(page_directory_entries)
    : "eax");
}

