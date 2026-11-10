#ifndef HAL_OLED_H
#define HAL_OLED_H
#include <stdint.h>
void oled_recover(void);
uint8_t oled_init(void);
uint8_t oled_flush(const uint8_t *framebuffer);
#endif
