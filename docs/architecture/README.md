# Architecture

InputTask -> xInputQueue -> GameTask -> xFrameQueue -> DisplayTask -> ST7735.

GameTask exclusively owns game state and buzzer control. DisplayTask exclusively owns SPI1. The single-element frame queue carries the latest frame without shared board mutations. FreeRTOS is required and configured in STM32CubeIDE.
