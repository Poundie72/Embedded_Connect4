#include "Game_Driver.h"

static STMPE811_TouchData StaticTouchData;
static uint8_t GAME_MODE = NONE_SELECTED;
static uint8_t board[NUM_COLS][NUM_ROWS] = {0};
static uint8_t currentSlot = 3;
static bool gameInProgress = true;
static bool competitionInProgress = false;
static bool resetGameFlag = false;
static uint8_t winner = PLAYERNULL;
static uint8_t PLAYER1_WINS = 0;
static uint8_t PLAYER2_WINS = 0;
static uint32_t startTime = 0;
static uint32_t endTime = 0;
static bool tieGame = false;

static uint8_t currentPlayer = PLAYER1;

void resetGame(){
	currentPlayer = PLAYER1;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			board[j][i] = PLAYERNULL;
		}
	}
	gameInProgress = true;
	resetGameFlag = false;
	winner = PLAYERNULL;
	board[6][4] = PLAYERNULL;
	board[6][0] = PLAYERNULL;
	startTime = 0;
	endTime = 0;
	tieGame = false;
}

void screen1(){
	//DONT REMOVE, THIS IS BLACK MAGIC
	board[5][4] = PLAYERNULL;
	board[5][0] = PLAYERNULL;
	for (int i = 0; i < NUM_ROWS; i++) {
			for (int j = 0; j < NUM_COLS; j++) {
				board[i][j] = PLAYERNULL;
			}
		}
	// ????????????????
	LCD_Clear(0, LCD_COLOR_BLACK);
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_SetFont(&Font16x24);




	//draw screen 1 display
	LCD_Draw_Box(30, 20, 180, 80, LCD_COLOR_GREEN);
		LCD_DisplayChar(45, 50, 'O');
		LCD_DisplayChar(65, 50, 'n');
		LCD_DisplayChar(80, 50, 'e');
		LCD_DisplayChar(95, 50, '-');
		LCD_DisplayChar(110, 50, 'P');
		LCD_DisplayChar(125, 50, 'l');
		LCD_DisplayChar(140, 50, 'a');
		LCD_DisplayChar(155, 50, 'y');
		LCD_DisplayChar(170, 50, 'e');
		LCD_DisplayChar(185, 50, 'r');
	LCD_Draw_Box(30, 120, 180, 80, LCD_COLOR_CYAN);
		LCD_DisplayChar(45, 150, 'T');
		LCD_DisplayChar(65, 150, 'w');
		LCD_DisplayChar(80, 150, 'o');
		LCD_DisplayChar(95, 150, '-');
		LCD_DisplayChar(110, 150, 'P');
		LCD_DisplayChar(125, 150, 'l');
		LCD_DisplayChar(140, 150, 'a');
		LCD_DisplayChar(155, 150, 'y');
		LCD_DisplayChar(170, 150, 'e');
		LCD_DisplayChar(185, 150, 'r');

		LCD_DisplayChar(30, 230, 'C');
		LCD_DisplayChar(50, 230, 'o');
		LCD_DisplayChar(70, 230, 'n');
		LCD_DisplayChar(90, 230, 'n');
		LCD_DisplayChar(110, 230, 'e');
		LCD_DisplayChar(130, 230, 'c');
		LCD_DisplayChar(150, 230, 't');
		LCD_DisplayChar(170, 230, '-');
		LCD_DisplayChar(190, 230, '4');

	//Game mode select
	while(GAME_MODE == NONE_SELECTED){

		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed){
			if (StaticTouchData.x > ONE_PLAYER_TOP_LEFT_X && StaticTouchData.x < ONE_PLAYER_BOTTOM_RIGHT_X
			 && StaticTouchData.y > ONE_PLAYER_TOP_LEFT_Y && StaticTouchData.y < ONE_PLAYER_BOTTOM_RIGHT_Y){
				GAME_MODE = ONE_PLAYER;
				competitionInProgress = true;

			}
			else if(StaticTouchData.x > TWO_PLAYER_TOP_LEFT_X && StaticTouchData.x < TWO_PLAYER_BOTTOM_RIGHT_X
				 && StaticTouchData.y > TWO_PLAYER_TOP_LEFT_Y && StaticTouchData.y < TWO_PLAYER_BOTTOM_RIGHT_Y){
				GAME_MODE = TWO_PLAYER;
				competitionInProgress = true;

			}
		}
	}

	if(GAME_MODE == ONE_PLAYER){
		LCD_DisplayChar(0, 0, '1');
	}
	else if(GAME_MODE == TWO_PLAYER){
		LCD_DisplayChar(0, 0, '2');
	}


	return;

}

void screen2(){
	LCD_Clear(0, LCD_COLOR_GREY);
	drawBoard();
	startTime = HAL_GetTick();

	while(gameInProgress){
		playTurn();
		
		if(!gameInProgress){
			endTime = HAL_GetTick();
			return;
		}
	}
}

