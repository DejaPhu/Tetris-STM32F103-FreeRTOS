#ifndef HAL_BUZZER_H
#define HAL_BUZZER_H
#include <stdint.h>
/* Safe before timer init and on fatal errors: drive the inactive GPIO level. */
void buzzer_hold_idle(void);
uint8_t buzzer_init(void);
void tone(uint16_t hz, uint32_t ms);
void tone_poll(void);
#endif
