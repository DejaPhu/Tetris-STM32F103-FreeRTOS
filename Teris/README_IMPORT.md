# Nhập dự án Teris từ STM32CubeMX

File `Teris.ioc` được nhập từ `Teris(1).zip` do nhóm cung cấp. Nhánh này **chưa chứa toàn bộ mã nguồn Core/Drivers/Middlewares** từ ZIP và chưa được tích hợp vào `develop`.

## Cấu hình nhận diện
- STM32F103C8T6, 72 MHz, HSE 8 MHz / PLL x9
- I2C1 PB8/PB9 (remap OLED), TIM4 CH1 PB6 (buzzer)
- 6 nút nhấn: PB12, PB13, PB14, PB15, PA8, PB10
- USART1 PA9/PA10, SWD PA13/PA14, TIM2 HAL timebase
- CMSIS-RTOS v2, 3 Task và 2 Queue (cả hai Queue kiểu uint16_t)
- Toolchain trong ZIP hiện là **MDK-ARM V5.32**; nếu dùng STM32CubeIDE, Leader chuyển toolchain và generate lại trước khi push.

## Nhập toàn bộ mã nguồn (Leader)
1. Từ máy Leader, clone repo và checkout `feature/config-cubemx-oled`.
2. Giải nén bản dự án Teris đã lọc vào thư mục repo sao cho có `Core/`, `Drivers/`, `Middlewares/` và `Teris/Teris.ioc`. Tránh lồng thêm thư mục `Teris/Teris/`.
3. Chỉ Leader chỉnh `.ioc`. Sau khi đổi toolchain, generate code và build, commit các file cần thiết. Không commit `*.uvoptx`, `Debug/`, `Release/`, `build/`, file `*.elf`, `*.hex`.
4. Mở Pull Request vào `develop` để người khác review. Không tự merge vào `main`.

Các thành viên còn lại phát triển Task riêng trên nhánh `feature/task-*`. Không gộp các file Task khi chưa review.
