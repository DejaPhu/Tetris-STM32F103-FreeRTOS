#ifndef HAL_GPIO_H
#define HAL_GPIO_H
#include <stdint.h>
/* Index: LEFT, RIGHT, ROTATE, DOWN, PAUSE, DROP. Read-only GPIO API. */
uint8_t hal_button_pressed(uint8_t index);
#endif
