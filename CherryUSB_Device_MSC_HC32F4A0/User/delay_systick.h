
#ifndef _DELAY_SYSTICK_H_
#define _DELAY_SYSTICK_H_


#include <hc32_ll.h>

void delay_init(uint8_t SYS_CLK);
void delay_us(uint32_t n);
void delay_xms(uint16_t n);
void delay_ms(uint16_t n);



#endif /* SYS_TICK_H */
