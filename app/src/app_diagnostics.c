/* Inspect app_diagnostics in the Keil Watch window; no fourth application Task. */
#include "main.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_diagnostics.h"
volatile AppDiagnostics app_diagnostics;
extern osThreadId_t InputTaskHandle,GameTaskHandle,DisplayTaskHandle;
void app_fault(AppFault reason) {
    app_diagnostics.fault=reason;
    /* Stop PWM without using a blocking HAL API after a scheduler/memory failure. */
    TIM4->CCER &= ~TIM_CCER_CC1E;
    __HAL_RCC_TIM4_CLK_DISABLE();
    __disable_irq();
    for (;;) { /* Break here in ST-Link to inspect fault. */ }
}
void app_measure_game(uint32_t duration) {
    static uint32_t sampled;
    uint32_t now=HAL_GetTick();
    if (duration>app_diagnostics.game_max_ms) app_diagnostics.game_max_ms=duration;
    if ((uint32_t)(now-sampled)<1000U) return;
    sampled=now;
    app_diagnostics.heap_free=xPortGetFreeHeapSize();
    app_diagnostics.heap_min=xPortGetMinimumEverFreeHeapSize();
    app_diagnostics.input_stack_free_words=uxTaskGetStackHighWaterMark((TaskHandle_t)InputTaskHandle);
    app_diagnostics.game_stack_free_words=uxTaskGetStackHighWaterMark((TaskHandle_t)GameTaskHandle);
    app_diagnostics.display_stack_free_words=uxTaskGetStackHighWaterMark((TaskHandle_t)DisplayTaskHandle);
}
void app_measure_display(uint32_t duration,uint8_t ok) {
    if (duration>app_diagnostics.display_max_ms) app_diagnostics.display_max_ms=duration;
    if (!ok) app_diagnostics.oled_errors++;
}
