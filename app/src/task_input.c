/* Task 1: active-low GPIO -> uint16_t events. Never interpret the game state. */
#include "main.h"
#include "hal_gpio.h"
#include "app_log.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_shared.h"
#include "button_filter.h"
#include "app_diagnostics.h"
typedef struct { uint8_t code; ButtonFilter filter; uint8_t pending; } Button;
static Button buttons[]={
 {BTN_LEFT,{0},0},
 {BTN_RIGHT,{0},0},
 {BTN_ROTATE,{0},0},
 {BTN_DOWN,{0},0},
 {BTN_PAUSE,{0},0},
 {BTN_DROP,{0},0}
};
void task_input_run(void *argument) {
    TickType_t last=xTaskGetTickCount();
    uint32_t raw_sampled=0U;
    (void)argument;
    for (;;) {
        uint32_t i,now=HAL_GetTick();
        /* Retry PAUSE first when capacity returns; queued data is never reset here. */
        for (i=0;i<6U;i++) {
            uint32_t index=(i==0U)?4U:((i<=4U)?i-1U:5U);
            Button *b=&buttons[index];
            uint8_t repeat=(b->code==BTN_LEFT||b->code==BTN_RIGHT||b->code==BTN_DOWN);
            uint8_t event=button_filter_step(&b->filter,
                hal_button_pressed((uint8_t)index),repeat,now);
            uint16_t code=b->code;
            if (event==1U) {
                if (b->pending) app_diagnostics.input_dropped++;
                b->pending=1U;
                app_log("KEY_PRESS",b->code,index,now);
            }
            if (b->pending) {
                if (osMessageQueuePut(xInputQueueHandle,&code,0U,0U)==osOK) {
                    b->pending=0U;app_log("KEY_ENQ",code,index,0U);
                }
                else app_diagnostics.input_queue_full++;
            } else if (event==2U) {
                code|=BUTTON_REPEAT_FLAG;
                if (osMessageQueuePut(xInputQueueHandle,&code,0U,0U)!=osOK)
                    app_diagnostics.input_dropped++;
            }
        }
        if ((uint32_t)(now-raw_sampled)>=1000U) {
            uint32_t pressed=0U;
            raw_sampled=now;
            for(i=0;i<6U;i++) if(hal_button_pressed((uint8_t)i))pressed|=1U<<i;
            app_log("INPUT_RAW",pressed,(GPIOB->IDR>>10U)&1U,(GPIOB->CRH>>8U)&15U);
        }
        vTaskDelayUntil(&last,pdMS_TO_TICKS(TETRIS_INPUT_PERIOD_MS));
    }
}
