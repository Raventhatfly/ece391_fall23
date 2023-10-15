#ifndef _RTC_H
#define _RTC_H

#include "types.h"

#define RTC_RATE 15

#define REG_A 0x8A
#define REG_B 0x8B
#define REG_C 0x8C

#define REG_SELECT 0X70
#define REG_DATA 0x71
#define rtc_irq 8

#define RTC_IRQ         0x8

volatile int INT_FLAG;

/* Initialize RTC */
extern void rtc_init(void);

#endif /* _I8259_H */
