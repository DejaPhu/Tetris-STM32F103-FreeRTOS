#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build/host
cc -std=c99 -Wall -Wextra -Werror -Wno-misleading-indentation -fsanitize=address,undefined -g -Itests/host_stubs -Iapp/inc tests/test_game.c app/src/game_engine.c app/src/game_fsm.c app/src/button_filter.c app/src/ui_renderer.c -o build/host/test_game
build/host/test_game
