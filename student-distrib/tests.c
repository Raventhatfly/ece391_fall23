#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "filesystem.h"
#include "terminal.h"
#include "rtc.h"

#define PASS 1
#define FAIL 0

extern void rtc_init();
extern int* filesys_base;

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}
unsigned char buf[128];	
static int32_t fd=0;
static int32_t nbytes=1;
/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

/*
 * div_test
 *   DESCRIPTION: test the divide error exception
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int div_test(){
	TEST_HEADER;
	int a = 1;
	int b = 0;
	a = a / b;
	return FAIL;
}

/*
 * page_value_test
 *   DESCRIPTION: test the page value
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int page_value_test(){
	TEST_HEADER;
	unsigned char* addr1=(unsigned char*)0xB80000;
	unsigned char* addr2=(unsigned char*)0x4000000;
	unsigned char return_value;
	return_value = *addr1;		/*read the value of the video memory, if there is no page fault, then should be PASS*/
	return_value = *addr2;		/*read the value of the video memory, if there is no page fault, then should be PASS*/
	return PASS;

}

/*
 * syscall_test
 *   DESCRIPTION: test the system call
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int syscall_test(){
	TEST_HEADER;
	asm volatile("int $128");	/* trigger system call */
	return PASS;
}
/*
 * page_dereference_test
 *   DESCRIPTION: test the page dereference
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
*/
int page_dereference_test(){
	TEST_HEADER;
	int* ptr = NULL;
	*ptr = 1;
	return FAIL;
}


// add more tests here

/* Checkpoint 2 tests */

/*
    *  int rtc_driver_test()
    *    DESCRIPTION: Tests the functionality of the RTC driver by iterating through 
    *                 a range of frequencies and checking the behavior of the rtc_write 
    *                 and rtc_read functions.
    *    INPUT: None.
    *    OUTPUT: Returns PASS if the function operates without crashing, otherwise returns FAIL 
    *    SIDE EFFECTS: 
    *                 - Prints a series of 1's to the console, with the speed increment each line
    *                 - The print output provides a rough visual representation of the 
    *                   RTC's operation at different frequencies.
    */
int rtc_driver_test() {
	TEST_HEADER;
	int freq, i;
 	for (freq = 2; freq <= 1024; freq <<= 1) {
	rtc_write(0, &freq, 4);
	/* the number of 1's printed
     * is five times the frequency value */
	/* speed increment each line */
	for (i=0; i<79; i++) {
		rtc_read(0, 0, 0);
		printf("%d", 1);
	}
	printf("\n");
	}
	return PASS;
}

/*
 * terminal_keyboard_test
 *   DESCRIPTION: test the keyboard input and the overflow by 130 inputs
 *  INPUTS: none
 * OUTPUTS: PASS/FAIL
 * RETURN VALUE: none
 * SIDE EFFECTS: none
*/
int terminal_keyboard_test(){
	TEST_HEADER;
	uint32_t buffer_test[40]={
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};
	int i,j;
	terminal_init();
	for(i=0;i<130;i++){   /* 130 > 128, so we can test overflow*/
		j = i % 40;
		buf[0]=buffer_test[j];
		terminal_display(fd, buf, nbytes);
	}
	return PASS;
}

/*
 * terminal_clear_test
 *   DESCRIPTION: test the terminal clear
 *  INPUTS: none
 * OUTPUTS: PASS/FAIL
 * RETURN VALUE: none
 * SIDE EFFECTS: none
*/
int terminal_clear_test(){
	TEST_HEADER;
	terminal_init();
	return PASS;
}

