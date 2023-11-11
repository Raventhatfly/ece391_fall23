#ifndef _PIT_H
#define _PIT_H

#define PIT_CMD 0x36            
#define PIT_CMD_PORT 0x43
#define PIT_CH0_PORT 0x40
#define PIT_CH1_PORT 0x41
#define PIT_CH2_PORT 0x42

#define DEFAULT_FRE 100 /* Hz */

#define PIT_FREQ 1193182 /* Hz */
#define PIT_IRQ 0
/* if freq == -1, use default; else we can set fre we like */
void pit_init(int fre);
void pit_handler();
#endif  /* _PIT_H */
