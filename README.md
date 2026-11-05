# Tetris STM32F103C8T6 + FreeRTOS

Starter repository for the 3-member embedded systems project. Source files are **scaffolds**, not yet a buildable firmware. Generate STM32CubeIDE/CubeMX `Core`, `Drivers` and `Middlewares` before compiling.

Hardware: STM32F103C8T6 Blue Pill, ST7735 SPI LCD, six GPIO buttons, TIM4 PWM buzzer, USART1 debug.

Architecture: InputTask -> `xInputQueue` -> GameTask -> `xFrameQueue` -> DisplayTask.

Git branches: `main` (stable), `develop` (integration), `feature/*` (each developer), `integration/system-v1` (system test).