/*
 * terminal_nul_test
 *   DESCRIPTION: test the terminal of input nul
 *  INPUTS: none
 * OUTPUTS: PASS/FAIL
 * RETURN VALUE: none
 * SIDE EFFECTS: none
*/
int terminal_nul_test(){
	TEST_HEADER;
	terminal_init();
	buf[0]=0;
	terminal_display(fd, buf, nbytes);
	buf[0]='w';
	terminal_display(fd, buf, nbytes);  /* if the input is nul, we win if we still can input*/
	buf[0]='i';
	terminal_display(fd, buf, nbytes);
	buf[0]='n';
	terminal_display(fd, buf, nbytes);
	return PASS;
}
int read_by_name_test()
{
	TEST_HEADER;
	dentry_t dentry;
	char* str = "frame0.txt";
	read_dentry_by_name((uint8_t*)str,&dentry);
	printf("File Name: %s, File Type: %d, File Size: %d\n", dentry.file_name, dentry.file_type, (((inode_t*)((boot_block_t*)filesys_base+1))+dentry.inode_num)->length);
	uint8_t buffer[6000];
    int i = 0,num;
    num=read_data(dentry.inode_num,0,buffer,6000);
    for(i = 0; i< num; i++){
        putc(buffer[i]);
    }
	return PASS;
}
int read_by_index_test()
{
	TEST_HEADER;
	dentry_t dentry;
	read_dentry_by_index(10,&dentry);
	printf("File Name: %s, File Type: %d, File Size: %d\n", dentry.file_name, dentry.file_type, (((inode_t*)((boot_block_t*)filesys_base+1))+dentry.inode_num)->length);
	return PASS;
}
int read_data_test1()
{
	TEST_HEADER;
	dentry_t dentry;
	read_dentry_by_index(10,&dentry);
	uint8_t buffer[6000];
    int i = 0,num;
    num=read_data(dentry.inode_num,0,buffer,6000);
    for(i = 0; i< num; i++){
        putc(buffer[i]);
    }
	return PASS;
}
int read_data_test2()
{
	TEST_HEADER;
	dentry_t dentry;
	read_dentry_by_index(11,&dentry);
	uint8_t buffer[6000];
    int i = 0,num;
    num=read_data(dentry.inode_num,0,buffer,6000);
    for(i = 0; i< num; i++){
        putc(buffer[i]);
    }
	return PASS;
}
int read_data_test3()
{
	TEST_HEADER;
	dentry_t dentry;
	read_dentry_by_index(12,&dentry);
	uint8_t buffer[6000];
    int i = 0,num;
    num=read_data(dentry.inode_num,0,buffer,6000);
    for(i = 0; i< num; i++){
        putc(buffer[i]);
    }
	return PASS;
}
int show_file_on_terminal_test(){
	TEST_HEADER;
	dentry_t dentry;
	read_dentry_by_index(10,&dentry);
	uint8_t buffer[6000];
    int i = 0,num;
    num=read_data(dentry.inode_num,0,buffer,6000);
	terminal_init();
    for(i = 0; i< num; i++){
		buf[0]=buffer[i];
        terminal_display(fd, buf, nbytes);
    }
	return PASS;
}
int directory_read_test(){
	TEST_HEADER;
	int i;
	int j;
	int length;
	uint8_t buffer[33];
	dentry_t dentry;
	buffer[32] = '\0';
	for(i=0;i<63;i++){
		if((length = directory_read(i,buffer,&dentry)) == -1){
			break;
		}
		printf("file_name: ");
		for(j = 0; j< 32 - length; j++){
			printf(" ");
		}
		printf((int8_t*) buffer);
		printf(", file_type: %d, file_size: %d\n", dentry.file_type, (((inode_t*)((boot_block_t*)filesys_base+1))+dentry.inode_num)->length);
	}
	return PASS;
	
}
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	/* Checkpoint 1 tests */
	// TEST_OUTPUT("idt_test", idt_test());
	// TEST_OUTPUT("div_test",div_test());
	// TEST_OUTPUT("page_value_test",page_value_test());
	// TEST_OUTPUT("page_derefernece_test",page_dereference_test());
	// TEST_OUTPUT("syscall_test",syscall_test());
	// rtc_init();
	// launch your tests here

	/* Checkpoint 2 tests */

	/* terminal clear */
	terminal_clear();

	
	// rtc_driver_test();
	// terminal_keyboard_test();
	// terminal_clear_test();
	// terminal_nul_test();
	 read_by_name_test();															/* Check Point 3.2 # TEST 2*/
	// read_by_index_test();
	// read_data_test1();
	// read_data_test2();
	// read_data_test3();
	// TEST_OUTPUT("show_file_on_terminal_test",show_file_on_terminal_test());
	// TEST_OUTPUT("directory_read_test",directory_read_test());					/* Check Point 3.2 # TEST 1 */
	//printf("%d\n",filesys_base);
	//printf("%d\n",dentry.inode_num);
	//printf("File Name: %s, File Type: %d, File Size: %d\n", dentry.file_name, dentry.file_type, (((inode_t*)((boot_block_t*)filesys_base+1))+dentry.inode_num)->length);

	/* Checkpoint 3 tests */
	/*asm volatile(
	"				\n\
	movl $3, %%eax; \n\
	int $0x80;		\n\
	"
	:
	:
<<<<<<< HEAD
	:"eax"
	);
=======
	:"eax","ebx"
	);*/
}
