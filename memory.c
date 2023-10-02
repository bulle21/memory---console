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

// FUNCTIONS
void createGameString(char *gameString, int rows, int columns);
void printGame(int turn, char *name1, char *name2, int points1, int points2);
void printTable(char *gameString, char *player1Inputs, char *player2Inputs, int rows, int columns);
void printTop(int columns);
void printBottom(int columns);
void resetInputs(int turn, char *player1Inputs,  char *player2Inputs);
int checkTurn(char * gameString, int input1, int input2, int turn); // Boolean, return 1 if input 1 and 2 match and 0 if they don't match return 0
int aleatoryChar();
int inputPlayer(int turn, int time, int input, char *player1Inputs,  char *player2Inputs);
int validateInput(int input, int rows, int columns);

int aleatoryChar(){
    srand((unsigned int)clock());
    int aleatoryNumber = rand() % (126 - 33) + 34; // Genera un número aleatorio entre 33 y 126
    return aleatoryNumber;
}

int validateInput(int input, int rows, int columns){ 
    int inputValidated = 0;
    scanf("%d", &input);
    while (inputValidated == 0) {
        if (input > rows * columns || input < 1) {
            printf("\x1b[31mPor favor ingresa un número entre 1 y %d\x1b[0m\n", rows * columns);
            printf("Ingresa una casilla dentro del rango: ");
            scanf("%d", &input);
        } else {
            inputValidated = 1;
        }
    }
    return input;
}

void createGameString(char *gameString, int rows, int columns) {
    char lastChar = '\0';
    char backUp[rows * columns];
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

int checkTurn(char * gameString, int input1, int input2, int turn){
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
                if (i * columns + j + 1 == (int)player2Inputs[k]) {
                    printf("   %c   │", gameString[i * columns + j]);
                    hasInput = 1;
                    k = rows * columns;
                } 
                if (i * columns + j + 1 == (int)player1Inputs[k]) {
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

int inputPlayer(int turn, int time, int input, char *player1Inputs,  char *player2Inputs){
    if (turn == 1){
        player1Inputs[time] = input;
    } else {
        player2Inputs[time] = input;
    }
    return 0;
}

void resetInputs(int turn, char *player1Inputs,  char *player2Inputs){
    if (turn == 1) {
        for (int i = 0; i <= sizeof(player1Inputs) + 1; i++) {
            player1Inputs[i] = 0;
        }
    } else {
        for (int i = 0; i <= sizeof(player2Inputs) + 1; i++) {
            player2Inputs[i] = 0;
        }
    }
}

void printGame(int turn, char *name1, char *name2, int points1, int points2){
    printf("Jugador 1: %s\tJugador 2: %s", name1, name2);
    turn == 1 ? printf("\t\tTurno de: %s\n", name1) : printf("\t\t\x1b[36mTurno de: %s\x1b[0m\n", name2);
    printf("Puntos: %d\t\tPuntos: %d\n\n", points1, points2);
}

int main(){
    printf("\x1b[32mWelcome to Char Memory\x1b[0m\n\nPor favor, a continuación ingresa el número de filas y columnas que deseas jugar\n\n");
    while (rows * columns > 100 || rows * columns < 4 || rows * columns % 2 != 0) {
        printf("Ingresa el número de filas: ");
        scanf("%d", &rows);
        printf("Ingresa el número de columnas: ");
        scanf("%d", &columns);
        if (rows * columns > 100) {
            printf("\x1b[31mEl número de filas por columnas no puede ser mayor a 100\x1b[0m\n");
        }
        if (rows * columns < 4) {
            printf("\x1b[31mEl número de filas por columnas no puede ser menor a 4\x1b[0m\n");
        }
        if (rows * columns % 2 != 0) {
            printf("\x1b[31mEl número de filas por columnas debe ser par\x1b[0m\n");
        }
    }
    system("clear ");
    printf("\x1b[32m¡Excelente! Ahora ingresa los nombres de los jugadores\x1b[0m\n\n");
    system("sleep 1.8 && clear");
    printf("Por favor ingresa tu nombre, jugador 1: ");
    scanf("%s", name1);
    printf("Por favor ingresa tu nombre, jugador 2: ");
    scanf("%s", name2);
    system("clear");
    char gameString[rows * columns];
    createGameString(gameString, rows, columns);
    char player1Inputs[rows*columns];
    char player2Inputs[rows*columns];
    int timePlayer1 = 0;
    int timePlayer2 = 0;
    memset(player1Inputs, 0, sizeof(player1Inputs));
    memset(player2Inputs, 0, sizeof(player2Inputs));
    while (points1 < rows * columns / 2 && points2 < rows * columns / 2) {
        printGame(turn, name1, name2, points1, points2);
        printTable(gameString, player1Inputs, player2Inputs, rows, columns);
        int input1 = 0;
        int input2 = 0;
        if (turn == 1) {
            printf("\n%s,\n\n", name1);
            printf("Escoge la primer casilla: ");
            input1 = validateInput(input1, rows, columns);
            inputPlayer(turn, timePlayer1, input1, player1Inputs,  player2Inputs);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, player2Inputs, rows, columns);
            printf("\n%s,\n\n", name1);
            printf("Escoge la segunda casilla: ");
            input2 = validateInput(input2, rows, columns);
            inputPlayer(turn, timePlayer1 + 1, input2, player1Inputs, player2Inputs);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, player2Inputs, rows, columns);
            if (checkTurn(gameString, input1, input2, turn) == 1) {
                printf("\x1b[32m¡Bien hecho, acertaste!\x1b[0m\n");
                system("sleep 1.2");
                timePlayer1 += 2;
            } else {
                printf("\x1b[31m¡Fallaste, pero no te rindas!\x1b[0m\n");
                system("sleep 1.2");
                resetInputs(turn, player1Inputs,  player2Inputs);
                turn = 2;
                timePlayer1 = 0;
            }
        } else {
            printf("%s,\n\n", name2);
            printf("Escoge la primer casilla: ");
            input1 = validateInput(input1, rows, columns);
            inputPlayer(turn, timePlayer2, input1, player1Inputs,  player2Inputs);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, player2Inputs, rows, columns);
            printf("\n%s,\n\n", name2);
            printf("Escoge la segunda casilla: ");
            input2 = validateInput(input2, rows, columns);
            inputPlayer(turn, timePlayer2 + 1, input2, player1Inputs,  player2Inputs);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, player2Inputs, rows, columns);
            if (checkTurn(gameString, input1, input2, turn) == 1) {
                printf("\x1b[32m¡Bien hecho, acertaste!\x1b[0m\n");
                system("sleep 1.6");
                timePlayer2 += 2;
            } else {
                printf("\x1b[31m¡Fallaste, pero no te rindas!\x1b[0m\n");
                system("sleep 1.6");
                resetInputs(turn, player1Inputs,  player2Inputs);
                turn = 1;
                timePlayer2 = 0;
            }
        }
        system("clear");
    }
    if (points1 == rows * columns / 2) {
        printTable(gameString, player1Inputs, player2Inputs, rows, columns);
        printf("\x1b[31m¡Felicidades %s, ganaste!\x1b[0m\n", name1);
    } else {
        printTable(gameString, player1Inputs, player2Inputs, rows, columns);
        printf("\x1b[31m¡Felicidades %s, ganaste!\x1b[0m\n", name2);
    }

}

/*
Solo se puede una combinación de fila y columna par
*/
