#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "filesystem.h"
#include "terminal.h"

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
	dentry_t dentry;
	char* str = "sigtest";
	read_dentry_by_name(str,&dentry);
	// printf("File Name: %s, File Type: %d, File Inode: %d\n", dentry.file_name, dentry.file_type, dentry.inode_num);

	read_dentry_by_index(1,&dentry);
	// printf("File Name: %s, File Type: %d, File Inode: %d\n", dentry.file_name, dentry.file_type, dentry.inode_num);

	/* terminal clear */
	terminal_clear();
	


	int num=0,i=0;
	printf("File Name: %s, File Type: %d, File Size: %d\n", dentry.file_name, dentry.file_type, filesys_base);//((inode_t*)filesys_base[ (1+dentry.inode_num)*4096])->length);
}
