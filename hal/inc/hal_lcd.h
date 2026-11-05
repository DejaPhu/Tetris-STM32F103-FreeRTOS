#ifndef HAL_LCD_H
#define HAL_LCD_H
#include <stdint.h>
void hal_lcd_init(void);
void hal_lcd_fill_rect(uint16_t x, uint16_t y,
                       uint16_t w, uint16_t h, uint16_t color);
#endif
