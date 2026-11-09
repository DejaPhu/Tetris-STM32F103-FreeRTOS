# FSM dự kiến

States: WELCOME -> MAIN_MENU -> DIFFICULTY -> PLAYING.
PLAYING -> PAUSED -> PLAYING/MAIN_MENU.
PLAYING -> WIN (điểm >= WIN_SCORE), hoặc GAME_OVER (không sinh được gạch).
WIN/GAME_OVER -> RETRY hoặc MAIN_MENU.

Nút trong menu: ROTATE = lên, DOWN = xuống, DROP = OK, PAUSE = Back.
Nút trong game: LEFT/RIGHT = dịch, ROTATE = xoay, DOWN = soft drop, DROP = hard drop, PAUSE = tạm dừng.

Độ khó đề xuất: EASY 800 ms/ô; NORMAL 500 ms/ô; HARD 250 ms/ô.
Điểm YOU WIN dự kiến: 1000; có thể đổi khi nhóm chốt yêu cầu.

Chỉ GameTask xử lý chuyển state; DisplayTask chỉ hiển thị.