void screen3(){
	char buffer[11];

	uint32_t totalTimeinTicks = endTime - startTime;
	uint32_t totalTimeinSeconds = totalTimeinTicks / 1000;
	LCD_Clear(0, LCD_COLOR_BLACK);

	if(winner == PLAYER1){
		PLAYER1_WINS++;
		LCD_Clear(0, PLAYER1_COLOR);
	}
	if(winner == PLAYER2){
		PLAYER2_WINS++;
		LCD_Clear(0, PLAYER2_COLOR);	
	}

	if(winner == PLAYER1) LCD_Display_String(20,100, "PLAYER 1 WINS!");
	if(winner == PLAYER2) LCD_Display_String(20,100, "PLAYER 2 WINS!");
	if(tieGame) LCD_Display_String(20,100, "TIE GAME!");

	HAL_Delay(1000);

	LCD_Clear(0, LCD_COLOR_BLACK);
	//display score and reset button

	LCD_Display_String(90, 30, "SCORE");
	LCD_Draw_Circle_Fill(30,110, COIN_SIZE,  PLAYER1_COLOR);
	LCD_Draw_Circle_Fill(30,150, COIN_SIZE,  PLAYER2_COLOR);
	LCD_Display_String(50, 100, "PLAYER1: ");
	LCD_Display_String(50, 140, "PLAYER2: ");

	LCD_Display_String(30, 260, "Match Length");
	LCD_Display_String(70, 290, "SECONDS");

	sprintf(buffer, "%lu", totalTimeinSeconds);
	LCD_Display_String(30, 290, buffer);


	

	sprintf(buffer, "%d", PLAYER1_WINS);
    LCD_Display_String(200, 100, buffer);
	
	sprintf(buffer, "%d", PLAYER2_WINS);
    LCD_Display_String(200, 140, buffer);

	LCD_Draw_Box(30, 200, 180, 60, LCD_COLOR_GREY);
	LCD_Display_String(45, 220, "PLAY AGAIN");


	while(1){

		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed){
			if (StaticTouchData.x > 30 && StaticTouchData.x < 210
			 && StaticTouchData.y > 200 && StaticTouchData.y < 260){
				resetGameFlag = true;
				return;
			}

		}
	}	
}

void game(){
	if(resetGameFlag == true) resetGame();

	if(!competitionInProgress) screen1();

	board[0][6] = PLAYERNULL;

	screen2();

	screen3();
}

