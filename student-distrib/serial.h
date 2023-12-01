#ifndef _SERIAL_H
#define _SERIAL_H

// #define PORT 0x3f8          /* COM 1 */
#define PORT 0x3E8          /* COM 3 */
#define SERIAL_IRQ 4
void serial_init();
void serial_handler();
int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);

#endif
