#ifndef TRON_H
#define TRON_H

#include <library.h>

#define CHARACTER_WIDTH 8
#define CHARACTER_HEIGHT 16
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define PIXEL 8

#define MAP_WIDTH 128
#define MAP_HEIGHT 96

#define BLACK 0x0
#define GRAY 0x808080
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define ORANGE 0xFFA500
#define CYAN 0x00FFFF
#define YELLOW 0xFDF9F6
#define WHITE 0xFFFFFF

#define PLAYER_1_INIT_X 64
#define PLAYER_1_INIT_Y 94

#define LEVEL_0 150
#define LEVEL_1 17
#define LEVEL_2 10
#define LEVEL_3 5

void playTron();

#endif