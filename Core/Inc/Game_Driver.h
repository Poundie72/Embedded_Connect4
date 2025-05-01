#ifndef GAME_DRIVER_H_
#define GAME_DRIVER_H_

#include "LCD_Driver.h"
#include <stdio.h>

#define NONE_SELECTED 0
#define ONE_PLAYER 1
#define TWO_PLAYER 2

#define PLAYERNULL 0
#define PLAYER1 1
#define PLAYER2 2
#define PLAYER1_COLOR LCD_COLOR_GREEN
#define PLAYER2_COLOR LCD_COLOR_CYAN

#define NUM_ROWS 6
#define NUM_COLS 7

#define COIN_SIZE 10

#define LEFT 0
#define RIGHT 1


#define BOARD_HEIGHT ((COIN_SIZE * 2.5) * NUM_ROWS)
#define BOARD_TOP    320 - BOARD_HEIGHT - 10

#define SCREEN_MIDDLE_X 120

#define ONE_PLAYER_TOP_LEFT_X     30
#define ONE_PLAYER_TOP_LEFT_Y     20
#define ONE_PLAYER_BOTTOM_RIGHT_X 210
#define ONE_PLAYER_BOTTOM_RIGHT_Y 100

#define TWO_PLAYER_TOP_LEFT_X     30
#define TWO_PLAYER_TOP_LEFT_Y     120
#define TWO_PLAYER_BOTTOM_RIGHT_X 210
#define TWO_PLAYER_BOTTOM_RIGHT_Y 200

#define ROW_HEIGHT (COIN_SIZE + 10)
#define COL_WIDTH  (220/7)

#define ROW0_Y 310 - (ROW_HEIGHT * 0)
#define ROW1_Y 310 - (ROW_HEIGHT * 1)
#define ROW2_Y 310 - (ROW_HEIGHT * 2)
#define ROW3_Y 310 - (ROW_HEIGHT * 3)
#define ROW4_Y 310 - (ROW_HEIGHT * 4)
#define ROW5_Y 310 - (ROW_HEIGHT * 5)
#define ROWx_Y(n) ((300 - (ROW_HEIGHT * n)))

#define COL0_X 10 + (COL_WIDTH*0)
#define COL1_X 10 + (COL_WIDTH*1)
#define COL2_X 10 + (COL_WIDTH*2)
#define COL3_X 10 + (COL_WIDTH*3)
#define COL4_X 10 + (COL_WIDTH*4)
#define COL5_X 10 + (COL_WIDTH*5)
#define COL6_X 10 + (COL_WIDTH*6)
#define COL7_X 10 + (COL_WIDTH*7)
#define COLx_X(n) ((15 + (COL_WIDTH * (n))) + COIN_SIZE)

#define DRAW_X_OFFSET COIN_SIZE + 5







//240x320
void resetGame();

void screen1();

void screen2();

void screen3();

void drawCurrentState();

void game();

void placeCoin(uint8_t col, uint8_t row, uint8_t player);

uint8_t findLowestEmptyRow(int col);

void drawBoard();

void selectSlot();

bool processTouchToMove();

uint8_t evaluateGame();

uint8_t evaluateAlignment(uint8_t player);

void dropCoin();

void playTurn();

void humanTurn();

void AITurn();
uint8_t decideAISlot();
uint32_t evaluateMoveWithLookahead(uint8_t col, uint8_t player);


#endif
