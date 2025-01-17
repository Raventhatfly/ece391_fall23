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

/*
    * serial_init()
    *   DESCRIPTION: initialize the serial driver
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
void serial_init(){
    /* @cite: https://wiki.osdev.org/Serial_Ports */
    sbuf.head = 0;
    sbuf.tail = 0;
    sbuf.length = 0;
    
    serial_set_mode(MODE_DIRECT_PRINT);
    // serial_set_mode(MODE_INPUT_BUFFER);
    enable_irq(SERIAL_IRQ);

    cli();
    outb(0x00, PORT+1);         // Disable all interrupts
    outb(0x80, PORT+3);         // Enable DLAB (set baud rate divisor)
    outb(0x01, PORT+0);         // Set divisor to 1 (lo byte) 115200 baud
    outb(0x00, PORT+1);         //                  (hi byte)
    outb(0x03, PORT+3);         // 8 bits, no parity, one stop bit
    outb(0xc7, PORT+2);         // Enable FIFO, clear them, with 14-byte threshold
    outb(0x0c, PORT+4);         // IRQs enabled, RTS/DSR set
    // outb(0x0b, PORT+1);         // Enable interrupts (status change)
    // outb(0x1e, PORT+4);         // Set in loopback mode, test the serial chip
    outb(0x0b, PORT+1);         // Enable interrupts (status change)
    sti();
    
    // outb(0xae, PORT+0);         // Test serial chip (send byte 0xAE and check if serial returns same byte)

     // enable_irq(SERIAL_IRQ);
    enable_irq(SERIAL_IRQ);
    
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

/*
    * serial_handler()
    *   DESCRIPTION: serial interrupt handler
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
void serial_handler(){
    int8_t data;
    cli();
    if(serial_mode == MODE_DIRECT_PRINT){
        data = (int8_t) inb(PORT);
        printf("%c", data);
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

/*
    * serial_read()
    *   DESCRIPTION: read the serial input from the buffer
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
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

/*
    * set_serial_mode()
    *   DESCRIPTION: set the mode of the serial driver
    *   INPUTS: mode
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
void serial_set_mode(int mode){
    serial_mode = mode;
}

/*
    * serial_off()
    *   DESCRIPTION: read the serial input from the buffer
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
void serial_off(){
    outb(0x00, PORT+1);         /* serial interrupt off */
}

/*
    * serial_send()
    *   DESCRIPTION: turn off the serial
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
void serial_send(int8_t byte){
    cli();
    outb(byte, PORT+0);
    sti();
    // cli();
    // outb('\n', PORT+0);
    // sti();       
}

/*
    * serial_loopback_enable()
    *   DESCRIPTION: serial loopback mode enabled
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
void serial_loopback_enable(){
    cli();
    outb(0x1e, PORT+4);         
    sti();
}

/*
    * serial_loopback_enable()
    *   DESCRIPTION: serial loopback mode disabled
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: none
*/
void serial_loopback_disable(){
    cli();
    outb(0x0f,PORT+4);      
    sti();
}
