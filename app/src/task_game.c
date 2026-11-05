#include "FreeRTOS.h"
#include "task.h"
#include "task_game.h"
#include "app_cfg.h"
#include "app_ipc.h"
/* TODO(member 2): implement engine state management and nonblocking buzzer. */
static void game_task(void *arg)
{
    (void)arg;
    TickType_t last = xTaskGetTickCount();
    InputEvent_t event;
    for (;;) {
        while (xQueueReceive(xInputQueue, &event, 0) == pdTRUE) {
            /* TODO: game_handle_input(event.command); */
        }
        /* TODO: game_step() and build GameFrame_t when state changes,
         * then xQueueOverwrite(xFrameQueue, &frame). */
        vTaskDelayUntil(&last, pdMS_TO_TICKS(GAME_PERIOD_MS));
    }
}
BaseType_t task_game_create(void)
{
    return xTaskCreate(game_task, "Game", GAME_STACK_WORDS,
                       NULL, GAME_TASK_PRIORITY, NULL);
}
