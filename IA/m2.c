#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // Pour la fonction sleep

#define PLAYER "Olivier"

// GLOBAL VARIABLES
int columns = 0;
int rows = 0;
int turn = 1;
int points1 = 0;
int points2 = 0;
char name1[20] = "NameForPlayer1var";
char name2[20] = "Machine";
int *discoveredCells = NULL;
char *machineMemory = NULL;
int machineReplays = 0; // Indique si la machine rejoue

// FUNCTIONS DECLARATIONS
void createGameString(char *gameString, int rows, int columns);
void printGame(int turn, char *name1, char *name2, int points1, int points2);
void printTable(char *gameString, char *player1Inputs, char *machineInputs, int rows, int columns);
void printTop(int columns);
void printBottom(int columns);
void resetInputs(int turn, char *player1Inputs, char *machineInputs);
int checkTurn(char *gameString, int input1, int input2, int turn);
char aleatoryChar();
int inputPlayer(int turn, int time, int input, char *player1Inputs, char *machineInputs);
int validateInput(int input, int rows, int columns, int isSecondChoice, int firstInput);
void markAsDiscovered(int input1, int input2);
void printFinalScore(char *name1, char *name2, int points1, int points2);
int machinePlay(char *gameString, int rows, int columns, int firstChoice);
void updateMachineMemory(char *gameString, int input, char symbol);

