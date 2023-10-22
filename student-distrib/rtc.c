/* rtc.c - Functions to interact with RTC */

#include "rtc.h"
#include "lib.h"
#include "i8259.h"

volatile uint32_t int_counter;
volatile uint32_t int_flag[10];
volatile uint32_t rtc_rate;


/* 
 * The int_flag handles rtc frequency from 2 ~ 1024 with the following index:
 *  index:      0    1    2    3    4    5    6    7    8    9
 *  freq(Hz):   2    4    8    16   32   64   128  256  512  1024
*/


/*
    *  void rtc_init()
    *    DESCRIPTION: initialize the rtc
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: - Configures the RTC control registers A and B to enable periodic 
    *                   interrupts and set the interrupt rate.
    *                 - Enables the RTC IRQ line on the PIC (Programmable Interrupt Controller),
    *                   allowing the system to receive and handle RTC interrupts.
*/
void rtc_init(void) {
    char prev;
    outb(REG_B,REG_SELECT);
    prev = inb(REG_DATA);
    outb(REG_B,REG_SELECT);
    outb(prev | 0x40,REG_DATA);       /* Enable the Periodic Interrupt */
    
    outb(REG_A,REG_SELECT);
    prev = inb(REG_DATA);
    outb(REG_A,REG_SELECT);
    
    /* Set the interrupt rate while preserving the upper 4 bits of Register A */
    outb((prev & 0xF0) | BASE_RATE, REG_DATA);

    int i;
    for (i=0; i<10; i++) {int_flag[i]=0;}
    int_counter=0;
    rtc_rate=9;

    enable_irq(RTC_IRQ);              /* Enable RTC interrupt */
}

/*
    *  void rtc_handler()
    *    DESCRIPTION: Handles the RTC (Real-Time Clock) interrupt by reading 
    *                 from the RTC to clear the interrupt flag, setting a flag 
    *                 for each different frequencies in the int_flag[10]
    *                 to indicate an interrupt has occurred, and sending an 
    *                 End Of Interrupt (EOI) signal to the Programmable 
    *                 Interrupt Controller (PIC).
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: 
    *                 - Sends an End Of Interrupt (EOI) signal to the PIC.
    *                 - Temporarily disables and then re-enables interrupts to 
    *                   ensure atomic operations.
*/
void rtc_handler(void){
    /* disable interrupt */
    cli();
    outb(REG_C, REG_SELECT);
    inb(REG_DATA);
    int_counter++;
    int i;
    for (i=0; i<10; i++) {
        if ( int_counter % (BASE_RATE / (2 << i)) == 0 ) {
            int_flag[i]=1;
        }
    }
    // test_interrupts();
    send_eoi(RTC_IRQ);              /* End Of Interrupt (EOI) signal */
    sti();                          /* re-enable interrupt */
}

/*
    *  int rtc_open(const uint8_t* filename)
    *    DESCRIPTION: Opens the RTC with a default frequency.
    *    INPUT: filename - Not used.
    *    OUTPUT: Returns 0 on success.
    *    SIDE EFFECTS: Resets the RTC frequency to the default value.
*/
int rtc_open(const uint8_t* filename) {
    rtc_rate=0;
    int_flag[rtc_rate]=0;
	return 0;
}

/*
    *  int rtc_close(int32_t fd)
    *    DESCRIPTION: Closes the RTC.
    *    INPUT: fd - File descriptor, not used.
    *    OUTPUT: Returns 0.
    *    SIDE EFFECTS: None.
*/
int rtc_close(int32_t fd) {
	return 0;
}

/*
    *  int log_2(uint32_t n)
    *    DESCRIPTION: Calculates the base-2 logarithm of n.
    *    INPUT: n - The value to compute the logarithm for.
    *    OUTPUT: Returns the base-2 logarithm of n.
    *    SIDE EFFECTS: None.
*/
int log_2(uint32_t n) {
    int ans=0;
    while (n) {
        ans++;
        n>>=1;
    }
    return ans;
}

/*
    *  int rtc_write(int32_t fd, const void* buf, int32_t nbytes)
    *    DESCRIPTION: Sets the RTC frequency.
    *    INPUT: fd - Not used.
    *            buf - Pointer to a buffer containing the new frequency.
    *            nbytes - Not used.
    *    OUTPUT: Returns 0 on success, -1 on failure.
    *    SIDE EFFECTS: Updates the RTC frequency.
*/
int rtc_write(int32_t fd, const void* buf, int32_t nbytes) {
    uint32_t freq = *(uint32_t*) buf;
    if (freq<=0 || freq>BASE_RATE) {return -1;}     /* check if between 1~1024 */
    if ((freq & (freq-1)) != 0) {return -1;}        /* check if power of 2 */
    rtc_rate=log_2(freq)-1;
    return 0;
}

/*
    *  int rtc_read(int32_t fd, void* buf, int32_t nbytes)
    *    DESCRIPTION: Blocks until the next RTC interrupt occurs.
    *    INPUT: fd - Not used.
    *            buf - Not used.
    *            nbytes - Not used.
    *    OUTPUT: Returns 0.
    *    SIDE EFFECTS: Resets the interrupt flag for the current frequency.
*/
int rtc_read(int32_t fd, void* buf, int32_t nbytes) {
    while (int_flag[rtc_rate]==0) {}
    int_flag[rtc_rate]=0;
    return 0;
}

