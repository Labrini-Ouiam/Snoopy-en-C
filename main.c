// main.c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

struct Game {
    int playerRow;
    int playerCol;
    int ballRow;
    int ballCol;
    int ballDirectionRow;
    int ballDirectionCol;
    char **grid;
    int rows;
    int cols;
    int birdCount;
};

void clearScreen() {
    system("cls");
}

struct Game initializeGame() {
    struct Game game = {
        .playerRow = 1,
        .playerCol = 2,
        .ballRow = 5,
        .ballCol = 10,
        .ballDirectionRow = 1,
        .ballDirectionCol = 1,
        .birdCount = 0,
        .rows = 13,
        .cols = 27,
    };

    game.grid = malloc(game.rows * sizeof(char *));
    for (int i = 0; i < game.rows; i++) {
        game.grid[i] = malloc((game.cols + 1) * sizeof(char));

        for (int j = 0; j < game.cols; j++) {
            if (i == 0 || i == game.rows - 1 || j == 0 || j == game.cols - 1) {
                // Set borders in the grid
                game.grid[i][j] = '#';
            } else {
                game.grid[i][j] = ' ';
            }
        }

        // Null-terminate the string
        game.grid[i][game.cols] = '\0';
    }

    // Place birds in the grid
    game.grid[4][4] = 'B';
    game.grid[4][20] = 'B';
    game.grid[8][4] = 'B';
    game.grid[8][20] = 'B';

    return game;
}

void displayLevel(const struct Game *game, int gameOver, int win) {
    clearScreen();

    if (gameOver) {
        printf("Game Over! You touched the ball.\n");
    } else if (win) {
        printf("You Win! You ate all four birds.\n");
    } else {
        for (int i = 0; i < game->rows; i++) {
            for (int j = 0; j < game->cols; j++) {
                if (i == game->playerRow && j == game->playerCol) {
                    printf("S");
                } else if (i == game->ballRow && j == game->ballCol) {
                    printf("O");
                } else if (game->grid[i][j] == 'B') {
                    printf("B");
                } else {
                    printf("%c", game->grid[i][j]);
                }
            }
            printf("\n");
        }
        printf("Birds eaten: %d\n", game->birdCount);
    }
}

void updateGame(struct Game *game, int *gameOver, int *win) {
    // Move the ball diagonally
    game->ballRow += game->ballDirectionRow;
    game->ballCol += game->ballDirectionCol;

    // Bounce the ball inside the grid
    if (game->ballRow == 0 || game->ballRow == game->rows - 1) {
        game->ballDirectionRow *= -1;
        game->ballRow += game->ballDirectionRow;
    }

    if (game->ballCol == 0 || game->ballCol == game->cols - 1) {
        game->ballDirectionCol *= -1;
        game->ballCol += game->ballDirectionCol;
    }

    // Check if the player catches a bird
    if (game->grid[game->playerRow][game->playerCol] == 'B') {
        game->birdCount++;
        // Remove the bird
        game->grid[game->playerRow][game->playerCol] = ' ';
    }

    // Check if Snoopy touches the ball
    if (game->playerRow == game->ballRow && game->playerCol == game->ballCol) {
        *gameOver = 1;
    }

    // Check if the user wins
    if (game->birdCount == 4) {
        *win = 1;
    }
}

void handleInput(struct Game *game, char move) {
    switch (move) {
        case 'z':
            if (game->playerRow > 1) {
                game->playerRow--;
            }
            break;
        case 's':
            if (game->playerRow < game->rows - 2) {
                game->playerRow++;
            }
            break;
        case 'q':
            if (game->playerCol > 1) {
                game->playerCol--;
            }
            break;
        case 'd':
            if (game->playerCol < game->cols - 2) {
                game->playerCol++;
            }
            break;
    }
}

void cleanupGame(struct Game *game) {
    for (int i = 0; i < game->rows; i++) {
        free(game->grid[i]);
    }
    free(game->grid);
}

int main() {
    struct Game game = initializeGame();
    int gameOver = 0;
    int win = 0;

    while (!gameOver && !win) {
        if (_kbhit()) {
            char move = _getch();
            if (move == 'p') {
                break;
            }
            handleInput(&game, move);
        }

        updateGame(&game, &gameOver, &win);
        displayLevel(&game, gameOver, win);
        Sleep(50); // Adjust the sleep time to control the ball speed
    }

    cleanupGame(&game);

    return 0;
}