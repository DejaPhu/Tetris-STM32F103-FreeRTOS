# Kiến trúc tích hợp

```mermaid
flowchart LR
 GPIO[6 nút GPIO] --> Input[InputTask 10 ms]
 Input -->|Queue 16 x uint16_t| Game[GameTask: FSM và engine]
 Game --> PWM[TIM4 PWM]
 Game --> Snapshot[Snapshot có critical section]
 Game -->|Queue 1 x uint16_t token| Display[DisplayTask 160 ms]
 Snapshot --> Display
 Display --> Render[Renderer 1024 byte]
 Render --> OLED[SSD1306 I2C1]
```

Task/Queue được tạo duy nhất trong main.c bởi CubeMX. Không thêm Task diagnostics hoặc buzzer. Ba wrapper gọi module trong USER CODE. GameTask ngủ tối đa 20 ms, thức sớm theo deadline gravity. RTOS SysTick 1 kHz; HAL tick TIM2. Chỉ DisplayTask truy cập I2C1, chỉ GameTask điều khiển game/PWM. Kernel còn Idle/Timer service nội bộ.

Xem docs/MODULES.md và docs/test-results/SYSTEM_V1.md; chưa kiểm thử board.
