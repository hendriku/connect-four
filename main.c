#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define X_MAX 7
#define Y_MAX 6
#define RED_PLAYER 'X'
#define BLUE_PLAYER 'O'
#define NO_PLAYER '-'
#define BOTTOM_BORDER '='

typedef struct temp_gamestate {
    bool redPlayer;
    char* board;
} gamestate;

void initNewGame(gamestate*);
void printState(gamestate*);
int getLowestY(int, gamestate*);
int gameEnded(gamestate*);
bool isOccupied(int, int, gamestate*);
bool checkDiagonally1(int, int, int, gamestate*);
bool checkDiagonally2(int, int, int, gamestate*);
bool checkHorizontally(int, int, int, gamestate*);
bool checkVertically(int, int, int, gamestate*);
char getToken(int, int, gamestate*);
void setToken(char, int, int, gamestate*);
bool moveIsValid(int, int, gamestate*);
int applyMove(int, gamestate*);
int validMove(gamestate*);
int redPlayerLogic(gamestate*);
int bluePlayerLogic(gamestate*);

int main() {
    gamestate* state = malloc(sizeof(gamestate));

    initNewGame(state);

    int gameResult, answer;
    do {
        answer = state->redPlayer ? redPlayerLogic(state) : bluePlayerLogic(state);
        gameResult = applyMove(answer, state);
        printState(state);
    } while(!gameResult);

    switch(gameResult) {
        case 1:
        case -1:
            printf("Player %s has won!\n", gameResult == 1 ? "RED" : "BLUE");
            break;
        case -2:
            printf("Move x=%d from player %s is invalid!\n", answer, state->redPlayer ? "RED" : "BLUE");
            break;
        default:
            break;
    }

    return gameResult;
}

void initNewGame(gamestate* state) {
    // Allocate memory for player space and one bottom bar
    state->board = malloc(X_MAX * (Y_MAX+1) * sizeof(char));
    for(int y = 0; y < Y_MAX; y++) {
        for(int x = 0; x < X_MAX; x++) {
            state->board[X_MAX * y + x] = NO_PLAYER;
        }
    }
    for(int i = 0; i < X_MAX; i++) {
        state->board[X_MAX * Y_MAX + i] = BOTTOM_BORDER;
    }
    state->redPlayer = true;
}

void printState(gamestate* state) {
    system("clear");
    for(int y = 0; y <= Y_MAX; y++) {
        for(int x = 0; x < X_MAX; x++) {
            printf("%c ", state->board[X_MAX * y + x]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Player %s has the turn\n", state->redPlayer ? "RED" : "BLUE");
}

int getLowestY(int x, gamestate* state) {
    for(int y = 0; y <= Y_MAX; y++) {
        if(state->board[X_MAX * y + x] != NO_PLAYER) {
            return y-1;
        }
    }
    return -1;
}

int gameEnded(gamestate* state) {
    for(int y = 0; y < Y_MAX; y++) {
        for(int x = 0; x < X_MAX; x++) {
            if(checkVertically(x, y, 4, state) || checkHorizontally(x, y, 4, state)
               || checkDiagonally1(x, y, 4, state) || checkDiagonally2(x, y, 4, state)) {
                printf("GAME ENDED!\n");
                return true;
            }
        }
    }
    return false;
}

bool isOccupied(int x, int y, gamestate* state) {
    char token = getToken(x, y, state);
    return token == RED_PLAYER || token == BLUE_PLAYER;
}

//for checking nrOfTokens (win situation: nrOfTokens = 4)
bool checkDiagonally1(int col, int row, int nrOfTokens, gamestate* state) {
    for (int j = 0; j < nrOfTokens; j++) {
        int adjacentSameTokens = 0;
        for (int i = 0; i < nrOfTokens; i++) {
            if ((col + i - j) >= 0 && (col + i - j) < X_MAX
                && (row + i - j) >= 1 && (row + i - j) < Y_MAX
                && isOccupied(col, row, state) && getToken(col + i - j, row + i - j, state) == getToken(col, row, state))
            {
                adjacentSameTokens++;
            }
        }
        if (adjacentSameTokens >= nrOfTokens) return true;
    }
    return false;
}

bool checkDiagonally2(int col, int row, int nrOfTokens, gamestate* state)  {
    for (int j = 0; j < nrOfTokens; j++) {
        int adjacentSameTokens = 0;
        for (int i = 0; i < nrOfTokens; i++) {
            if ((col - i + j) >= 0 && (col - i + j) < X_MAX
                && (row + i - j) >= 1 && (row + i - j) < Y_MAX
                && isOccupied(col, row, state) && getToken(col - i + j, row + i - j, state) == getToken(col, row, state))
            {
                adjacentSameTokens++;
            }
        }
        if (adjacentSameTokens >= nrOfTokens) return true;
    }
    return false;
}

bool checkHorizontally(int col, int row, int nrOfTokens, gamestate* state)  {
    int adjacentSameTokens = 1;
    int i = 1;
    while (col - i >= 0 && isOccupied(col, row, state) && getToken(col - i, row, state) == getToken(col, row, state)) {
        adjacentSameTokens++;
        i++;
    }
    i = 1;
    while (col + i < X_MAX && isOccupied(col, row, state) && getToken(col + i, row, state) == getToken(col, row, state)) {
        adjacentSameTokens++;
        i++;
    }
    return (adjacentSameTokens >= nrOfTokens);
}

bool checkVertically(int col, int row, int nrOfTokens, gamestate* state) {
    int adjacentSameTokens = 1;
    int i = 1;
    while (row + i < Y_MAX && isOccupied(col, row, state) && getToken(col, row + i, state) == getToken(col, row, state)) {
        adjacentSameTokens++;
        i++;
    }
    return (adjacentSameTokens >= nrOfTokens);
}

char getToken(int x, int y, gamestate* state) {
    return state->board[X_MAX * y + x];
}

void setToken(char value, int x, int y, gamestate* state) {
    state->board[X_MAX * y + x] = value;
}

bool moveIsValid(int x, int y, gamestate* state) {
    return x >= 0 && x < X_MAX && y >= 0 && y < Y_MAX && getToken(x, y, state) == NO_PLAYER;
}

int applyMove(int x, gamestate* state) {
    // Get the lowest y position for the move
    int y = getLowestY(x, state);
    // Validate the move
    if(y == -1) {
        return -2;
    }
    // Create entry in array
    setToken((char) (state->redPlayer ? RED_PLAYER : BLUE_PLAYER), x, y, state);
    // Change player
    state->redPlayer = !state->redPlayer;
    // Return if game ended
    return gameEnded(state);
}

int validMove(gamestate* state) {
    int tryY;
    for(int tryX = 0; tryX < X_MAX; tryX++) {
        tryY = getLowestY(tryX, state);
        if(moveIsValid(tryX, tryY, state)) {
            return tryX;
        }
    }
    return 0;
}

int redPlayerLogic(gamestate* state) {
    return validMove(state);
}

int bluePlayerLogic(gamestate* state) {
    return validMove(state);
}