void drawBoard(){
	LCD_Clear(0, LCD_COLOR_BLACK);

	//board[6][4] = PLAYERNULL;
	LCD_Draw_Vertical_Line(COL0_X, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(COL1_X, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(COL2_X, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(COL3_X, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(COL4_X, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(COL5_X, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(COL6_X, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(COL7_X, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_WHITE);


	for(int i = 0; i < NUM_COLS; i++){
		for(int j = 0; j < NUM_ROWS; j++){
			if(board[i][j] == PLAYER1) placeCoin(i,j, PLAYER1);
			else if(board[i][j] == PLAYER2) placeCoin(i,j, PLAYER2);
			//else placeCoin(i,j,PLAYERNULL);
		}
	}
}

void placeCoin(uint8_t col, uint8_t row, uint8_t player){
	uint16_t color = LCD_COLOR_WHITE;
	if(player == PLAYER1) color = LCD_COLOR_GREEN;
	if(player == PLAYER2) color = LCD_COLOR_CYAN;
	if(player == PLAYERNULL) color = LCD_COLOR_BLACK;
	LCD_Draw_Circle_Fill(COLx_X(col), ROWx_Y(row) - row * 5, COIN_SIZE, color);
}



bool processTouchToMove() {
    if (GAME_MODE == ONE_PLAYER && currentPlayer == PLAYER2) return false;

    if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
        if (StaticTouchData.x < SCREEN_MIDDLE_X && currentSlot < 6) {
            placeCoin(currentSlot, 8, PLAYERNULL);
            currentSlot++;
        } else if (StaticTouchData.x > SCREEN_MIDDLE_X && currentSlot > 0) {
            placeCoin(currentSlot, 8, PLAYERNULL);
            currentSlot--;
        }
        placeCoin(currentSlot, 8, currentPlayer);
        HAL_Delay(200);
        return true;  // action taken
    }

    return false;  // no input
}


void dropCoin() {
    for (int i = 0; i < 6; i++) {
        if (board[currentSlot][i] != PLAYER1 && board[currentSlot][i] != PLAYER2) {
            board[currentSlot][i] = currentPlayer;
            drawBoard();

			uint8_t maxCount = evaluateAlignment(currentPlayer);
            if (maxCount >= 4) {
				winner = currentPlayer;
                gameInProgress = false;
				LCD_Display_String(30, 30, "GAME OVER");
				
				
                return;
            }

			bool tieGame = isBoardFull();
			if(tieGame == true){
				gameInProgress = false;
				tieGame = true;
				return;
			}

            // Update state of game
            if (currentPlayer == PLAYER1) currentPlayer = PLAYER2;
            else if (currentPlayer == PLAYER2) currentPlayer = PLAYER1;

            currentSlot = 3;
            placeCoin(currentSlot, 8, currentPlayer);
            return;
        }
    }
    return;
}

uint8_t evaluateAlignment(uint8_t player) {
    int directions[4][2] = {
        {1, 0},
        {0, 1},  
        {1, 1},
        {1, -1}
    };

    uint8_t maxInARow = 0;

    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (board[col][row] != player) continue;

            for (int d = 0; d < 4; d++) {
                int dx = directions[d][0];
                int dy = directions[d][1];
                int count = 1;

                for (int step = 1; step < 4; step++) {
                    int cx = col + dx * step;
                    int cy = row + dy * step;

                    if (cx < 0 || cx >= NUM_COLS || cy < 0 || cy >= NUM_ROWS) break;
                    if (board[cx][cy] == player) count++;
                    else break;
                }

                if (count > maxInARow) {
                    maxInARow = count;
                }
            }
        }
    }

    return maxInARow;  // 4 = win, 3 = good, etc.
}


void playTurn() {
    if (GAME_MODE == TWO_PLAYER) {
        humanTurn();
    } else if (GAME_MODE == ONE_PLAYER) {
        if (currentPlayer == PLAYER1) {
            humanTurn();
        } else if (currentPlayer == PLAYER2) {
            AITurn();
        }
    }
}

void humanTurn(){
	processTouchToMove();
	return;
}

void AITurn() {
    HAL_Delay(200);
    currentSlot = decideAISlot();
    dropCoin();
	HAL_Delay(500);
    return;
}

uint32_t evaluateMove(uint8_t col, uint8_t player) {
    int row = findLowestEmptyRow(col);
    if (row == -1) return 0;

	uint8_t opponent;
	if (player == PLAYER1) {
		opponent = PLAYER2;
	} else {
		opponent = PLAYER1;
	}
    uint32_t score = 0;

    board[col][row] = player;

    // Immediate win
    if (evaluateAlignment(player) >= 4) {
        board[col][row] = PLAYERNULL;
        return 10000;
    }

    int align = evaluateAlignment(player);
    if (align == 3) score += 500;
    else if (align == 2) score += 150;

    int centerBias[NUM_COLS] = {3, 4, 5, 4, 5, 4, 3};
    score += centerBias[col] * 10;

    score += (NUM_ROWS - row) * 3;

    int worstThreat = 0;
    for (int c2 = 0; c2 < NUM_COLS; c2++) {
        int r2 = findLowestEmptyRow(c2);
        if (r2 == -1) continue;

        board[c2][r2] = opponent;

        int oppAlign = evaluateAlignment(opponent);
        int oppScore = 0;
        if (oppAlign >= 4) {
            oppScore = 10000; // Opponent can win
        } else if (oppAlign == 3) {
            oppScore = 500;

            // 🔥 Check if opponent has *another* open 3 threat
            for (int c3 = 0; c3 < NUM_COLS; c3++) {
                if (c3 == c2) continue;
                int r3 = findLowestEmptyRow(c3);
                if (r3 == -1) continue;
                board[c3][r3] = opponent;

                int secondAlign = evaluateAlignment(opponent);
                if (secondAlign == 3) {
                    oppScore += 1500; // Fork penalty
                }

                board[c3][r3] = PLAYERNULL;
            }
        } else if (oppAlign == 2) {
            oppScore = 150;
        }

        if (oppScore > worstThreat) worstThreat = oppScore;

        board[c2][r2] = PLAYERNULL;
    }

    board[col][row] = PLAYERNULL;

    return score - worstThreat;
}

uint8_t decideAISlot() {
    int bestScore = -1000000;
    uint8_t bestColumn = 3; // fallback to center

    for (uint8_t col = 0; col < NUM_COLS; col++) {
        int score = evaluateMove(col, PLAYER2);
        if (score > bestScore) {
            bestScore = score;
            bestColumn = col;
        }
    }

    return bestColumn;
}


uint8_t findLowestEmptyRow(int col) {
	for (int row = 0; row < NUM_ROWS; row++) {
		if (board[col][row] == PLAYERNULL) {
			return row;
		}
	}
	return -1; // No empty row found
}

bool isBoardFull() {
    for (int col = 0; col < NUM_COLS; col++) {
        for (int row = 0; row < NUM_ROWS; row++) {
            if (board[col][row] == PLAYERNULL) {
                return false; // Found an empty slot
            }
        }
    }
    return true; // No empty slots found
}

