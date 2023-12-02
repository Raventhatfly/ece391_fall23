#ifndef _SERIAL_H
#define _SERIAL_H
#include "types.h"

#define PORT 0x3f8          /* COM 1 */
// #define PORT 0x3E8          /* COM 3 */
#define SERIAL_IRQ 4
#define SERIAL_BUFFER_LEN 100
#define MODE_DIRECT_PRINT 1
#define MODE_INPUT_BUFFER 2

typedef struct serial_buffer{   
    int head;
    int tail;
    int length;
    int8_t buf[SERIAL_BUFFER_LEN];
}serial_buffer_t;

void serial_init();
void serial_handler();
void serial_set_mode(int mode);
int32_t serial_read();
void serial_send(int8_t byte);
void serial_loopback_enable();
void serial_loopback_disable();
void serial_off();
void test_serial();

#endif