char aleatoryChar() {
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "!@#$%^&*()_+{}|:<>?-=[];',./";
    return alphanum[rand() % (sizeof(alphanum) - 1)];
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

void printTable(char *gameString, char *player1Inputs, char *machineInputs, int rows, int columns) {
    printTop(columns);
    for (int i = 0; i < rows; i++) {
        printf("│");
        for (int j = 0; j < columns; j++) {
            int cellIndex = i * columns + j;
            if (discoveredCells[cellIndex]) {
                printf("       │");
            } else if ((int)player1Inputs[cellIndex]) {
                printf("   \x1b[34m%c\x1b[0m   │", gameString[cellIndex]);
            } else if ((int)machineInputs[cellIndex]) {
                printf("   \x1b[34;5m%c\x1b[0m   │", gameString[cellIndex]);
            } else {
                if (cellIndex + 1 < 10) {
                    printf("   %d   │", cellIndex + 1);
		    }

	    	else {
			if(cellIndex == 99)
                		printf("   %d │", cellIndex + 1);
			else
                		printf("   %d  │", cellIndex + 1);
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
    if (turn == 2) {
        sleep(2); // Pause pour le tour de la machine
    }
}

void printGame(int turn, char *name1, char *name2, int points1, int points2) {
    printf("Joueur : %s\tMachine\n", name1);
    if (turn == 1) {
        printf("\t\tTour de : %s\n", name1);
    } else {
        if (machineReplays) {
            printf("\t\t\x1b[36mLa machine rejoue\x1b[0m\n");
        } else {
            printf("\t\t\x1b[36mLa machine joue\x1b[0m\n");
        }
    }
    printf("Points : %d\t\tPoints : %d\n\n", points1, points2);
}

int validateInput(int input, int rows, int columns, int isSecondChoice, int firstInput) { 
    int inputValidated = 0;
    int availableCells = 0;
    
    for (int i = 0; i < rows * columns; i++) {
        if (!discoveredCells[i]) {
            availableCells++;
        }
    }
    
    while (inputValidated == 0) {
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("\x1b[31mEntrée invalide. Veuillez entrer un nombre.\x1b[0m\n");
        } else if (input > rows * columns || input < 1 || discoveredCells[input - 1] || 
                  (isSecondChoice && input == firstInput)) {
            if (availableCells > 0) {
                if (isSecondChoice && input == firstInput) {
                    printf("\x1b[31mVous ne pouvez pas choisir la même case que votre premier choix!\x1b[0m\n");
                } else {
                    printf("\x1b[31mVeuillez entrer un nombre valide entre 1 et %d (case non découverte)\x1b[0m\n", rows * columns);
                }
            } else {
                return -1;
            }
        } else {
            inputValidated = 1;
        }
        
        if (!inputValidated) {
            printf("Entrez un numéro de case valide : ");
        }
    }
    return input;
}

int checkTurn(char *gameString, int input1, int input2, int turn) {
    if (gameString[input1 - 1] == gameString[input2 - 1]) {
        if (turn == 1) {
            points1 += 1;
        } else {
            points2 += 1;
            machineReplays = 1; // La machine rejoue
        }
        markAsDiscovered(input1, input2);
        return 1;
    }
    if (turn == 2) {
        machineReplays = 0; // La machine perd son tour
    }
    return 0;
}

void markAsDiscovered(int input1, int input2) {
    discoveredCells[input1 - 1] = 1;
    discoveredCells[input2 - 1] = 1;
}

void resetInputs(int turn, char *player1Inputs, char *machineInputs) {
    if (turn == 1) {
        memset(player1Inputs, 0, rows * columns);
    } else {
        memset(machineInputs, 0, rows * columns);
    }
}

int inputPlayer(int turn, int time, int input, char *player1Inputs, char *machineInputs) {
    if (turn == 1) {
        player1Inputs[input - 1] = 1;
    } else {
        machineInputs[input - 1] = 1;
    }
    return 0;
}

void updateMachineMemory(char *gameString, int input, char symbol) {
    machineMemory[input - 1] = symbol;
}

void printFinalScore(char *name1, char *name2, int points1, int points2) {
    printf("\n\x1b[33m=== Résultat Final ===\x1b[0m\n\n");
    printf("%s : %d points\n", name1, points1);
    printf("Machine : %d points\n\n", points2);
    
    if (points1 > points2) {
        printf("\x1b[32mFélicitations %s, vous avez gagné contre la machine !\x1b[0m\n", name1);
    } else if (points2 > points1) {
        printf("\x1b[31mDommage, la machine a gagné cette fois-ci.\x1b[0m\n");
    } else {
        printf("\x1b[33mIncroyable, c'est une égalité entre vous et la machine !\x1b[0m\n");
    }
}

int machinePlay(char *gameString, int rows, int columns, int firstChoice) {
    for (int i = 0; i < rows * columns; i++) {
        if (machineMemory[i] != '\0' && !discoveredCells[i] && (i + 1) != firstChoice) {
            for (int j = i + 1; j < rows * columns; j++) {
                if (machineMemory[j] == machineMemory[i] && !discoveredCells[j] && (j + 1) != firstChoice) {
                    return j + 1;
                }
            }
        }
    }
    int availableCells[rows * columns];
    int count = 0;
    for (int i = 0; i < rows * columns; i++) {
        if (!discoveredCells[i] && (i + 1) != firstChoice) {
            availableCells[count++] = i;
        }
    }
    if (count > 0) {
        int randomIndex = rand() % count;
        return availableCells[randomIndex] + 1;
    }
    return -1;
}

int main() {
    srand(time(NULL));
    printf("\x1b[32mBienvenue à Char Memory contre la Machine !\x1b[0m\n\nVeuillez entrer le nombre de lignes et de colonnes que vous souhaitez jouer\n\n");
    while (rows * columns > 100 || rows * columns < 4 || rows * columns % 2 != 0) {
        printf("Entrez le nombre de lignes : ");
        scanf("%d", &rows);
        printf("Entrez le nombre de colonnes : ");
        scanf("%d", &columns);
        if (rows * columns > 100) {
            printf("\x1b[31mLe nombre de lignes multiplié par le nombre de colonnes ne peut pas dépasser 100\x1b[0m\n");
        }
        if (rows * columns < 4) {
            printf("\x1b[31mLe nombre de lignes multiplié par le nombre de colonnes ne peut pas être inférieur à 4\x1b[0m\n");
        }
        if (rows * columns % 2 != 0) {
            printf("\x1b[31mLe nombre de lignes multiplié par le nombre de colonnes doit être pair\x1b[0m\n");
        }
    }
    system("clear");
    strcpy(name1, PLAYER);
    printf("\x1b[32mC'est parti !\x1b[0m\n\n");
    system("sleep 1.8 && clear");
    system("clear");

    char gameString[rows * columns];
    char player1Inputs[rows * columns];
    char machineInputs[rows * columns];

    createGameString(gameString, rows, columns);
    discoveredCells = calloc(rows * columns, sizeof(int));
    machineMemory = calloc(rows * columns, sizeof(char));
    memset(player1Inputs, 0, sizeof(player1Inputs));
    memset(machineInputs, 0, sizeof(machineInputs));

    while (points1 + points2 < rows * columns / 2) {
        printGame(turn, name1, name2, points1, points2);
        printTable(gameString, player1Inputs, machineInputs, rows, columns);
        int input1 = 0;
        int input2 = 0;

        if (turn == 1) {
            printf("\n%s,\n\n", name1);
            printf("Choisissez la première case : ");
            input1 = validateInput(input1, rows, columns, 0, -1);
            if (input1 == -1) break;
            inputPlayer(turn, 0, input1, player1Inputs, machineInputs);
            updateMachineMemory(gameString, input1, gameString[input1 - 1]);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, machineInputs, rows, columns);
            printf("\n%s,\n\n", name1);
            printf("Choisissez la deuxième case : ");
            input2 = validateInput(input2, rows, columns, 1, input1);
            if (input2 == -1) break;
            inputPlayer(turn, 0, input2, player1Inputs, machineInputs);
            updateMachineMemory(gameString, input2, gameString[input2 - 1]);
        } else {
            printf("\nLa machine joue...\n");
            system("sleep 1");
            input1 = machinePlay(gameString, rows, columns, -1);
            if (input1 == -1) break;
            inputPlayer(turn, 0, input1, player1Inputs, machineInputs);
            updateMachineMemory(gameString, input1, gameString[input1 - 1]);
            system("clear");
            printGame(turn, name1, name2, points1, points2);
            printTable(gameString, player1Inputs, machineInputs, rows, columns);
            printf("\nLa machine joue...\n");
            system("sleep 1");
            input2 = machinePlay(gameString, rows, columns, input1);
            if (input2 == -1) break;
            inputPlayer(turn, 0, input2, player1Inputs, machineInputs);
            updateMachineMemory(gameString, input2, gameString[input2 - 1]);
        }

        system("clear");
        printGame(turn, name1, name2, points1, points2);
        printTable(gameString, player1Inputs, machineInputs, rows, columns);
        if (checkTurn(gameString, input1, input2, turn) == 1) {
            printf("\x1b[32mBien joué, une paire a été trouvée !\x1b[0m\n");
            system("sleep 1.2");
        } else {
            printf("\x1b[31mPas de paire cette fois-ci.\x1b[0m\n");
            system("sleep 1.2");
            turn = (turn == 1) ? 2 : 1;
        }
        resetInputs(1, player1Inputs, machineInputs);
        resetInputs(2, player1Inputs, machineInputs);
        system("clear");
    }

    printTable(gameString, player1Inputs, machineInputs, rows, columns);
    printFinalScore(name1, name2, points1, points2);

    free(discoveredCells);
    free(machineMemory);
    return 0;
}
