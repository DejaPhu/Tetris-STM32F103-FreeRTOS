# Giao diện tích hợp

Contract công khai: app/inc/app_shared.h; task_input_run, task_game_run, task_display_run, app_game_copy_snapshot.

- Queue handles được định nghĩa trong main.c; không tạo/định nghĩa lại.
- Queue input: uint16_t; low byte mã BTN_*, bit 8 repeat. Không gửi con trỏ hoặc struct vào Queue.
- Queue frame: uint16_t token 1; nếu đầy không cần thêm token vì snapshot luôn được cập nhật.
- Snapshot: board có overlay khối đang rơi, score/lines/level, next piece, menu index, state, difficulty. Display chỉ sao chép qua app_game_copy_snapshot; không đọc Engine_t.
- game_internal.h là private cho engine/FSM/GameTask. Không dùng trong Input/Display.
- HAL OLED/buzzer, renderer, filter và diagnostics có header riêng; xem docs/MODULES.md.

Không đổi chân, số Task hoặc kiểu Queue trong bản tích hợp. Quyền sở hữu nhánh Task giữ nguyên; bản tích hợp là ứng viên review, không tự merge.
