# Tetris STM32F103C8T6 + FreeRTOS — PROJECT SKELETON

**Giai đoạn: khởi tạo bộ khung. Chưa có mã nguồn, firmware, driver, .ioc hoặc Task chạy thật.**
Ba thành viên tự viết code trong nhánh riêng, mở Pull Request và chờ review.

## Phần cứng dự kiến
- STM32F103C8T6 Blue Pill, 72 MHz, RAM 20 KB
- OLED SSD1306 128x64 I2C1 remap PB8/PB9
- Passive buzzer TIM4_CH1 PWM trên PB6
- 6 nút: PB12 LEFT, PB13 RIGHT, PB14 ROTATE, PB15 DOWN, PA8 PAUSE, PB10 DROP
- Debug USART1 PA9/PA10 và SWD PA13/PA14; HAL tick TIM2

## Phần mềm sẽ triển khai
- InputTask: quét nút, chống rung, gửi input queue
- GameTask: Tetris engine + FSM + độ khó EASY/NORMAL/HARD + điều kiện YOU WIN
- DisplayTask: OLED và menu, đọc frame queue
- Tách BSP, HAL, APP, FreeRTOS. Không viết toàn bộ trong main.c.

## Git workflow
- main: chỉ code ổn định/đã được kiểm thử; không commit trực tiếp.
- develop: tích hợp chung; bộ khung đang nằm ở đây.
- feature/hal-* và feature/task-*: mỗi cá nhân tự triển khai.
- integration/system-v1: ghép các module và test trên Blue Pill.
- release: chốt bản sẵn sàng nộp, sau review mới merge main/tag.

Đọc docs/GIT_WORKFLOW.md, docs/TEAM_ASSIGNMENT.md và docs/INTERFACES.md trước khi viết code.
Leader sẽ thêm dự án STM32CubeIDE và file .ioc sau trên nhánh cấu hình riêng.
