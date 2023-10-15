#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

#define PASS 1
#define FAIL 0

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
	unsigned char* video_mem_start = (unsigned char*) 0xB8000;
	unsigned char* video_mem_end   = video_mem_start + 0x1000;
	unsigned char* kernel_start    = (unsigned char*) 0x400000;
	unsigned char* kernel_end      = (unsigned char*) 0x800000;
	unsigned char* ptr;
	unsigned char test;
	for (ptr = video_mem_start; ptr < video_mem_end; ptr++) { /* test video memory */
		test = (*ptr);
	}

	for (ptr = kernel_start; ptr < kernel_end; ptr++) { /* test kernel memory */
		test = (*ptr);
	}
	return FAIL;
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

/*
 * page_dereference_dne_test
 *   DESCRIPTION: test the page dereference does not exist
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
*/
int page_dereference_dne_test(){
	TEST_HEADER;
	int* ptr = (int*)0x800001;
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
	TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	TEST_OUTPUT("div_test",div_test());
	TEST_OUTPUT("page_value_test",page_value_test());
	TEST_OUTPUT("page_derefernece_test",page_dereference_test());
	TEST_OUTPUT("page_derefernece_dne_test",page_dereference_dne_test());
	
}
