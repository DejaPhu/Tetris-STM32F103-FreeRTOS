#include "FreeRTOS.h"
#include "task.h"
#include "task_input.h"
#include "app_cfg.h"
#include "app_ipc.h"
/* TODO(member 1): GPIO scanning, 30ms debounce, auto-repeat and Queue sends.
 * Use vTaskDelayUntil rather than HAL_Delay in tasks. */
static void input_task(void *arg)
{
    (void)arg;
    TickType_t last = xTaskGetTickCount();
    for (;;) {
        /* TODO: read buttons and xQueueSend(xInputQueue, &event, 0). */
        vTaskDelayUntil(&last, pdMS_TO_TICKS(INPUT_PERIOD_MS));
    }
}
BaseType_t task_input_create(void)
{
    return xTaskCreate(input_task, "Input", INPUT_STACK_WORDS,
                       NULL, INPUT_TASK_PRIORITY, NULL);
}
