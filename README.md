# Tetris STM32F103C8T6 + FreeRTOS

Starter scaffold for a three-member embedded-systems course project. **This is not yet buildable firmware.** Generate STM32CubeIDE/CubeMX Core, Drivers and FreeRTOS middleware files before compiling.

## Hardware and pins
- Blue Pill STM32F103C8T6 (72 MHz, 20 KB SRAM)
- ST7735 1.8 inch SPI TFT: PA5 SCK, PA7 MOSI, PA4 CS, PB0 DC, PB1 RST
- Buttons with pullups: PB12 left, PB13 right, PB14 rotate, PB15 soft drop, PA8 pause, PB10 hard drop
- Passive buzzer: PB6 TIM4_CH1 PWM
- USART1: PA9 TX, PA10 RX (3.3V TTL)
- ST-LINK SWD: PA13 SWDIO, PA14 SWCLK

## Three tasks
1. InputTask (10 ms) scans buttons, debounces, sends events to xInputQueue.
2. GameTask (20 ms) handles gameplay, sends the latest frame via xFrameQueue.
3. DisplayTask (~33 ms) draws changed tiles on the ST7735.

## Team assignments
- Member 1 / leader: CubeMX configuration, GPIO/UART, InputTask, Git integration
- Member 2: TIM4 buzzer, GameTask, Tetris engine
- Member 3: SPI ST7735, DisplayTask, renderer

## Git workflow
Keep main stable. Integrate on develop. Work on feature/hal-* and feature/task-*; integrate on integration/system-v1, review through PR, and release to main.

## Getting started
1. Generate an STM32F103C8Tx project in STM32CubeIDE; enable GPIO, SPI1, TIM4 PWM, USART1, SWD and FreeRTOS.
2. Add app/src and hal/src to the build, plus include directories app/inc, hal/inc and bsp.
3. Implement the driver stubs, game engine and renderer.
4. Initialize app IPC and create the three tasks in exactly one startup path: main.c or generated freertos.c, not both.
5. Measure stack high watermarks and heap. Avoid a full RGB565 framebuffer (40,960 bytes, exceeding the MCU's 20 KB RAM).
6. Validate hardware before merging to main.

See docs/ for architecture, flowcharts and tests.
