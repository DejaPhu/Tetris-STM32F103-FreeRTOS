#ifndef BUTTON_FILTER_H
#define BUTTON_FILTER_H
#include <stdint.h>
typedef struct { uint32_t changed_at, repeat_at; uint8_t sample, stable; } ButtonFilter;
/* Returns 1 for a press, 2 for repeat, 0 otherwise. Elapsed time tolerates wrap. */
uint8_t button_filter_step(ButtonFilter *b,uint8_t pressed,uint8_t repeat,uint32_t now);
#endif
