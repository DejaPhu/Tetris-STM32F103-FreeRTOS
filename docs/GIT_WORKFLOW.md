# QUY TRÌNH GIT NHÓM 3 NGƯỜI

## Nhánh
- **main**: code chạy ổn định; cấm push/commit trực tiếp.
- **develop**: code tích hợp chung; chỉ PR đã review vào đây. Skeleton khởi tạo được đặt ở nhánh này.
- **feature/hal-***: driver; **feature/task-***: Task và logic; đều xuất phát từ develop.
- **integration/system-v1**: ghép module và test thực tế.
- **release**: nhánh đóng gói/chốt để nộp sau khi integration đạt yêu cầu; PR vào main và gắn tag.

## Nhánh phân công dự kiến
- TV1: feature/hal-gpio-uart, feature/task-input
- TV2: feature/hal-timer-buzzer, feature/task-game
- TV3: feature/hal-i2c-oled, feature/task-display
- Leader: feature/config-cubemx-oled (khởi tạo .ioc và dự án CubeIDE)

## Vòng đời code
1. Cập nhật develop rồi làm trên nhánh feature của mình.
2. Code + kiểm thử trên board/mock, commit nhỏ, push lên remote.
3. Mở PR vào develop; người khác review về API, race, timing, bộ nhớ.
4. Leader merge khi đạt yêu cầu; không ghi đè code thành viên khác.
5. Khi đủ driver và 3 Task, dùng integration/system-v1 để test tổng.
6. Sau khi ổn định, đưa vào release, review rồi merge main và gắn tag.

## Quy ước
- Chỉ Leader sửa .ioc và generate CubeMX.
- Không code toàn bộ trong main.c.
- Không commit Debug/, Release/, binary hoặc cấu hình máy cá nhân.
- Commit message: feat(...), fix(...), refactor(...), docs(...).
- Release branch không phải nơi phát triển tính năng mới.

## Lệnh tham khảo
```bash
git clone https://github.com/DejaPhu/Tetris-STM32F103-FreeRTOS.git
cd Tetris-STM32F103-FreeRTOS
git fetch origin
git switch develop
git pull origin develop
git switch feature/task-input
# Sau khi tự code và test:
git add app/
git commit -m "feat(task-input): implement debounced buttons"
git push -u origin feature/task-input
```
