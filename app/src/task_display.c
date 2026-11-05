#include "FreeRTOS.h"
#include "task.h"
#include "task_display.h"
#include "app_cfg.h"
#include "app_ipc.h"
/* TODO(member 3): ST7735 initialize and dirty-cell redraw. */
static void display_task(void *arg)
{
    (void)arg;
    TickType_t last = xTaskGetTickCount();
    GameFrame_t frame;
    for (;;) {
        if (xQueueReceive(xFrameQueue, &frame, 0) == pdTRUE) {
            /* TODO: ui_render_frame(&frame); */
        }
        vTaskDelayUntil(&last, pdMS_TO_TICKS(DISPLAY_PERIOD_MS));
    }
}
BaseType_t task_display_create(void)
{
    return xTaskCreate(display_task, "Display", DISPLAY_STACK_WORDS,
                       NULL, DISPLAY_TASK_PRIORITY, NULL);
}
