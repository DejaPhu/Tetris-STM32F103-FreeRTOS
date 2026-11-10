#include "button_filter.h"
uint8_t button_filter_step(ButtonFilter *b,uint8_t pressed,uint8_t repeat,uint32_t now) {
    if (pressed != b->sample) { b->sample=pressed; b->changed_at=now; }
    if (pressed != b->stable && (uint32_t)(now-b->changed_at)>=30U) {
        b->stable=pressed; b->repeat_at=now+300U;
        if (pressed) return 1U;
    }
    /* Stop repeat as soon as release is sampled, even during release debounce. */
    if (repeat && pressed && b->stable && (int32_t)(now-b->repeat_at)>=0) {
        b->repeat_at=now+100U; return 2U;
    }
    return 0U;
}
