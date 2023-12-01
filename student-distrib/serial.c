#include "serial.h"
#include "lib.h"
#include "i8259.h"

// static int8_t serial_buffer[SERIAL_BUFFER_LEN];
// static int8_t serial_output_buffer[SERIAL_BUFFER_LEN];
// static int serial_buffer_start;
// static int serial_buffer_end;
static serial_buffer_t sbuf;
static int serial_mode;
int buffer_data_cnt;    /* In Bytes */
void serial_init(){
    /* @cite: https://wiki.osdev.org/Serial_Ports */
    sbuf.head = 0;
    sbuf.tail = 0;
    sbuf.length = 0;
    
    serial_set_mode(MODE_DIRECT_PRINT);

    outb(0x00, PORT+1);         // Disable all interrupts
    outb(0x80, PORT+3);         // Set divisor to 3 (lo byte) 38400 baud
    outb(0x03, PORT+0);         // Set divisor to 3 (lo byte) 38400 baud
    outb(0x00, PORT+1);         //                  (hi byte)
    outb(0x03, PORT+3);         // 8 bits, no parity, one stop bit
    outb(0xc7, PORT+2);         // Enable FIFO, clear them, with 14-byte threshold
    outb(0x0b, PORT+4);         // IRQs enabled, RTS/DSR set
    outb(0x0b, PORT+1);         // Enable interrupts (status change)
    outb(0x1e, PORT+4);         // Set in loopback mode, test the serial chip

    
    outb(0xae, PORT+0);         // Test serial chip (send byte 0xAE and check if serial returns same byte)

     // enable_irq(SERIAL_IRQ);
    enable_irq(4);
    
    // Check if serial is faulty (i.e: not same byte as sent)
    // if(inb(PORT + 0) != 0xAE) {
    //     WARNING("Serial initialization failed!");
    //     return;
    // }else{
    //     printf("Serial initialization success!\n");
    // }

    // If serial is not faulty set it in normal operation mode
    
    
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    // outb(0x0f,PORT+4);

}

void serial_handler(){
    int8_t data;
    cli();
    if(serial_mode == MODE_DIRECT_PRINT){
        data = (int8_t) inb(PORT);
        printf("%d", data);
    }else if(serial_mode == MODE_INPUT_BUFFER){
        data = (int8_t) inb(PORT);
        if(sbuf.length < SERIAL_BUFFER_LEN){
            sbuf.buf[sbuf.tail] = data;
            sbuf.tail = (sbuf.tail + 1) % SERIAL_BUFFER_LEN;
            sbuf.length++;
        }
    }
    send_eoi(SERIAL_IRQ);
    sti();
}

int32_t serial_read(){
    int8_t byte;
    if(sbuf.length == 0){
        return -1;
    }else{
        byte = sbuf.buf[sbuf.head];
        sbuf.tail = (sbuf.tail + 1) % SERIAL_BUFFER_LEN;
        sbuf.length--;
        return (int32_t) byte;
    }
}

void serial_set_mode(int mode){
    serial_mode = mode;
}

void serial_off(){
    outb(0x00, PORT+1);         /* serial interrupt off */
}
