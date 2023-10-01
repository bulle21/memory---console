#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// GLOBAL VARIABLES
int columns = 0;
int rows = 0;
int turn = 1;
int points1 = 0;
int points2 = 0;
char name1[20] = "NameForPlayer1var";
char name2[20] = "NameForPlayer2var";
char gameString[] = "structGameString";
char player1Inputs[] = "\0";
char player2Inputs[] = "\0";

// FUNCTIONS
void createGameString(char *gameString, int rows, int columns);
void printGame(int turn, char *name1, char *name2, int points1, int points2);
void printTable(char *gameString, char *player1Inputs, char *player2Inputs, int rows, int columns);
void printTop(int columns);
void printBottom(int columns);
void resetInputs(int turn);
int checkTurn(int input1, int input2, int turn); // Boolean, return 1 if input 1 and 2 match and 0 if they don't match return 0
int aleatoryChar();
int inputPlayer(int turn, int time, int input);

int aleatoryChar(){
    srand((unsigned int)clock());
    int aleatoryNumber = rand() % (126 - 33) + 34; // Genera un número aleatorio entre 33 y 126
    return aleatoryNumber;
}

void createGameString(char *gameString, int rows, int columns) {
    char lastChar = '\0';
    char backUp[rows * columns + 1];
    for (int i = 0; i < rows * columns; i += 2) {
        char aleatoryCharCreated = aleatoryChar();
        while (aleatoryCharCreated == lastChar) {
            aleatoryCharCreated = aleatoryChar();
        }
        lastChar = aleatoryCharCreated;
        backUp[i] = lastChar;
        backUp[i + 1] = lastChar;
    }

    for (int i = 0; i <= rows * columns; i++) {
        gameString[i] = '\0';
    }

    for (int i = 0; i < rows * columns; i++) {
        int aleatoryPosition = rand() % (rows * columns);
        while (gameString[aleatoryPosition] != '\0') {
            aleatoryPosition = rand() % (rows * columns);
        }
        gameString[aleatoryPosition] = backUp[i];
    }
}

int checkTurn(int input1, int input2, int turn){
    if (turn == 1){
        if (gameString[input1 - 1] == gameString[input2 - 1]) {
            points1 = points1 + 1;
            return 1;
        } else {
            points1 = 0;
            return 0;
        }
    } else {
        if (gameString[input1 - 1] == gameString[input2 - 1]) {
            points2 = points2 + 1;
            return 1;
        } else {
            points2 = 0;
            return 0;
        }
    }
}//Return 1 if input 1 and 2 match and 0 if they don't match

void printTable(char *gameString, char *player1Inputs, char *player2Inputs, int rows, int columns){
    printTop(columns);
    for (int i = 0; i < rows; i++) {
        printf("│");
        for (int j = 0; j < columns; j++) {
            int hasInput = 0;
            for (int k = 0; k < rows * columns; k++) {
                if (i * columns + j + 1 == (int)player1Inputs[k]) {
                    printf("   %c   │", gameString[i * columns + j]);
                    hasInput = 1;
                    k = rows * columns;
                }
                if (i * columns + j + 1 == (int)player2Inputs[k]) {
                    printf("   %c   │", gameString[i * columns + j]);
                    hasInput = 1;
                    k = rows * columns;
                }
            }
            if (hasInput == 0) {
                if (i * columns + j + 1 < 10) {
                    printf("   %d   │", i * columns + j + 1);
                } else {
                    printf("   %d  │", i * columns + j + 1);
                }
            }
        }
        printf("\n");
        if (i != rows - 1) {
            printf("├");
            for (int j = 0; j < columns; j++) {
                if (j == columns - 1) {
                    printf("───────┤\n");
                } else{
                    printf("───────┼");
                }
            }
        }
    }
    printBottom(columns);
}

void printTop(int columns){
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

int inputPlayer(int turn, int time, int input){
    if (turn == 1){
        char inputChar[2];
        sprintf(inputChar, "%c", input);
        strcat(player1Inputs, inputChar);
    } else {
        char inputChar[2];
        sprintf(inputChar, "%c", input);
        strcat(player2Inputs, inputChar);
    }
}

void resetInputs(int turn){
    if (turn == 1) {
        for (int i = 0; i <= sizeof(player1Inputs) + 1; i++) {
            player1Inputs[i] = '\0';
        }
    } else {
        for (int i = 0; i <= sizeof(player2Inputs) + 1; i++) {
            player2Inputs[i] = '\0';
        }
    }
}

void printGame(int turn, char *name1, char *name2, int points1, int points2){
    turn == 1 ? printf("Turno: %s\n", name1) : printf("Turno: %s\n", name2);
    printf("Jugador 1: %s           Jugador 2: %s\n", name1, name2);
    printf("Puntos: %d              Puntos: %d\n", points1, points2);
}


int main(){
    printf("Bienvenido al juego de memoria\n\nPor favor ingrese el número de filas y columnas que desea jugar\n");
    printf("Filas: ");
    scanf("%d", &rows);
    printf("Columnas: ");
    scanf("%d", &columns);
    system("clear");
    printf("Por favor ingrese el nombre del jugador 1: ");
    scanf("%s", name1);
    printf("Por favor ingrese el nombre del jugador 2: ");
    scanf("%s", name2);
    system("clear");
    createGameString(gameString, rows, columns);
    while (points1 < rows * columns / 2 && points2 < rows * columns / 2) {
        printGame(turn, name1, name2, points1, points2);
        printTable(gameString, player1Inputs, player2Inputs, rows, columns);
        int input1 = 0;
        int input2 = 0;
        int time = 0;
        if (turn == 1) {
            printf("Jugador 1: %s\n", name1);
            printf("Ingrese el primer número: ");
            scanf("%d", &input1);
            inputPlayer(turn, time, input1);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, player2Inputs, rows, columns);
            printf("Ingrese el segundo número: ");
            scanf("%d", &input2);
            inputPlayer(turn, time, input2);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, player2Inputs, rows, columns);
            if (checkTurn(input1, input2, turn) == 1) {
                printf("¡Bien hecho!\n");
                system("sleep 2");
            } else {
                printf("¡Fallaste!\n");
                system("sleep 2");
                resetInputs(turn);
                turn = 2;
            }
        } else {
            printf("Jugador 2: %s\n", name2);
            printf("Ingrese el primer número: ");
            scanf("%d", &input1);
            inputPlayer(turn, time, input1);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, player2Inputs, rows, columns);
            printf("Ingrese el segundo número: ");
            scanf("%d", &input2);
            inputPlayer(turn, time, input2);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, player2Inputs, rows, columns);
            if (checkTurn(input1, input2, turn) == 1) {
                printf("¡Bien hecho!\n");
                system("sleep 2");
            } else {
                printf("¡Fallaste!\n");
                system("sleep 2");
                resetInputs(turn);
                turn = 1;
            }
        }
        system("clear");
    }
}

/*
Solo se puede una combinación de fila y columna par
*/
