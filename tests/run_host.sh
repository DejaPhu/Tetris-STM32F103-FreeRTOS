#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build/host
cc -std=c99 -Wall -Wextra -Werror -Wno-misleading-indentation -fsanitize=address,undefined -g -Itests/host_stubs -Iapp/inc tests/test_game.c app/src/game_engine.c app/src/game_fsm.c app/src/button_filter.c app/src/ui_renderer.c -o build/host/test_game
build/host/test_game
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -g -Itests/uart_stubs -Ihal/inc -Iapp/inc tests/test_uart.c hal/src/hal_uart.c app/src/app_log.c -o build/host/test_uart
build/host/test_uart
cc -std=c99 -Wall -Wextra -Werror -Wno-misleading-indentation -fsanitize=address,undefined -g -Itests/buzzer_stubs -Ihal/inc tests/test_buzzer.c hal/src/hal_buzzer.c -o build/host/test_buzzer
build/host/test_buzzer
