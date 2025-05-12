#!/bin/bash

gcc -o bin/Debug/TF  ./src/main.c \
-I./include/ \
./include/drivers/keyboard/keyboard.c -I./include/drivers/keyboard \
./include/drivers/graphic/graphic.c -I./include/drivers/graphic \
./bin/Debug/libraylib.a -I./build/external/raylib-master/src \
-lm && cd bin/Debug && ./TF && cd ../.. 
