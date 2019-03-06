#!/bin/bash
g++ -g src/falcon9landing.cpp src/main.cpp -o build/falcon9landing `sdl2-config --libs --cflags` -lSDL2_image -lm -lSDL2_ttf