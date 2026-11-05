#ifndef HAL_BUTTONS_H
#define HAL_BUTTONS_H
#include <stdint.h>
typedef enum {
    BTN_LEFT, BTN_RIGHT, BTN_ROTATE, BTN_DOWN,
    BTN_PAUSE, BTN_DROP, BTN_COUNT
} ButtonId_t;
uint8_t hal_buttons_pressed(ButtonId_t id);
#endif
