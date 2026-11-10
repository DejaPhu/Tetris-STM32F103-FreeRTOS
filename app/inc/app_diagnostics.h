#ifndef APP_DIAGNOSTICS_H
#define APP_DIAGNOSTICS_H
#include <stdint.h>
typedef enum { APP_FAULT_NONE,APP_FAULT_RTOS,APP_FAULT_MALLOC,APP_FAULT_STACK,APP_FAULT_PWM } AppFault;
typedef struct {
 uint32_t fault, input_queue_full, input_dropped, oled_errors;
 uint32_t game_max_ms, display_max_ms, heap_free, heap_min;
 uint32_t input_stack_free_words, game_stack_free_words, display_stack_free_words;
} AppDiagnostics;
extern volatile AppDiagnostics app_diagnostics;
void app_fault(AppFault reason);
void app_measure_game(uint32_t duration);
void app_measure_display(uint32_t duration,uint8_t ok);
#endif
