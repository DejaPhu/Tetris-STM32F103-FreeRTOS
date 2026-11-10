#ifndef HAL_BUZZER_H
#define HAL_BUZZER_H
#include <stdint.h>
uint8_t buzzer_init(void);
void tone(uint16_t hz, uint32_t ms);
void tone_poll(void);
#endif
