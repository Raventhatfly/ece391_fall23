#ifndef PAGE_H
#define PAGE_H

#include "lib.h"
#include "types.h"

#define ENTRIES 1024 /* the entries for the page directory and page table */ 
#define BROKEN_PAGE_SIZE 4*ENTRIES /* the broken up page size is 4 KB */
#define INITIAL_STATE 0x0 /* the initial state for the page */
#define VIDEO   0xB8000  /* video memory address */
#define KERNEL  0x400000 /* the kernel memory is 4 MB */
#define EIGHT_MBYTE 0x800000
#define FOUR_MBYTE 0x400000
#define PRESENT_MASK 0x1 /* the mask for the last bits */
#define R_AND_W_MASK 0x2 /* the mask for the last second bits */
#define USER_MASK 0x4 /* the mask for the last third bits */
#define MB_BITMASK 0x83 /* the mask for the 4MB */
#define USER_START_PLACE  0x8000000  // 128 MB, the user space starts at 128 MB
#define ADDR_SEARCH    22 /* as the highest 10 bits are the page directory index, so we need to move 22 bits to the right */
uint32_t page_directory_entries[ENTRIES] __attribute__((aligned(BROKEN_PAGE_SIZE)));
uint32_t page_table_entries[ENTRIES] __attribute__((aligned(BROKEN_PAGE_SIZE)));
uint32_t video_table_entries[ENTRIES] __attribute__((aligned(BROKEN_PAGE_SIZE)));




extern void page_init();
extern void program_page_init(uint32_t program_id);
extern void set_map(uint8_t** screen_start);
#endif 
