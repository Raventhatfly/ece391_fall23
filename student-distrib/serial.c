#include "serial.h"
#include "lib.h"
#include "i8259.h"

void serial_init(){
    /* @cite: https://wiki.osdev.org/Serial_Ports */
    outb(0x00, PORT+1);         // Disable all interrupts
    outb(0x80, PORT+3);         // Set divisor to 3 (lo byte) 38400 baud
    outb(0x03, PORT+0);         // Set divisor to 3 (lo byte) 38400 baud
    outb(0x00, PORT+1);         //                  (hi byte)
    outb(0x03, PORT+3);         // 8 bits, no parity, one stop bit
    outb(0xc7, PORT+2);         // Enable FIFO, clear them, with 14-byte threshold
    outb(0x0b, PORT+4);         // IRQs enabled, RTS/DSR set
    outb(0x1e, PORT+4);         // Set in loopback mode, test the serial chip

    enable_irq(SERIAL_IRQ);
    outb(0xae, PORT+0);         // Test serial chip (send byte 0xAE and check if serial returns same byte)
   
    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(PORT + 0) != 0xAE) {
        WARNING("Serial initialization failed!");
        return;
    }

    // If serial is not faulty set it in normal operation mode
    
    printf("Serial initialization success!\n");
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    // outb(0x0f,PORT+4);

}

void serial_handler(){
    printf("Serial Interrupt Occurred!\n");
    send_eoi(SERIAL_IRQ);
}

int serial_received() {
   return inb(PORT + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return inb(PORT);
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}
 
void write_serial(char a) {
    while (is_transmit_empty() == 0);
    outb(a,PORT);
}