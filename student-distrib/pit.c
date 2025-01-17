#include "pit.h"
#include "i8259.h"
#include "lib.h"
#include "lib.h"
#include "scheduler.h"

int counter;
int freqency;
int tick = 0;

/*
    *  void pit_init()
    *    DESCRIPTION: initialize the pit with the given frequency
    *    INPUT: fre
    *    OUTPUT: none
    *    SIDE EFFECT: 
*/
void pit_init(int fre){
    int div;
    counter = 0;
    freqency = fre;
    if(fre == -1){
        /* Use default value if input is -1 */
        fre = DEFAULT_FRE;
        freqency = fre;
    }
    div = PIT_FREQ / fre;
    outb(PIT_CMD,PIT_CMD_PORT);
    outb(div & 0x0ff, PIT_CH0_PORT);            /* sending the lower byte*/
    outb((div >> 8) & 0x0ff, PIT_CH0_PORT);       /* sending the higher byte*/
    enable_irq(PIT_IRQ);
}

/*
    *  void pit_handler()
    *    DESCRIPTION: handle the pit interrupt 
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: Do the process switch within the handler and increase the 
    *                 system time counter.
*/
void pit_handler(){
    send_eoi(PIT_IRQ);
    counter++;
    process_switch();
    
}

/*
    *  void get_systime()
    *    DESCRIPTION: return the system time
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: none
*/
systime_t get_systime(){
    systime_t systime;
    systime.sec = counter / freqency;
    systime.ms = (counter - systime.sec * freqency) * 1000 / freqency;
    return systime;
}
