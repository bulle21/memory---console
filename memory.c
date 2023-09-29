#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//GLOBAL VARIABLES
int columns = 0;
int rows = 0;
int turn = 1;
int points1 = 0;
int points2 = 0;
char name1[20] = "NameForPlayer1var";
char name2[20] = "NameForPlayer2var";
char gameString[] = "structGameString";

//FUNCTIONS
void createGameString(char *gameString, int rows, int columns);
// void printMenu();
// void printGame(char *gameString, int rows, int columns, int turn, char *name1, char *name2);
void printTable(char *gameString, int rows, int columns, int input1, int input2);
void printTop(int columns);
void printBottom(int columns);
int checkTurn(int input1, int input2, int turn);//Boolean, return 1 if input 1 and 2 match and 0 if they don't match return 0
int aleatoryChar();


int aleatoryChar() {
    srand((unsigned int)clock());
    int aleatoryNumber = rand() % (126 - 33) + 34; // Genera un número aleatorio entre 33 y 126
    return aleatoryNumber;
}

void createGameString(char *gameString, int rows, int columns) {
    char lastChar = '\0';
    char backUp[rows*columns + 1];
    for (int i = 0; i < rows*columns; i += 2) {
        char aleatoryCharCreated = aleatoryChar();
        while (aleatoryCharCreated == lastChar) {
            aleatoryCharCreated = aleatoryChar();
        }
        lastChar = aleatoryCharCreated;
        backUp[i] = lastChar;
        backUp[i+1] = lastChar;
    }

    for (int i = 0; i <= rows*columns; i++) {
        gameString[i] = '\0';
    }

    for (int i = 0; i < rows*columns; i++) {
        int aleatoryPosition = rand() % (rows*columns );
        while (gameString[aleatoryPosition] != '\0') {
            aleatoryPosition = rand() % (rows*columns );
        }
        gameString[aleatoryPosition] = backUp[i];
    }
    
}


int checkTurn(int input1, int input2, int turn) {
    if (gameString[input1 - 1] == gameString[input2 - 1]) {
        if (turn == 1) {
            points1++;
        } else {
            points2++;
        }
        return 1;
    } else {
        return 0;
    }
}

void printTable(char *gameString, int rows, int columns, int input1, int input2) {
    printTop(columns);
    for (int i = 0; i < rows; i++) {
        printf("│");
        for (int j = 0; j < columns; j++) {
            if (i*columns + j == input1 - 1 || i*columns + j == input2 - 1) {
                printf("   %c   │", gameString[i*columns + j]);
            } else {

            if (gameString[i*columns + j] == '\0') {
                printf("       │");
            } if (i*columns + j > 8) {
                printf("   %i  │", i*columns + j + 1);
            }else {
                printf("   %i   │", i*columns + j + 1);
            }
            }
        }
        printf("\n");
        if (i != rows - 1) {
            printf("├");
            for (int j = 0; j < columns; j++) {
                if (j == columns - 1) {
                    printf("───────┤\n");
                } else {
                    printf("───────┼");
                }
            }
        }
    }
    printBottom(columns);
}

void printTop(int columns) {
    for (int i = 0; i < columns; i++) {
        if (i == 0) {
            printf("┌");
        }
        if (i == columns - 1) {
            printf("───────┐\n");
        } else {
            printf("───────┬");
        }
    }
}

void printBottom(int columns) {
    for (int i = 0; i < columns; i++) {
        if (i == 0) {
            printf("└");
        }
        if (i == columns - 1) {
            printf("───────┘\n");
        } else {
            printf("───────┴");
        }
    }
}


int main () {
    createGameString(gameString, 4, 4);
    printf("%s\n", gameString);
    printTable(gameString, 4, 4, 1, 2);
    return 0;
}

/*
Solo se puede una combinación de fila y columna par
*/