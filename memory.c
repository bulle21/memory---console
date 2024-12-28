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
int *discoveredCells = NULL;

// FUNCTIONS
void createGameString(char *gameString, int rows, int columns);
void printGame(int turn, char *name1, char *name2, int points1, int points2);
void printTable(char *gameString, char *player1Inputs, char *player2Inputs, int rows, int columns);
void printTop(int columns);
void printBottom(int columns);
void resetInputs(int turn, char *player1Inputs,  char *player2Inputs);
int checkTurn(char * gameString, int input1, int input2, int turn);
char aleatoryChar();
int inputPlayer(int turn, int time, int input, char *player1Inputs,  char *player2Inputs);
int validateInput(int input, int rows, int columns);
void markAsDiscovered(int input1, int input2);
void printFinalScore(char *name1, char *name2, int points1, int points2);

char aleatoryChar(){
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "!@#$%^&*()_+{}|:<>?-=[];',./";
    return alphanum[rand() % (sizeof(alphanum) - 1)];
}

int validateInput(int input, int rows, int columns){ 
    int inputValidated = 0;
    int availableCells = 0;
    
    // Count available cells
    for (int i = 0; i < rows * columns; i++) {
        if (!discoveredCells[i]) {
            availableCells++;
        }
    }
    
    while (inputValidated == 0) {
        if (scanf("%d", &input) != 1) {
            // Clear input buffer
            while (getchar() != '\n');
            printf("\x1b[31mEntrée invalide. Veuillez entrer un nombre.\x1b[0m\n");
        } else if (input > rows * columns || input < 1 || discoveredCells[input - 1]) {
            if (availableCells > 0) {
                printf("\x1b[31mVeuillez entrer un nombre valide entre 1 et %d (case non découverte)\x1b[0m\n", rows * columns);
            } else {
                // No more valid cells, end the game
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
    if (gameString[input1 - 1] == gameString[input2 - 1]) {
        if (turn == 1) {
            points1 = points1 + 1;
        } else {
            points2 = points2 + 1;
        }
        markAsDiscovered(input1, input2);
        return 1;
    }
    return 0;
}

void markAsDiscovered(int input1, int input2) {
    discoveredCells[input1 - 1] = 1;
    discoveredCells[input2 - 1] = 1;
}

void printTable(char *gameString, char *player1Inputs, char *player2Inputs, int rows, int columns){
    printTop(columns);
    for (int i = 0; i < rows; i++) {
        printf("│");
        for (int j = 0; j < columns; j++) {
            int cellIndex = i * columns + j;
            if (discoveredCells[cellIndex]) {
                printf("       │");
            } else if ((int)player1Inputs[cellIndex] || (int)player2Inputs[cellIndex]) {
                printf("   %c   │", gameString[cellIndex]);
            } else {
                if (cellIndex + 1 < 10) {
                    printf("   %d   │", cellIndex + 1);
                } else {
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
        player1Inputs[input - 1] = 1;
    } else {
        player2Inputs[input - 1] = 1;
    }
    return 0;
}

void resetInputs(int turn, char *player1Inputs,  char *player2Inputs){
    if (turn == 1) {
        memset(player1Inputs, 0, rows * columns);
    } else {
        memset(player2Inputs, 0, rows * columns);
    }
}

void printGame(int turn, char *name1, char *name2, int points1, int points2){
    printf("Joueur 1 : %s\tJoueur 2 : %s", name1, name2);
    turn == 1 ? printf("\t\tTour de : %s\n", name1) : printf("\t\t\x1b[36mTour de : %s\x1b[0m\n", name2);
    printf("Points : %d\t\tPoints : %d\n\n", points1, points2);
}

void printFinalScore(char *name1, char *name2, int points1, int points2) {
    printf("\n\x1b[33m=== Résultat Final ===\x1b[0m\n\n");
    printf("%s : %d points\n", name1, points1);
    printf("%s : %d points\n\n", name2, points2);
    
    if (points1 > points2) {
        printf("\x1b[32mFélicitations %s, vous avez gagné !\x1b[0m\n", name1);
    } else if (points2 > points1) {
        printf("\x1b[32mFélicitations %s, vous avez gagné !\x1b[0m\n", name2);
    } else {
        printf("\x1b[33mFélicitations, c'est une égalité !\x1b[0m\n");
    }
}

int main(){
    srand(time(NULL));
    printf("\x1b[32mBienvenue à Char Memory\x1b[0m\n\nVeuillez entrer le nombre de lignes et de colonnes que vous souhaitez jouer\n\n");
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
    printf("\x1b[32mExcellent ! Maintenant, entrez les noms des joueurs\x1b[0m\n\n");
    system("sleep 1.8 && clear");
    printf("Veuillez entrer votre nom, joueur 1 : ");
    scanf("%s", name1);
    printf("Veuillez entrer votre nom, joueur 2 : ");
    scanf("%s", name2);
    system("clear");
    
    char gameString[rows * columns];
    createGameString(gameString, rows, columns);
    char player1Inputs[rows*columns];
    char player2Inputs[rows*columns];
    discoveredCells = calloc(rows * columns, sizeof(int));
    memset(player1Inputs, 0, sizeof(player1Inputs));
    memset(player2Inputs, 0, sizeof(player2Inputs));
    
    while (points1 + points2 < rows * columns / 2) {
        printGame(turn, name1, name2, points1, points2);
        printTable(gameString, player1Inputs, player2Inputs, rows, columns);
        int input1 = 0;
        int input2 = 0;
        printf("\n%s,\n\n", turn == 1 ? name1 : name2);
        printf("Choisissez la première case : ");
        input1 = validateInput(input1, rows, columns);
        if (input1 == -1) break; // End the game if no valid input
        inputPlayer(turn, 0, input1, player1Inputs, player2Inputs);
        system("clear");
        printGame(turn, name1, name2, points1, points2);
        printTable(gameString, player1Inputs, player2Inputs, rows, columns);
        printf("\n%s,\n\n", turn == 1 ? name1 : name2);
        printf("Choisissez la deuxième case : ");
        input2 = validateInput(input2, rows, columns);
        if (input2 == -1) break; // End the game if no valid input
        inputPlayer(turn, 0, input2, player1Inputs, player2Inputs);
        system("clear");
        printGame(turn, name1, name2, points1, points2);
        printTable(gameString, player1Inputs, player2Inputs, rows, columns);
        if (checkTurn(gameString, input1, input2, turn) == 1) {
            printf("\x1b[32mBien joué, vous avez trouvé une paire !\x1b[0m\n");
            system("sleep 1.2");
        } else {
            printf("\x1b[31mRaté, mais ne vous découragez pas !\x1b[0m\n");
            system("sleep 1.2");
            turn = (turn == 1) ? 2 : 1;
        }
        resetInputs(1, player1Inputs, player2Inputs);
        resetInputs(2, player2Inputs, player2Inputs);
        system("clear");
    }
    
    printTable(gameString, player1Inputs, player2Inputs, rows, columns);
    printFinalScore(name1, name2, points1, points2);

    free(discoveredCells);
    return 0;
}

/*
Seule une combinaison de nombre pair de lignes et de colonnes est possible
*/


