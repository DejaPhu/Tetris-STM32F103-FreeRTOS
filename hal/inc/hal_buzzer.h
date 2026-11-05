#ifndef HAL_BUZZER_H
#define HAL_BUZZER_H
#include <stdint.h>
void hal_buzzer_play(uint16_t frequency_hz, uint16_t duration_ms);
void hal_buzzer_update(uint32_t elapsed_ms);
#endif
