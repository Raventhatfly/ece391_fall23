#ifndef PAGE_H
#define PAGE_H

#include "lib.h"
#include "types.h"

#define ENTRIES 1024 /* the entries for the page directory and page table */ 
#define BROKEN_PAGE_SIZE 4*ENTRIES /* the broken up page size is 4 KB */
#define INITIAL_STATE 0x0 /* the initial state for the page */
#define VIDEO   0xB8000 
#define KERNEL  0x400000 /* the kernel memory is 4 MB */
#define TWO_BITMASK 0x3 /* the mask for the last 2 bits */
#define THREE_BITMASK 0x7 /* the mask for the last 3 bits */
#define MB_BITMASK 0x87 /* the mask for the 4MB */
uint32_t page_directory_entries[ENTRIES] __attribute__((aligned(BROKEN_PAGE_SIZE)));
uint32_t page_table_entries[ENTRIES] __attribute__((aligned(BROKEN_PAGE_SIZE)));








extern void page_init();

#endif 
