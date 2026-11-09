# Kiến trúc FreeRTOS — dự kiến, chưa implement

```text
6 Buttons / GPIO
    |
    v
InputTask -- xInputQueue --> GameTask (Tetris + FSM)
                                |
                                +--> PWM Buzzer
                                |
                                v
                           xFrameQueue
                                |
                                v
                            DisplayTask --> I2C1 OLED
```

- InputTask: khoảng 10 ms; chỉ đọc phím và phát sự kiện.
- GameTask: khoảng 20 ms; sở hữu trạng thái gameplay, menu, điểm và WIN.
- DisplayTask: khoảng 100 ms; nhận bản sao frame, vẽ đơn sắc SSD1306.
- Queue sự kiện phím (dự kiến 16 phần tử) và mailbox frame (dự kiến 1 phần tử).
- Không Task nào khác thay đổi dữ liệu game; tránh dùng framebuffer màu toàn màn hình.

**Chưa có code Task/Queue trong bộ khung.**
