# Pin mapping dự kiến: STM32F103C8T6 Blue Pill

| Kết nối | Chân MCU | Cấu hình |
|---|---|---|
| OLED SCL | PB8 | I2C1_SCL, remap |
| OLED SDA | PB9 | I2C1_SDA, remap |
| Buzzer | PB6 | TIM4_CH1, PWM |
| LEFT | PB12 | GPIO Input Pull-up |
| RIGHT | PB13 | GPIO Input Pull-up |
| ROTATE | PB14 | GPIO Input Pull-up |
| DOWN | PB15 | GPIO Input Pull-up |
| PAUSE | PA8 | GPIO Input Pull-up |
| HARD DROP | PB10 | GPIO Input Pull-up |
| UART TX/RX | PA9/PA10 | USART1 115200, 8N1 |
| ST-Link | PA13/PA14 | SWD |
| HAL Timebase | TIM2 | SysTick dành cho FreeRTOS |

Nút nhấn nối GPIO xuống GND; active-low. Dùng mức 3.3V cho I2C và UART. Kiểm tra nguồn, pull-up OLED và dòng buzzer trước khi nối thực tế.
