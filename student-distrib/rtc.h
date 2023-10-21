#ifndef _RTC_H
#define _RTC_H

#include "types.h"

#define BASE_RATE 1024

#define REG_A 0x8A
#define REG_B 0x8B
#define REG_C 0x8C

#define REG_SELECT 0X70
#define REG_DATA 0x71
#define RTC_IRQ 8


/* Initialize RTC */
extern void rtc_init(void);
extern int rtc_open(const uint8_t* filename);
extern int rtc_close(int32_t fd);
extern int rtc_read(int32_t fd, void* buf, int32_t nbytes);
extern int rtc_write(int32_t fd, const void* buf, int32_t nbytes);
extern void rtc_handler(void)

#endif /* _I8259_H */

