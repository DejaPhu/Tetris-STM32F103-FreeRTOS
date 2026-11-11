# Tetris — STM32F103C8T6 / Keil µVision 5 / HAL / CMSIS-RTOS v2

Bản tích hợp có đầy đủ nguồn Keil, ba Task và module game/OLED/buzzer. **Đã kiểm thử logic trên host và cross-build ARM GCC; chưa build lại bản tích hợp bằng Keil hoặc chạy trên board.** Kết quả build Keil trước đó của người dùng chỉ áp dụng cho bộ khung.

## Mở project

1. Checkout `develop` sau khi PR tích hợp được merge hoặc dùng `integration/system-v1` và giải nén toàn bộ. Giữ nguyên toàn bộ cây thư mục; xem [cấu trúc theo PDF](docs/REPO_STRUCTURE.md).
2. Mở `Teris/MDK-ARM/Teris.uvprojx` bằng Keil µVision 5. Dùng Arm Compiler 5 phù hợp port FreeRTOS RVDS/ARM_CM3, bật C99 như project. Không chuyển Compiler 6 tự động.
3. Target: STM32F103C8, IROM1 `0x08000000/0x10000`, IRAM1 `0x20000000/0x5000`; IROM1 Startup. Không dùng Flash 128 KB trên chip C8.
4. Rebuild All, lưu Build Output và `.map`, sau đó nạp bằng ST-Link/SWD theo [hướng dẫn](docs/BOARD_TEST.md).

Không cần copy thủ công ba Task hoặc thêm lại Queue/Task: các nguồn đã có trong project. Không dùng `tests/host_stubs` trong Keil; đó chỉ là kiểu dữ liệu giả cho host tests.

## Chức năng

Bàn 10×20, 7 khối, xoay/wall-kick đơn giản (không phải full SRS), xóa 1–4 hàng, điểm 100/300/500/800 nhân level trước lần xóa; mỗi 10 hàng tăng level. Độ khó cố định EASY 800 ms, NORMAL 500 ms, HARD 250 ms. WIN khi score >=1000 ưu tiên trước spawn/Game Over. Có WELCOME, MAIN_MENU, HELP, DIFFICULTY, PLAYING, PAUSED, WIN, GAME_OVER; Retry, Main Menu, next preview, score/lines/level. Không có hold hoặc lưu high score sau mất nguồn.

| Nút | Chân | Gameplay | Menu |
|---|---|---|---|
| LEFT | PB12 | Trái | Back tùy màn |
| RIGHT | PB13 | Phải | Xác nhận |
| ROTATE | PB14 | Xoay | Lên |
| DOWN | PB15 | Soft drop | Xuống |
| PAUSE | PA8 | Pause | Back/Resume |
| HARD_DROP | PB10 | Hard drop | OK |

Nút nối xuống GND, active-low/pull-up. OLED SSD1306 128×64 I2C1: PB8 SCL/PB9 SDA, remap, 7-bit 0x3C, pull-up lên 3.3 V. Buzzer TIM4_CH1 PB6: bản này đã chỉnh cho module ba chân **kích LOW** người dùng xác nhận; xem [nối dây và giới hạn chủ động/thụ động](docs/BUZZER_MODULE.md). UART PA9/PA10 115200; SWD PA13/PA14. Không đưa 5 V vào GPIO.

## Kiến trúc

- `bsp/` chứa pin/MCU; `hal/` chứa driver; `middlewares/` chứa CMSIS/FreeRTOS; `main.c` ở gốc; `app_cfg.h`, `app_ipc.h`, `task_*.h` là contract chung.
- InputTask 10 ms: GPIO, debounce theo thời gian đủ 30 ms, auto-repeat 300/100 ms; giữ pending press khi Queue đầy, bỏ repeat có thống kê. Bộ đệm pending hữu hạn một press mỗi nút.
- GameTask: engine + FSM + PWM; ngủ tối đa 20 ms và thức sớm theo deadline rơi. Chỉ Task này sửa dữ liệu game.
- DisplayTask 160 ms: lấy token, sao chép snapshot trong critical section ngắn, render rồi truyền I2C; lỗi bus có clear tối đa 9 xung và retry sau 500 ms. Chưa đo timing bus trên phần cứng.
- Hai Queue giữ `16 × uint16_t` mã nút và `1 × uint16_t` token. Không truyền snapshot qua Queue. Idle/Timer service là Task nội bộ FreeRTOS.

UART debug USART1 PA9/PA10 115200 8N1 đã được thêm; xem [nối USB–UART và đọc log](docs/UART_DEBUG.md). Không thêm Task/Queue.

Xem [module và comment](docs/MODULES.md), [kiểm thử board](docs/BOARD_TEST.md), [kết quả kiểm chứng](docs/test-results/SYSTEM_V1.md). Không tự merge vào develop/main/release; integration là ứng viên để nhóm review.

## Kiểm tra bổ sung trên Linux

```sh
python3 tests/check_project.py
./tests/run_host.sh
python3 tools/build_gcc.py  # cần arm-none-eabi-gcc/newlib trong PATH
```

GCC chỉ là build kiểm chứng bổ sung: dùng startup/linker/port GCC riêng trong `tools/`, không thay port và startup của Keil. Các sản phẩm build nằm trong `build/` và không commit. Không dùng số RAM/Flash GCC thay cho kết quả Keil.
