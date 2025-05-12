#!/bin/bash

gcc -o bin/Debug/TF  ./src/main.c \
-I./build/external/raylib-master/src \
./include/drivers/keyboard/keyboard.c -I./include/drivers/keyboard \
./include/drivers/graphic/graphic.c -I./include/drivers/graphic \
-I./include/ \
./bin/Debug/libraylib.a -lm && cd bin/Debug && ./TF && cd ../..
