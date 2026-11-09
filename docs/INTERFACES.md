# Đặc tả giao diện module (chưa có code)

| Module dự kiến | Người viết | Nhiệm vụ |
|---|---|---|
| hal_buttons.h/.c, hal_uart.h/.c | TV1 | GPIO, UART |
| hal_buzzer.h/.c | TV2 | TIM4 PWM |
| hal_oled.h/.c | TV3 | SSD1306 I2C |
| task_input.h/.c | TV1 | Đọc nút, debounce, gửi Queue |
| task_game.h/.c | TV2 | Luật Tetris, FSM, điều kiện thắng |
| game_engine.h/.c, game_fsm.h/.c | TV2 | Logic thuần C, trạng thái |
| task_display.h/.c, ui_renderer.h/.c | TV3 | Menu + OLED |
| app_ipc.h/.c, app_cfg.h | Thống nhất cả nhóm | Queue, kiểu dữ liệu, chu kỳ và stack |

Thống nhất định dạng InputEvent, GameFrame, enum state và API trước khi viết module. Leader là người duy nhất chỉnh cấu hình CubeMX. Các thành viên không tự generate đè .ioc và không tạo duplicate Tasks.
