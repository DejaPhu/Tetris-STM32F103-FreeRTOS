/* MEMBER 1: GPIO + buttons -> 16-element uint16_t CMSIS-RTOS2 input queue.
 * Input-only task. Menu mapping is handled by GameTask (state machine).
 */
#include <stdint.h>
#include "main.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "../inc/app_shared.h"

#define DEBOUNCE_SAMPLES   3U    /* 3 * 10 ms = 30 ms */
#define REPEAT_START_MS    300U
#define REPEAT_EVERY_MS    100U

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    ButtonCode_t code;
    uint8_t stable_pressed;
    uint8_t last_sample;
    uint8_t same_samples;
    uint32_t held_ms;
} Button_t;

static Button_t buttons[] = {
    {LEFT_GPIO_Port,      LEFT_Pin,      BTN_LEFT,   0, 0, 0, 0},
    {RIGHT_GPIO_Port,     RIGHT_Pin,     BTN_RIGHT,  0, 0, 0, 0},
    {ROTATE_GPIO_Port,    ROTATE_Pin,    BTN_ROTATE, 0, 0, 0, 0},
    {DOWN_GPIO_Port,      DOWN_Pin,      BTN_DOWN,   0, 0, 0, 0},
    {PAUSE_GPIO_Port,     PAUSE_Pin,     BTN_PAUSE,  0, 0, 0, 0},
    {HARD_DROP_GPIO_Port, HARD_DROP_Pin, BTN_DROP,   0, 0, 0, 0}
};

static void send_button(ButtonCode_t code, uint8_t repeated)
{
    uint16_t event = (uint16_t)code;
    if (repeated) event |= BUTTON_REPEAT_FLAG;
    /* No waiting for queue space: do not stall a high-priority input task. */
    (void)osMessageQueuePut(xInputQueueHandle, &event, 0U, 0U);
}

void task_input_run(void *argument)
{
    (void)argument;
    const TickType_t interval = pdMS_TO_TICKS(TETRIS_INPUT_PERIOD_MS);
    TickType_t last = xTaskGetTickCount();
    uint32_t i;

    for (i = 0; i < sizeof(buttons)/sizeof(buttons[0]); ++i) {
        buttons[i].last_sample =
            (HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == GPIO_PIN_RESET);
        buttons[i].stable_pressed = buttons[i].last_sample;
        buttons[i].same_samples = 0;
    }

    for (;;) {
        for (i = 0; i < sizeof(buttons)/sizeof(buttons[0]); ++i) {
            Button_t *b = &buttons[i];
            uint8_t sample =
                (HAL_GPIO_ReadPin(b->port, b->pin) == GPIO_PIN_RESET);

            if (sample == b->last_sample) {
                if (b->same_samples < DEBOUNCE_SAMPLES) b->same_samples++;
            } else {
                b->last_sample = sample;
                b->same_samples = 1U;
            }

            if (b->same_samples >= DEBOUNCE_SAMPLES &&
                sample != b->stable_pressed) {
                b->stable_pressed = sample;
                b->held_ms = 0U;
                if (sample) send_button(b->code, 0U);
            }

            if (b->stable_pressed) {
                /* Auto-repeat for movement keys only; GameTask ignores repeat
                 * outside STATE_PLAYING. Single-press for PAUSE/ROTATE/DROP. */
                b->held_ms += TETRIS_INPUT_PERIOD_MS;
                if ((b->code == BTN_LEFT || b->code == BTN_RIGHT ||
                     b->code == BTN_DOWN) &&
                    b->held_ms >= REPEAT_START_MS &&
                    ((b->held_ms - REPEAT_START_MS) % REPEAT_EVERY_MS) == 0U) {
                    send_button(b->code, 1U);
                }
            }
        }
        vTaskDelayUntil(&last, interval);
    }
}
