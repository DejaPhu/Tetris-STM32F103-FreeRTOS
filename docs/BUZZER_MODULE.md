# Buzzer ba chân kích LOW của người dùng

Người dùng xác nhận dùng module xanh ba chân trong ảnh (GND, VCC, I/O), kích mức LOW. Mã trước đây dùng PWM1 cực tính HIGH, CCR=0 làm PB6 xuống LOW khi im lặng, vô tình kích module liên tục.

## Nối dây

- GND → GND chung.
- I/O hoặc S → PB6.
- Với module định mức 3.3–5 V: VCC → 3.3 V để mức HIGH của STM32 phù hợp mức tắt module.
- Nếu module chỉ định mức 5 V, cần kiểm tra ngưỡng logic hoặc dùng mạch chuyển mức phù hợp; không đưa 5 V vào PB6. Mức HIGH 3.3 V có thể không tắt được một số module cấp 5 V.
- Trong reset/trước main(), PB6 còn ở trạng thái đầu vào. Nếu muốn giữ im lặng ngay từ lúc cấp nguồn, cần điện trở pull-up ngoài phù hợp, ví dụ 4.7 kΩ từ PB6 lên 3.3 V, và kiểm chứng với mạch module. Firmware không điều khiển được chân trước khi CPU chạy.

## Sửa phần mềm

- hal/inc/buzzer_config.h mặc định BUZZER_ACTIVE_LOW=1; main.c dùng TIM_OCPOLARITY_LOW.
- PWM1 + cực tính LOW + CCR=0 → PB6 HIGH khi im lặng. Khi phát âm CCR≈50%, sóng vẫn có xung LOW kích module, thời hạn do GameTask quản lý.
- main.c gọi buzzer_hold_idle ngay sau HAL_Init. BSP không nối PB6 vào AF trước khi timer chạy; buzzer_init khởi động PWM ở mức im lặng trước, sau đó chuyển GPIO sang TIM4_CH1.
- buzzer_hold_idle nạp mức GPIO trước khi đổi mode, không phụ thuộc HAL tick. app_fault gọi nó trước khi flush log/disable timer để không để chân kích LOW khi lỗi.
- Nếu dùng buzzer kích HIGH khác, cần chọn BUZZER_ACTIVE_LOW=0 và kiểm tra lại phần cứng. Không đổi tùy tiện CCR=ARR vì ARR thay theo tần số.
- Project Keil và .ioc đã lưu cực tính LOW. Nếu regenerate CubeMX ở thư mục tạm, Leader phải giữ buzzer_hold_idle và trình tự nối AF của BSP; không chép lại GPIO AF mặc định làm mất cách giữ im lặng.

## Giới hạn phần cứng

Kích LOW không tự xác định module là chủ động hay thụ động. Nếu là buzzer chủ động, nó tự tạo cao độ; PWM chỉ điều chế/bật tắt âm, các hiệu ứng không nhất thiết có cao độ 900/1500/2200 Hz như tính toán timer. Muốn điều khiển cao độ đúng cần buzzer thụ động và tầng lái phù hợp.

## Test trên board (chưa thực hiện)

1. Nạp đúng bản mới, Reset: ở WELCOME/menu không được kêu liên tục sau khi firmware đã khởi tạo. Dùng đồng hồ/logic analyzer đo PB6 idle gần 3.3 V.
2. Khi chơi, xoay gạch/xóa hàng/thắng/thua chỉ tạo hiệu ứng ngắn rồi tắt. Kiểm tra timer polarity LOW và CCR=0 khi im lặng.
3. Nếu PB6 đã HIGH mà vẫn kêu, kiểm tra dây VCC/GND/I/O, điện áp cấp module, ngưỡng kích và loại module. Không coi đây là lỗi đã được khắc phục trên board chỉ dựa vào host test.
4. Cần kiểm tra reset/cấp nguồn và fault path bằng dụng cụ; pull-up ngoài cần đánh giá trên module thực tế.

## Kiểm chứng phần mềm

Host test (ASan/UBSan) kiểm tra LOW polarity, GPIO idle, lỗi PWM_Start giữ GPIO im lặng, trình tự start-PWM trước AF, tắt sau deadline, wrap tick và giới hạn TIM4 16-bit. ARM cross-build được ghi riêng trong docs/test-results/BUZZER_ARM_BUILD.txt. Chưa build Keil hoặc đo âm/PB6 trên module thật.

ARM GCC 12.2.1 build/link sau sửa: PASS không warning/error; text=20972, data=144, bss=15512; Flash=21116/65536 byte, RAM=15656/20480 byte. Các test game và UART hiện có vẫn PASS; kiểm cấu trúc Keil vẫn đúng ba Tasks, hai Queues.
