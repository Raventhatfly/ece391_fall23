#include "pit.h"
#include "i8259.h"
#include "lib.h"
#include "lib.h"

int counter = 0;
int tick = 0;
void pit_init(int fre){
    int div;
    if(fre == -1){
        /* Use default value if input is -1 */
        fre = DEFAULT_FRE;
    }
    div = PIT_FREQ / fre;
    outb(PIT_CMD,PIT_CMD_PORT);
    outb(div & 0x0ff, PIT_CH0_PORT);            /* sending the lower byte*/
    outb((div >> 8) & 0x0ff, PIT_CH0_PORT);       /* sending the higher byte*/
    enable_irq(PIT_IRQ);
}

void pit_handler(){
    send_eoi(PIT_IRQ);
    if(++counter > 100){
        tick++;
        printf("PIT recieved at tick %d\n",tick);
        counter = 0;
    }
    
}
