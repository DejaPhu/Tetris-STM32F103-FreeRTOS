/* GPIO reads are reentrant; initialization remains with CubeMX main.c. */
#include "board_pins.h"
#include "hal_gpio.h"
uint8_t hal_button_pressed(uint8_t index) {
    static GPIO_TypeDef *const ports[BOARD_BUTTON_COUNT]={LEFT_GPIO_Port,RIGHT_GPIO_Port,
        ROTATE_GPIO_Port,DOWN_GPIO_Port,PAUSE_GPIO_Port,HARD_DROP_GPIO_Port};
    static const uint16_t pins[BOARD_BUTTON_COUNT]={LEFT_Pin,RIGHT_Pin,ROTATE_Pin,
        DOWN_Pin,PAUSE_Pin,HARD_DROP_Pin};
    if(index>=BOARD_BUTTON_COUNT) return 0U;
    return HAL_GPIO_ReadPin(ports[index],pins[index])==GPIO_PIN_RESET;
}
