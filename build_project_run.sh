#!/bin/bash

gcc -o bin/Debug/TF  ./src/main.c \
-I./include/ \
./include/drivers/keyboard/keyboard.c -I./include/drivers/keyboard \
./include/drivers/graphic/graphic.c -I./include/drivers/graphic \
./include/game/player/player.c -I./include/game/player \
./include/game/enemies/enemies.c -I./include/game/enemies \
./include/game/game.c -I./include/game/ \
 -I./include/game/ \
./bin/Debug/libraylib.a -I./build/external/raylib-master/src \
-lm -fsanitize=address -fsanitize=leak -g&& cd bin/Debug && ./TF && cd ../.. 
