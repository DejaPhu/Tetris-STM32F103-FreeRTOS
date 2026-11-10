# Cấu trúc repository theo QuyTrinh-Git-2.pdf

Tài liệu PDF là quy trình MCU tổng quát. Dự án áp dụng cách tách lớp/nhánh; các ví dụ task_sensor/xTaskCreate/vTaskStartScheduler không được chép nguyên vì CubeMX đã tạo ba Task và hai Queue CMSIS-RTOS v2. main.c vẫn là nơi duy nhất tạo chúng.

```text
Tetris-STM32F103-FreeRTOS/
├── bsp/
│   ├── inc/                   # main.h, board_pins.h, cấu hình HAL/FreeRTOS
│   └── src/                   # system clock, IRQ, MSP và HAL tick TIM2
├── hal/
│   ├── inc/                   # hal_gpio.h, hal_oled.h, hal_buzzer.h
│   ├── src/                   # driver nút, SSD1306 và PWM
│   └── vendor/STM32F1xx_HAL_Driver/ # thư viện ST nguyên bản
├── middlewares/
│   ├── CMSIS/                 # thư viện ARM/ST nguyên bản
│   └── FreeRTOS/Source/       # Kernel, CMSIS_RTOS_V2, Port, Heap
├── app/
│   ├── inc/                   # task_*.h, app_cfg.h, app_ipc.h, snapshot API
│   └── src/                   # đúng ba Task; engine/FSM/UI/filter/diagnostics
├── main.c                     # init HAL, tạo 2 Queue/3 Task, osKernelStart
├── Teris/
│   ├── Teris.ioc              # cấu hình CubeMX do Leader quản lý
│   └── MDK-ARM/               # Teris.uvprojx và startup ARMASM
├── docs/                      # kiến trúc, phân công, test và báo cáo
├── tests/                     # kiểm thử host, không đưa vào firmware
├── tools/                     # build ARM GCC kiểm chứng bổ sung
├── .gitignore
└── README.md
```

## API và quyền sở hữu

- app_cfg.h: chu kỳ, kích thước bàn, điểm thắng, stack và priority; main.c dùng các macro này. Stack/priority trong .ioc phải được Leader đồng bộ khi thay đổi.
- app_ipc.h: khai báo hai Queue handles; không tạo lại chúng trong app_ipc.c. main.c tạo Queue đúng một lần.
- task_input.h/task_game.h/task_display.h chỉ khai báo hàm run để wrapper sẵn có gọi, không tạo thêm thread.
- GPIO đọc reentrant; OLED do DisplayTask sở hữu độc quyền, game/PWM do GameTask sở hữu; không cần thêm mutex khi vẫn giữ quyền sở hữu này. Nếu thêm client I2C/PWM/UART dùng chung, cần thiết kế đồng bộ trước.
- main.h chứa pin CubeMX chính thức; board_pins.h là điểm truy cập của BSP/HAL. Không có bản sao main.c hoặc HAL driver để tránh liên kết trùng.

## CubeMX regeneration

File .ioc vẫn ở Teris/. Cấu trúc nguồn đã tách theo yêu cầu PDF, nên không regenerate đè trực tiếp checkout: CubeMX có thể tạo lại Core/Drivers/Middlewares tại vị trí cũ và project cũ. Leader generate trong thư mục tạm riêng, review diff rồi đưa các phần cấu hình cần thiết vào bsp/, hal/vendor/, middlewares/ và main.c; giữ USER CODE và đường dẫn Keil. Không commit cả bản cũ lẫn bản mới, không để mọi thành viên generate riêng.

## Nhánh và trạng thái

Feature → develop; sửa tích hợp ở integration/system-v1 → PR develop. Chỉ đưa vào release/main sau khi có kiểm thử kit. Lần merge này do người dùng yêu cầu đưa nguồn đã kiểm tra phần mềm vào develop; không đồng nghĩa đã nghiệm thu phần cứng và không gắn tag release.
